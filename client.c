#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9999
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <client_id>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int client_id = atoi(argv[1]);
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Создаем сокет
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Настраиваем адрес
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    // Подключаемся к серверу
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка подключения к серверу");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Отправляем идентификатор клиента
    snprintf(buffer, sizeof(buffer), "%d", client_id);
    send(client_socket, buffer, strlen(buffer), 0);

    // Получаем ответ от сервера
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0'; // Завершаем строку
        printf("Ответ от сервера: %s\n", buffer);
    }

    close(client_socket);
    return 0;
}
