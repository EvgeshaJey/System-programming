#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

extern char **environ;  // Объявление переменной environ

int main(void) {
    pid_t result;

    // Запуск календаря с execlp
    result = fork();
    if (result == 0) {
        execlp("gnome-calendar", "gnome-calendar", NULL);
        perror("Error launching gnome-calendar");
        exit(1);
    }

    // Запуск браузера с execvp
    result = fork();
    if (result == 0) {
        char *browser_args[] = { "firefox", NULL };
        execvp("firefox", browser_args);
        perror("Error launching firefox");
        exit(1);
    }

    // Запуск файлового менеджера с execl
    result = fork();
    if (result == 0) {
        execl("/usr/bin/nautilus", "nautilus", NULL);
        perror("Error launching nautilus");
        exit(1);
    }

    // Запуск текстового редактора с execle
    result = fork();
    if (result == 0) {
        execle("/usr/bin/gedit", "gedit", NULL, environ);
        perror("Error launching gedit");
        exit(1);
    }

    // Ожидание завершения всех процессов
    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }

    return 0;
}
