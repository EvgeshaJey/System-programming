#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define BUFFER_SIZE 4096

void error(const char *msg) {
perror(msg);
exit(1);
}

int main() {
SSL_CTX *ctx;
SSL *ssl;
int sockfd;
struct sockaddr_in server_addr;
struct hostent *host;
char *hostname = "bntu.by";
char *path = "/faculties/fitr";
char buffer[BUFFER_SIZE];

// Инициализация OpenSSL
SSL_library_init();
OpenSSL_add_all_algorithms();
SSL_load_error_strings();
ctx = SSL_CTX_new(TLS_client_method());
if (ctx == NULL) {
error("Ошибка при создании SSL_CTX");
}

// Получение IP-адреса хоста
host = gethostbyname(hostname);
if (host == NULL) {
error("Ошибка при получении IP-адреса");
}

// Создание сокета
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0) {
error("Ошибка при создании сокета");
}

// Настройка адреса сервера
memset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(443); // Порт HTTPS
memcpy(&server_addr.sin_addr.s_addr, host->h_addr, host->h_length);

// Подключение к серверу
if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
error("Ошибка при подключении");
}

// Создание SSL-соединения
ssl = SSL_new(ctx);
SSL_set_fd(ssl, sockfd);
if (SSL_connect(ssl) <= 0) {
error("Ошибка при SSL_connect");
}

// Формирование HTTP-запроса
sprintf(buffer, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, hostname);

// Отправка запроса
if (SSL_write(ssl, buffer, strlen(buffer)) <= 0) {
error("Ошибка при отправке запроса");
}

// Получение ответа
while (1) {
memset(buffer, 0, BUFFER_SIZE);
int bytes_received = SSL_read(ssl, buffer, BUFFER_SIZE - 1);
if (bytes_received <= 0) {
break; // Выход из цикла при завершении
}
printf("%s", buffer);
}

// Завершение работы с SSL
SSL_shutdown(ssl);
SSL_free(ssl);
close(sockfd);
SSL_CTX_free(ctx);
return 0;
}
