#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_NAME "my_fifo"

int main() {
    char buffer[256];
    
    while (1) {
        int fd = open(FIFO_NAME, O_RDONLY);
        read(fd, buffer, sizeof(buffer));
        printf("Получено: %s\n", buffer);
        close(fd);
    }
    return 0;
}
