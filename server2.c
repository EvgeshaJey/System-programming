#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define SHM_SIZE 1024

void handle_signal(int sig) {
    sem_unlink("/shared_semaphore");
    exit(0);
}

int main() {
    key_t key;
    int shmid;
    char *shm;
    sem_t *sem;

    // Ввод ключа
    printf("Enter key for shared memory (server 2): ");
    scanf("%d", &key);

    // Создание общей памяти
    shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    shm = shmat(shmid, NULL, 0);
    if (shm == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    // Открытие семафора
    sem = sem_open("/shared_semaphore", 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    signal(SIGINT, handle_signal); // Обработка сигнала для завершения

    while (1) {
        char message[SHM_SIZE];

        // Ввод сообщения
        printf("Server 2: Enter message: ");
        getchar(); // Очистка буфера
        fgets(message, SHM_SIZE, stdin);
        message[strcspn(message, "\n")] = 0; // Удаление новой строки

        // Запись в общую память
        sem_wait(sem); // Захват семафора
        snprintf(shm, SHM_SIZE, "Server 2: %s", message);
        sem_post(sem); // Освобождение семафора
    }

    // Освобождение ресурсов
    sem_close(sem);
    shmdt(shm);
    return 0;
}
