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
    exit(0);
}

int main() {
    key_t key;
    int shmid;
    char *shm;
    sem_t *sem;

    // Ввод ключа
    printf("Enter key for shared memory (client): ");
    scanf("%d", &key);

    // Получение общей памяти
    shmid = shmget(key, SHM_SIZE, 0666);
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
        sem_wait(sem); // Захват семафора
        printf("Received: %s\n", shm); // Чтение из общей памяти
        sem_post(sem); // Освобождение семафора
        sleep(1); // Задержка для уменьшения загрузки ЦП
    }

    // Освобождение ресурсов
    sem_close(sem);
    shmdt(shm);
    return 0;
}
