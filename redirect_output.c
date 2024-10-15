#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    FILE *output = fopen("output.txt", "w");
    if (output == NULL) {
        perror("Error opening output.txt");
        return 1;
    }

    int orig_stdout = dup(STDOUT_FILENO);

    if (dup2(fileno(output), STDOUT_FILENO) == -1) {
        perror("Error redirecting stdout");
        return 1;
    }

    printf("This will be written to output.txt\n");

    if (dup2(orig_stdout, STDOUT_FILENO) == -1) {
        perror("Error restoring stdout");
        return 1;
    }

    fclose(output);

    return 0;
}
