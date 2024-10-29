#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define SERVER_IP "::1"  // Используем IPv6 localhost
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in6 server_addr;
    char buffer[BUFFER_SIZE];

    // Создаем UDP-сокет (IPv6)
    client_socket = socket(AF_INET6, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    // Инициализируем адрес сервера (IPv6)
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, SERVER_IP, &server_addr.sin6_addr); // Использование IPv6-адреса
    server_addr.sin6_port = htons(SERVER_PORT);

    // Отправляем данные серверу
    strcpy(buffer, "Привет, сервер! (IPv6)");
    int bytes_sent = sendto(client_socket, buffer, strlen(buffer), 0,
                           (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bytes_sent == -1) {
        perror("Ошибка при отправке данных");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Данные отправлены серверу.\n");

    // Получаем ответ от сервера
    int bytes_received = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, NULL, NULL);
    if (bytes_received == -1) {
        perror("Ошибка при получении данных");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0';
    printf("Получен ответ от сервера: %s\n", buffer);

    close(client_socket);
    return 0;
}
