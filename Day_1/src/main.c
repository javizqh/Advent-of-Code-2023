#define _GNU_SOURCE
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024
#define INPUT_FILE "input.txt"

typedef struct {
    char * ptr;
    int value;
} digit;

const char *digits[9] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

int get_numbers(char *line);
int get_numbers_and_digits(char *line);

int main(int argc, char const *argv[]) {
    FILE * fp;
    size_t len = 0;
    char * buff = NULL;
    int result = 0;

    fp = fopen(INPUT_FILE, "r");
    if (fp == NULL) err(EXIT_FAILURE, "open failed");

    while (getline(&buff, &len, fp) != -1) {
        // result += get_numbers(buff); // Part 1
        result += get_numbers_and_digits(buff); // Part 2
    }

    free(buff);
    fclose(fp);
    printf("Result: %d\n", result);
    return 0;
}

// Part 1 ----------------------------------------------------------------------
int get_numbers(char *line) {
    // Part 1
    int first = -1, last = 0;

    for (char *ptr = line; *ptr != '\0'; ptr++) {
        if (*ptr > '0' && *ptr <= '9') {
            last = *ptr - '0';
            if (first == -1) first = last;
        }
    }
    return 10 * first + last;
}
// Part 1 ----------------------------------------------------------------------

// Part 2 ----------------------------------------------------------------------
digit * get_first_digit(char *line) {
    char *last;
    digit *first = malloc(sizeof(digit));
    if (first == NULL) return NULL;
    
    first->ptr = NULL;

    for (int i = 0; i < 9; i++) {
        last = strstr(line,digits[i]);
        if (last == NULL) continue;
        if (first->ptr == NULL || last < first->ptr) {
            first->ptr = last;
            first->value = i + 1;
        }
    }
    return first;
}

digit * get_last_digit(char *line) {
    char *first;
    digit *last = malloc(sizeof(digit));
    if (last == NULL) return NULL;
    
    last->ptr = NULL;

    for (int i = 0; i < 9; i++) {
        while((first = strstr(line,digits[i])) != NULL) {
            line++;
            if (last->ptr == NULL || first > last->ptr) {
                last->ptr = first;
                last->value = i + 1;
            }
        }
    }
    return last;
}

int get_numbers_and_digits(char *line) {
    int first = -1, last = 0;
    char * last_ptr;

    digit * first_digit = get_first_digit(line);
    digit * last_digit = get_last_digit(line);

    for (char *ptr = line; *ptr != '\0'; ptr++) {
        if (*ptr > '0' && *ptr <= '9') {
            last = *ptr - '0';
            last_ptr = ptr;
            if (first == -1) {
                if (first_digit->ptr == NULL || ptr < first_digit->ptr) first = last;
                else first = first_digit->value;
            } 
        }
    }

    if (last_digit->ptr != NULL && last_ptr < last_digit->ptr) last = last_digit->value; 

    free(first_digit);
    free(last_digit);

    return 10 * first + last; 
}

// Part 2 ----------------------------------------------------------------------
