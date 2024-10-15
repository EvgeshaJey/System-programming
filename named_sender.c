#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#define FIFO_NAME "my_fifo"

void print_time() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("Системное время: %02d:%02d:%02d, Дата: %04d-%02d-%02d\n",
           tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_year + 1900,
           tm.tm_mon + 1, tm.tm_mday);
}

int main() {
    mkfifo(FIFO_NAME, 0666);
    for (int week = 1; week <= 16; week++) {
        int fd = open(FIFO_NAME, O_WRONLY);
        
        char message[256];
        if (week <= 4) {
            snprintf(message, sizeof(message), "Неделя %d: ", week);
            print_time();
        } else if (week <= 7) {
            snprintf(message, sizeof(message), "Неделя %d: Получение календаря. ", week);
            print_time();
        } else if (week <= 10) {
            snprintf(message, sizeof(message), "Неделя %d: Имя ядра и ОС. ", week);
            print_time();
        } else {
            snprintf(message, sizeof(message), "Неделя %d: Информация о ОС и версии. ", week);
            print_time();
        }

        write(fd, message, sizeof(message));
        close(fd);
        sleep(1); // Задержка для синхронизации
    }
    return 0;
}
