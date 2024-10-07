#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ABOOK_FNAME "abook"
#define NAME_LENGTH 50
#define PHONE_LENGTH 30
#define EMAIL_LENGTH 30

struct ab_entry {
    char name[NAME_LENGTH];
    char phone[PHONE_LENGTH];
    char email[EMAIL_LENGTH];
};

void abook_failed(int retcode) {
    fprintf(stderr, "Cannot open address book\n");
    exit(retcode);
}

void abook_add(void) {
    struct ab_entry entry;
    printf("Name: ");
    scanf("%49s", entry.name);  // Ограничиваем ввод до длины буфера
    printf("Phone number: ");
    scanf("%29s", entry.phone);  // Ограничиваем ввод до длины буфера
    printf("E-mail: ");
    scanf("%29s", entry.email);  // Ограничиваем ввод до длины буфера

    FILE *file = fopen(ABOOK_FNAME, "a");  // Открываем файл для добавления
    if (file == NULL) abook_failed(1);

    fprintf(file, "%s %s %s\n", entry.name, entry.phone, entry.email);  // Записываем запись в файл

    fclose(file);  // Закрываем файл
}

void abook_find(void) {
    char find_buffer[NAME_LENGTH];
    struct ab_entry entry;
    int found = 0;

    printf("Name: ");
    scanf("%49s", find_buffer);  // Ограничиваем ввод

    FILE *file = fopen(ABOOK_FNAME, "r");  // Открываем файл для чтения
    if (file == NULL) abook_failed(1);

    // Читаем построчно данные из файла
    while (fscanf(file, "%49s %29s %29s", entry.name, entry.phone, entry.email) == 3) {
        if (strcmp(find_buffer, entry.name) == 0) {
            printf("Phone: %s\n", entry.phone);
            printf("E-mail: %s\n", entry.email);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Name '%s' hasn't been found\n", find_buffer);
    }

    fclose(file);  // Закрываем файл
}

void abook_delete(void) {
    char find_buffer[NAME_LENGTH];
    struct ab_entry entry;
    int found = 0;

    printf("Name: ");
    scanf("%49s", find_buffer);  // Ограничиваем ввод

    FILE *file = fopen(ABOOK_FNAME, "r");  // Открываем файл для чтения
    if (file == NULL) abook_failed(1);

    // Временный файл для записи оставшихся записей
    FILE *temp_file = fopen("/tmp/abook_temp", "w");
    if (temp_file == NULL) abook_failed(1);

    // Читаем записи из исходного файла
    while (fscanf(file, "%49s %29s %29s", entry.name, entry.phone, entry.email) == 3) {
        if (strcmp(find_buffer, entry.name) == 0) {
            printf("Deleted: %s\n", entry.name);
            found = 1;
        } else {
            // Записываем все оставшиеся записи во временный файл
            fprintf(temp_file, "%s %s %s\n", entry.name, entry.phone, entry.email);
        }
    }

    fclose(file);  // Закрываем исходный файл
    fclose(temp_file);  // Закрываем временный файл

    // Заменяем исходный файл временным
    if (remove(ABOOK_FNAME) != 0) {
        fprintf(stderr, "Error deleting original file\n");
        exit(1);
    }
    if (rename("/tmp/abook_temp", ABOOK_FNAME) != 0) {
        fprintf(stderr, "Error renaming temporary file\n");
        exit(1);
    }

    if (!found) {
        printf("Name '%s' hasn't been found\n", find_buffer);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Too few arguments\n");
        return 1;
    }

    if (strcmp(argv[1], "add") == 0) {
        abook_add();
    } else if (strcmp(argv[1], "find") == 0) {
        abook_find();
    } else if (strcmp(argv[1], "delete") == 0) {
        abook_delete();
    } else {
        fprintf(stderr, "%s: unknown command\nUsage: abook {add, find, delete}\n", argv[1]);
        return 1;
    }

    return 0;
}
