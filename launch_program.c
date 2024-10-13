#include <stdio.h>
#include <unistd.h>

int main(void) {
    // Запуск браузера или текстового редактора
    char *browser_args[] = { "firefox", NULL }; // Замените на ваш браузер
    char *editor_args[] = { "gedit", NULL }; // Замените на ваш текстовый редактор

    pid_t browser_pid = fork();
    if (browser_pid == 0) {
        execve("/usr/bin/firefox", browser_args, NULL);
        fprintf(stderr, "Error launching browser\n");
        return 1;
    }

    pid_t editor_pid = fork();
    if (editor_pid == 0) {
        execve("/usr/bin/gedit", editor_args, NULL);
        fprintf(stderr, "Error launching editor\n");
        return 1;
    }

    printf("Launched browser with PID=%d\n", browser_pid);
    printf("Launched editor with PID=%d\n", editor_pid);

    return 0;
}
