#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 5

int server_socket;
int client_sockets[MAX_CLIENTS];
int num_clients = 0;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    char buffer[1024];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            break;
        }

        printf("Received message from client: %s\n", buffer);
        send(client_socket, buffer, strlen(buffer), 0);
    }

    pthread_mutex_lock(&client_mutex);
    for (int i = 0; i < num_clients; i++) {
        if (client_sockets[i] == client_socket) {
            client_sockets[i] = -1;
            num_clients--;
            break;
        }
    }
    pthread_mutex_unlock(&client_mutex);

    close(client_socket);
    printf("Client disconnected.\n");
    return NULL;
}

int main() {
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_size;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket");
        exit(1);
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("bind");
        exit(1);
    }

    if (listen(server_socket, 5) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_size);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }

        pthread_mutex_lock(&client_mutex);
        if (num_clients < MAX_CLIENTS) {
            client_sockets[num_clients] = client_socket;
            num_clients++;

            pthread_t thread;
            if (pthread_create(&thread, NULL, handle_client, &client_sockets[num_clients - 1]) != 0) {
                perror("pthread_create");
                close(client_socket);
            }
        } else {
            close(client_socket);
            printf("Maximum number of clients reached.\n");
        }
        pthread_mutex_unlock(&client_mutex);
    }

    close(server_socket);
    pthread_mutex_destroy(&client_mutex);
    return 0;
}
