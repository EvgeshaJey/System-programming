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

    // Открытие семафора
    sem = sem_open(SEM_NAME, 0);

    // Отправка данных клиенту
    while (1) {
        sleep(3); // Задержка перед отправкой сообщения
        sem_wait(sem); // Захват семафора
        snprintf(mapped, SIZE, "Message from Server 2: %ld", time(NULL));
        printf("Server 2 sent message.\n");
        sem_post(sem); // Освобождение семафора
    }

    munmap(mapped, SIZE);
    close(fd);
    sem_close(sem);
    return 0;
}
