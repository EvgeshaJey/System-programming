#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9999
#define BUFFER_SIZE 1024

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    free(arg);
    char buffer[BUFFER_SIZE];

    while (1) {
        // Чтение запроса от клиента
        int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0) {
            break; // Если чтение не удалось, выходим
        }
        buffer[bytes_read] = '\0'; // Завершаем строку

        // Определяем ответ в зависимости от клиента
        int client_id = atoi(buffer);
        int start, end;
        if (client_id == 1) {
            start = 1; end = 10;
        } else if (client_id == 2) {
            start = 100; end = 110;
        } else if (client_id == 3) {
            start = 200; end = 210;
        } else {
            snprintf(buffer, sizeof(buffer), "Неизвестный клиент: %d", client_id);
            send(client_socket, buffer, strlen(buffer), 0);
            continue;
        }

        // Формируем ответ
        snprintf(buffer, sizeof(buffer), "Счет от %d до %d: ", start, end);
        for (int i = start; i <= end; i++) {
            char num[10];
            sprintf(num, "%d ", i);
            strcat(buffer, num);
        }
        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    return NULL;
}

int main() {
    int server_socket, *client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Создаем сокет
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Настраиваем адрес
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Привязываем сокет к адресу
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка привязки сокета");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Слушаем входящие соединения
    listen(server_socket, 5);
    printf("Сервер запущен на порту %d\n", PORT);

    while (1) {
        client_socket = malloc(sizeof(int));
        *client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (*client_socket < 0) {
            perror("Ошибка при принятии соединения");
            free(client_socket);
            continue;
        }

        printf("Клиент подключен.\n");
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_socket);
        pthread_detach(tid); // Автоматически освобождаем поток
    }

    close(server_socket);
    return 0;
}
