#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void createZombie() {
    pid_t pid = fork();
    if (pid == 0) {
        // Дочерний процесс
        printf("Child process (PID: %d) created, will become a zombie.\n", getpid());
        // Завершение дочернего процесса без вызова wait
        exit(0);
    } else if (pid > 0) {
        // Родительский процесс
        printf("Parent process (PID: %d) waiting for child to become a zombie.\n", getpid());
        sleep(2); // Даем время дочернему процессу стать зомби
        printf("Parent process (PID: %d) will now wait for the child.\n", getpid());
        wait(NULL); // Ожидание завершения дочернего процесса
        printf("Child process reaped.\n");
    } else {
        perror("fork");
        exit(1);
    }
}

void createChildProcess() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Дочерний процесс
        printf("Child process (PID: %d) is running.\n", getpid());
        sleep(3); // Имитация работы
        printf("Child process (PID: %d) finished.\n", getpid());
        exit(0);
    } else {
        // Родительский процесс
        int status;
        printf("Parent process (PID: %d) waiting for child (PID: %d).\n", getpid(), pid);
        waitpid(pid, &status, 0);
        printf("Parent process (PID: %d) detected child process finished.\n", getpid());
    }
}

int main() {
    printf("Process Management Example\n");
    createZombie();
    createChildProcess();
    return 0;
}
