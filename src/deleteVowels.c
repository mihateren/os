#include <stdio.h>
#include "../include/deleteVowels.h"

void deleteVowels(const char* str, FILE* file) {
    if (file == NULL) {
        printf("Ошибка: файл не открыт.\n");
        return;
    }

    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (!(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
              c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')) {
            if (fputc(c, file) == EOF) {
                printf("Ошибка записи в файл.\n");
                return;
            }
        }
    }
    fputc('\n', file);
}
