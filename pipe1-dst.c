#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // Добавлено для atoi

int main(int argc, char *argv[]) {
    int read_fd = atoi(argv[1]);
    char buffer[32];
    read(read_fd, buffer, sizeof(buffer));
    printf("Received: %s\n", buffer);
    close(read_fd);
    return 0;
}
