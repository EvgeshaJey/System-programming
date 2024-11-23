#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

#define SHM_NAME "/my_shm"
#define SEM_NAME "/my_semaphore"
#define SIZE 256

int main() {
    int fd;
    char *mapped;
    sem_t *sem;

    // Открытие существующего файла
    fd = shm_open(SHM_NAME, O_RDWR, 0666);
    mapped = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Открытие семафора
    sem = sem_open(SEM_NAME, 0);

    // Получение данных от серверов
    while (1) {
        sleep(1); // Задержка для получения сообщений
        sem_wait(sem); // Захват семафора
        if (strlen(mapped) > 0) {
            printf("Client received: %s\n", mapped);
            memset(mapped, 0, SIZE); // Очистка для следующего сообщения
        }
        sem_post(sem); // Освобождение семафора
    }

    munmap(mapped, SIZE);
    close(fd);
    sem_close(sem);
    return 0;
}
