#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <limits.h>

#define MAX_WORD_LENGTH 100

int is_text_file(const char *filename) {
    size_t len = strlen(filename);
    if (len < 4) return 0; // Файлы с короткими именами точно не *.txt
    return (strcmp(filename + len - 4, ".txt") == 0);
}

int count_words_in_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Не удалось открыть файл: %s\n", filename);
        return -1;
    }

    int word_count = 0;
    int in_word = 0;
    char word[MAX_WORD_LENGTH];
    int word_length = 0;

    int c;
    while ((c = fgetc(file)) != EOF) {
        if (isalnum(c)) {
            if (word_length < MAX_WORD_LENGTH - 1) {
                word[word_length++] = (char) c;
            }
            in_word = 1;
        } else {
            if (in_word) {
                word[word_length] = '\0';
                word_count++;
                in_word = 0;
                word_length = 0;
            }
        }
    }

    if (in_word) {
        word[word_length] = '\0';
        word_count++;
    }

    fclose(file);
    return word_count;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Формат вызова программы: %s <путь>\n", argv[0]);
        return 1;
    }

    const char *directory_path = argv[1];
    DIR *dir = opendir(directory_path);
    if (dir == NULL) {
        fprintf(stderr, "Не удалось открыть каталог: %s\n", directory_path);
        return 1;
    }

    struct dirent *entry;
    int found_text_files = 0; // Добавляем переменную для отслеживания .txt файлов
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && is_text_file(entry->d_name)) {
            char filepath[PATH_MAX];
            snprintf(filepath, PATH_MAX, "%s/%s", directory_path, entry->d_name);

            int word_count = count_words_in_file(filepath);
            if (word_count >= 0) {
                printf("%s - %d\n", entry->d_name, word_count);
                found_text_files = 1; // Устанавливаем, что нашли .txt файл
            }
        }
    }

    closedir(dir);

    if (!found_text_files) {
        printf("Нет *.txt файлов в указанной директории.\n");
    }

    return 0;
}
