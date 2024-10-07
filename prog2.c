#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ABOOK_FNAME "abook"
#define NAME_LENGTH 50
#define PHONE_LENGTH 30
#define EMAIL_LENGTH 30

struct iovec ab_entry[3];
char name_buffer[NAME_LENGTH];
char phone_buffer[PHONE_LENGTH];
char email_buffer[EMAIL_LENGTH];

void abook_failed(int retcode) {
    fprintf(stderr, "Cannot open address book\n");
    exit(retcode);
}

void abook_delete(void) {
    char find_buffer[NAME_LENGTH];
    char tfile[] = "/tmp/abook-XXXXXX";
    char ch;
    int tfd = mkstemp(tfile);
    if (tfd == -1) {
        fprintf(stderr, "mkstemp() error\n");
        exit(1);
    }
    unlink(tfile);
    
    printf("Name: ");
    scanf("%s", find_buffer);
    
    int fd = open(ABOOK_FNAME, O_RDONLY);
    if (fd == -1) abook_failed(1);

    while (readv(fd, ab_entry, 3) > 0) {
        if (!strcmp(find_buffer, ab_entry[0].iov_base)) {
            printf("Delete: %s\n", (char*)ab_entry[0].iov_base);
            continue;
        }
        writev(tfd, ab_entry, 3);
    }
    close(fd);

    lseek(tfd, 0, SEEK_SET);
    fd = open(ABOOK_FNAME, O_WRONLY | O_TRUNC);
    if (fd == -1) abook_failed(1);
    
    while (read(tfd, &ch, 1) > 0) write(fd, &ch, 1);
    
    close(fd);
    close(tfd);
}

void abook_add(void) {
    ab_entry[0].iov_base = name_buffer;
    ab_entry[0].iov_len = NAME_LENGTH;
    ab_entry[1].iov_base = phone_buffer;
    ab_entry[1].iov_len = PHONE_LENGTH;
    ab_entry[2].iov_base = email_buffer;
    ab_entry[2].iov_len = EMAIL_LENGTH;

    printf("Name: ");
    scanf("%s", name_buffer);
    printf("Phone number: ");
    scanf("%s", phone_buffer);
    printf("E-mail: ");
    scanf("%s", email_buffer);

    int fd = open(ABOOK_FNAME, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP);
    if (fd == -1) abook_failed(1);
    
    if (writev(fd, ab_entry, 3) <= 0) {
        fprintf(stderr, "Cannot write to address book\n");
        exit(1);
    }
    close(fd);
}

void abook_find(void) {
    char find_buffer[NAME_LENGTH];
    printf("Name: ");
    scanf("%s", find_buffer);

    int fd = open(ABOOK_FNAME, O_RDONLY);
    if (fd == -1) abook_failed(1);

    while (readv(fd, ab_entry, 3) > 0) {
        if (!strcmp(find_buffer, ab_entry[0].iov_base)) {
            printf("Phone: %s\n", (char*)ab_entry[1].iov_base);
            printf("E-mail: %s\n", (char*)ab_entry[2].iov_base);
            close(fd);
            return;
        }
    }
    printf("Name '%s' hasn't found\n", find_buffer);
    close(fd);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Too few arguments\n");
        return 1;
    }

    ab_entry[0].iov_base = name_buffer;
    ab_entry[0].iov_len = NAME_LENGTH;
    ab_entry[1].iov_base = phone_buffer;
    ab_entry[1].iov_len = PHONE_LENGTH;
    ab_entry[2].iov_base = email_buffer;
    ab_entry[2].iov_len = EMAIL_LENGTH;

    if (!strcmp(argv[1], "add")) {
        abook_add();
    } else if (!strcmp(argv[1], "find")) {
        abook_find();
    } else if (!strcmp(argv[1], "delete")) {
        abook_delete();
    } else {
        fprintf(stderr, "%s: unknown command\nUsage: abook {add, find, delete}\n", argv[1]);
        return 1;
    }

    return 0;
}
