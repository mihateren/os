#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "include/deleteVowels.h"

#define MAX_LENGTH_OF_LINE 30 // Максимальная длина строки
#define MAX_AMOUNT_OF_LINES 100 // Максимальное количество строк
#define FILENAME_SIZE 10 // Размер для хранения имени файла

// Функция для обработки ошибок создания процесса
void handle_error(int result_of_statement) {
    if (result_of_statement == -1) { // Если операция вернула -1, значит произошла ошибка
        perror("Ошибка создания процесса: ");
        exit(-1); // Завершение программы с ошибкой
    }
}

// Функция для попытки создания нового процесса (fork)
pid_t try_to_create_process() {
    pid_t process_id = fork(); // Создание нового процесса
    handle_error(process_id); // Обработка ошибки, если fork не удался
    return process_id; // Возвращаем ID процесса
}

int main() {
    char file_first_name[FILENAME_SIZE]; // Имя файла для первого дочернего процесса
    char file_second_name[FILENAME_SIZE]; // Имя файла для второго дочернего процесса

    char lines[MAX_AMOUNT_OF_LINES][MAX_LENGTH_OF_LINE]; // Массив для хранения всех строк

    // Ввод имени файла для первого дочернего процесса
    printf("Имя первого файла: ");
    scanf("%s", file_first_name);

    // Ввод имени файла для второго дочернего процесса
    printf("Имя второго файла: ");
    scanf("%s", file_second_name);

    int amount_lines; // Количество вводимых строк
    printf("Теперь введите количество строк (не больше %d): ", MAX_AMOUNT_OF_LINES);
    scanf("%d", &amount_lines); // Ввод количества строк

    printf("Теперь вводите сами строки.\n");

    char lines_for_first_process[MAX_AMOUNT_OF_LINES][MAX_LENGTH_OF_LINE]; // Массив строк для первого процесса
    char lines_for_second_process[MAX_AMOUNT_OF_LINES][MAX_LENGTH_OF_LINE]; // Массив строк для второго процесса

    int amount_of_lines_for_first_process = 0; // Счётчик строк для первого процесса
    int amount_of_lines_for_second_process = 0; // Счётчик строк для второго процесса

    // Ввод строк и распределение их между процессами
    for (int i = 0; i < amount_lines; i++) {
        scanf("%s", lines[i]); // Ввод очередной строки
        if (strlen(lines[i]) > 10) { // Если длина строки больше 10 символов
            strcpy(lines_for_first_process[amount_of_lines_for_first_process++], lines[i]); // Строка для первого процесса
        } else {
            strcpy(lines_for_second_process[amount_of_lines_for_second_process++], lines[i]); // Строка для второго процесса
        }
    }

    int pipe_for_first_child[2]; // Канал для первого дочернего процесса (pipe)
    int pipe_for_second_child[2]; // Канал для второго дочернего процесса (pipe)

    handle_error(pipe(pipe_for_first_child)); // Создание pipe для первого процесса
    handle_error(pipe(pipe_for_second_child)); // Создание pipe для второго процесса

    pid_t first_process_id = try_to_create_process(); // Создание первого дочернего процесса

    if (first_process_id == 0) {
        // Первый дочерний процесс

        close(pipe_for_first_child[1]); // Закрываем конец для записи, так как процесс будет только читать
        dup2(pipe_for_first_child[0], STDIN_FILENO); // Перенаправляем pipe на стандартный ввод

        FILE* file = fopen(file_first_name, "w"); // Открываем файл для записи
        if (file == NULL) {
            printf("Ошибка открытия файла: %s\n", file_first_name); // Обработка ошибки открытия файла
            exit(1);
        }

        char line[MAX_LENGTH_OF_LINE]; // Буфер для строки
        while (read(STDIN_FILENO, line, sizeof(line)) > 0) { // Чтение строк из pipe
            deleteVowels(line, file); // Удаляем гласные из строки и записываем в файл
        }

        fclose(file); // Закрываем файл
        close(pipe_for_first_child[0]); // Закрываем конец для чтения pipe
        exit(0); // Завершаем первый дочерний процесс

    } else {
        // Родительский процесс

        close(pipe_for_first_child[0]); // Закрываем конец для чтения, так как процесс будет только записывать

        // Отправляем данные в первый дочерний процесс через pipe
        for (int i = 0; i < amount_of_lines_for_first_process; i++) {
            write(pipe_for_first_child[1], lines_for_first_process[i], sizeof(lines_for_first_process[i])); // Записываем строки
        }
        close(pipe_for_first_child[1]); // Закрываем конец для записи pipe после отправки данных

        wait(NULL); // Ждём завершения первого дочернего процесса
        printf("Первый процесс завершён.\n");
    }

    pid_t second_process_id = try_to_create_process(); // Создание второго дочернего процесса

    if (second_process_id == 0) {
        // Второй дочерний процесс

        close(pipe_for_second_child[1]); // Закрываем конец для записи, так как процесс будет только читать
        dup2(pipe_for_second_child[0], STDIN_FILENO); // Перенаправляем pipe на стандартный ввод

        FILE* file = fopen(file_second_name, "w"); // Открываем файл для записи
        if (file == NULL) {
            printf("Ошибка открытия файла: %s\n", file_second_name); // Обработка ошибки открытия файла
            exit(1);
        }

        char line[MAX_LENGTH_OF_LINE]; // Буфер для строки
        while (read(STDIN_FILENO, line, sizeof(line)) > 0) { // Чтение строк из pipe
            deleteVowels(line, file); // Удаляем гласные из строки и записываем в файл
        }

        fclose(file); // Закрываем файл
        close(pipe_for_second_child[0]); // Закрываем конец для чтения pipe
        exit(0); // Завершаем второй дочерний процесс

    } else {
        // Родительский процесс

        close(pipe_for_second_child[0]); // Закрываем конец для чтения, так как процесс будет только записывать

        // Отправляем данные во второй дочерний процесс через pipe
        for (int i = 0; i < amount_of_lines_for_second_process; i++) {
            write(pipe_for_second_child[1], lines_for_second_process[i], sizeof(lines_for_second_process[i])); // Записываем строки
        }
        close(pipe_for_second_child[1]); // Закрываем конец для записи pipe после отправки данных

        wait(NULL); // Ждём завершения второго дочернего процесса
        printf("Второй процесс завершён.\n");
    }

    return 0; // Завершение программы
}
