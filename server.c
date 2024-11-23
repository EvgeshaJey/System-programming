#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SHM_NAME "/my_shm"
#define SIZE 256

int main() {
    int fd;
    char *mapped;

    // Создание файла
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);

    // Маппинг файла в память
    mapped = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Ожидание ввода от клиентов
    while (1) {
        printf("Server: Waiting for input...\n");
        sleep(1); // Задержка для предотвращения перегрузки CPU
        if (strlen(mapped) > 0) {
            printf("Server received: %s\n", mapped);
            memset(mapped, 0, SIZE); // Очистка для следующего ввода
        }
    }

    munmap(mapped, SIZE);
    close(fd);
    shm_unlink(SHM_NAME);
    return 0;
}
