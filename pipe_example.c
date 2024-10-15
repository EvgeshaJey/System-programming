#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#define STR_SIZE 32

int main(void) {
    int pf[2];
    int pid1, pid2;
    char spf[2][STR_SIZE];

    // Создание канала
    if (pipe(pf) == -1) {
        fprintf(stderr, "pipe() error\n");
        return 1;
    }

    // Запись в строку
    sprintf(spf[0], "%d", pf[0]);
    sprintf(spf[1], "%d", pf[1]);

    // Первый процесс
    if ((pid1 = fork()) == 0) {
        close(pf[0]); // Закрываем неиспользуемый конец
        execl("./pipe1-src", "pipe1-src", spf[1], NULL);
        fprintf(stderr, "exec() [src] error\n");
        return 1;
    }

    // Второй процесс
    if ((pid2 = fork()) == 0) {
        close(pf[1]); // Закрываем неиспользуемый конец
        execl("./pipe1-dst", "pipe1-dst", spf[0], NULL);
        fprintf(stderr, "exec() [dst] error\n");
        return 1;
    }

    // Ожидание завершения процессов
    waitpid(pid1, NULL, 0);
    close(pf[0]);
    close(pf[1]);
    waitpid(pid2, NULL, 0);
    return 0;
}
