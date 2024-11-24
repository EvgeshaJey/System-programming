#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define SIZE 1024

void handle_signal(int sig) {
    // Очистка ресурсов при завершении
    sem_unlink("/chat_semaphore");
    exit(0);
}

int main() {
    char input[SIZE];
    key_t key1, key2;
    int shmid1, shmid2;
    char *shm1, *shm2;
    sem_t *sem;

    // Ввод ключей
    printf("Enter key for shared memory 1: ");
    scanf("%d", &key1);
    printf("Enter key for shared memory 2: ");
    scanf("%d", &key2);

    // Создание первого объекта общей памяти
    shmid1 = shmget(key1, SIZE, 0666 | IPC_CREAT);
    if (shmid1 == -1) {
        perror("shmget");
        exit(1);
    }
    shm1 = shmat(shmid1, NULL, 0);
    if (shm1 == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    // Создание второго объекта общей памяти
    shmid2 = shmget(key2, SIZE, 0666 | IPC_CREAT);
    if (shmid2 == -1) {
        perror("shmget");
        exit(1);
    }
    shm2 = shmat(shmid2, NULL, 0);
    if (shm2 == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    // Создание семафора
    sem = sem_open("/chat_semaphore", O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    signal(SIGINT, handle_signal); // Обработка сигнала для завершения

    while (1) {
        // Чтение из общего объекта 1
        sem_wait(sem); // Захват семафора
        printf("Message from other user: %s\n", shm2);
        sem_post(sem); // Освобождение семафора

        // Ввод сообщения
        printf("Enter your message: ");
        getchar(); // Очистка буфера
        fgets(input, SIZE, stdin);
        input[strcspn(input, "\n")] = 0; // Удаление новой строки

        // Запись в общий объект 1
        sem_wait(sem); // Захват семафора
        strncpy(shm1, input, SIZE);
        sem_post(sem); // Освобождение семафора
    }

    // Освобождение ресурсов
    sem_close(sem);
    shmdt(shm1);
    shmdt(shm2);
    return 0;
}
