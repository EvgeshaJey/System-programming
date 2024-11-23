#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define SHM_NAME "/my_shm"
#define SEM_NAME "/my_semaphore"
#define SIZE 256

int main() {
    int fd;
    char *mapped;
    sem_t *sem;

    // Создание файла
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);

    // Маппинг файла в память
    mapped = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Создание семафора
    sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);

    // Отправка данных клиенту
    while (1) {
        sleep(2); // Задержка перед отправкой сообщения
        sem_wait(sem); // Захват семафора
        snprintf(mapped, SIZE, "Message from Server 1: %ld", time(NULL));
        printf("Server 1 sent message.\n");
        sem_post(sem); // Освобождение семафора
    }

    munmap(mapped, SIZE);
    close(fd);
    shm_unlink(SHM_NAME);
    sem_close(sem);
    sem_unlink(SEM_NAME);
    return 0;
}
