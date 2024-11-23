#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>

#define MAX_TEXT 100
#define MSG_TYPE 1

struct message {
    long msg_type;
    char text[MAX_TEXT];
    char sender[20];
};

sem_t *sem;

void handle_signal(int sig) {
    sem_close(sem);
    sem_unlink("/msg_semaphore"); // Удаление семафора
    exit(0);
}

int main() {
    int msgid;
    struct message msg;
    key_t key = ftok("msg_queue", 65); // Генерация ключа

    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    // Создание семафора
    sem = sem_open("/msg_semaphore", O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    signal(SIGINT, handle_signal); // Обработка сигнала для завершения

    while (1) {
        printf("Enter your name: ");
        scanf("%s", msg.sender);
        printf("Enter message: ");
        getchar(); // Очистка буфера
        fgets(msg.text, MAX_TEXT, stdin);
        msg.text[strcspn(msg.text, "\n")] = 0; // Удаление новой строки
        msg.msg_type = MSG_TYPE;

        sem_wait(sem); // Захват семафора
        msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);
        sem_post(sem); // Освобождение семафора

        printf("Message sent: %s from %s\n", msg.text, msg.sender);
    }

    sem_close(sem);
    return 0;
}
