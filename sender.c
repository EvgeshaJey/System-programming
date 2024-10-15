#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void print_time() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("Системное время: %02d:%02d:%02d, Дата: %04d-%02d-%02d\n",
           tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_year + 1900,
           tm.tm_mon + 1, tm.tm_mday);
}

int main() {
    int fd[2];
    pipe(fd);

    if (fork() == 0) { // Дочерний процесс
        close(fd[1]); // Закрываем запись
        char buffer[256];
        read(fd[0], buffer, sizeof(buffer));
        printf("Получено: %s\n", buffer);
        close(fd[0]);
        exit(0);
    } else { // Родительский процесс
        close(fd[0]); // Закрываем чтение
        // Отправка данных в зависимости от недели
        for (int week = 1; week <= 16; week++) {
            char message[256];
            if (week <= 4) {
                snprintf(message, sizeof(message), "Неделя %d: ", week);
                print_time();
                write(fd[1], message, sizeof(message));
            } else if (week <= 7) {
                snprintf(message, sizeof(message), "Неделя %d: Получение календаря. ", week);
                print_time();
                write(fd[1], message, sizeof(message));
            } else if (week <= 10) {
                snprintf(message, sizeof(message), "Неделя %d: Имя ядра и ОС. ", week);
                print_time();
                write(fd[1], message, sizeof(message));
            } else {
                snprintf(message, sizeof(message), "Неделя %d: Информация о ОС и версии. ", week);
                print_time();
                write(fd[1], message, sizeof(message));
            }
        }
        close(fd[1]);
    }
    return 0;
}
