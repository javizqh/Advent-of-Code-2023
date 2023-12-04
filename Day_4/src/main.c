#define _GNU_SOURCE
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_ARRAY_SIZE 1024
#define MAX_CORRECT 1024
#define INPUT_FILE "input.txt"
#define TEST_FILE "test.txt"

int check_card(char *line);

int main(int argc, char const *argv[]) {
    FILE * fp;
    size_t len = 0;
    char * buff = NULL;
    int result = 0;
    int copies;
    int max_copies[MAX_CORRECT] = {};
    int to_copy = 0;
    int orig_copy = to_copy;

    fp = fopen(INPUT_FILE, "r");
    if (fp == NULL) err(EXIT_FAILURE, "open failed");

    while (getline(&buff, &len, fp) != -1) {
        // result += check_card(buff); // Part 1
        // Part 2 --------------------------------------------------------------
        copies = check_card(buff);

        result += max_copies[to_copy] + 1;

        orig_copy = to_copy;
        to_copy = (to_copy + 1) % MAX_CORRECT;

        for (size_t i = to_copy; i < copies + to_copy % MAX_CORRECT;
             i = (i + 1) %  MAX_CORRECT) 
        {
            max_copies[i] += (max_copies[orig_copy] + 1); 
        }

        max_copies[orig_copy] = 0; // Restore card copies
        // Part 2 --------------------------------------------------------------
    }

    free(buff);
    fclose(fp);
    printf("Result: %d\n", result);
    return 0;
}

// Part 1 & 2 ------------------------------------------------------------------
int check_card(char *line) {
    int numbers[1024];
    int numbers_i = 0;
    int curr_num = 0;
    int result = 0;
    char * ptr;

    for (ptr = line; *ptr != ':'; ptr++) {} // Skip until :

    for (ptr; *ptr != '\0'; ptr++) {
        if (*ptr >= '0' && *ptr <= '9') {
            curr_num = 10 * curr_num + *ptr - '0';
        } else if (curr_num > 0) {
            numbers[numbers_i] = curr_num;
            numbers_i++;
            curr_num = 0;
        }
    }

    for (int x = 0; x < numbers_i; x++) {
        for (int y = x; y < numbers_i; y++) {
            if (numbers[x] == numbers[y] && x != y) {
                // Part 1
                // if (result == 0) result = 1;
                // else result *= 2;
                // Part 2
                result++;
            }
        }
    }

    return result;
}

// Part 1 & 2 ------------------------------------------------------------------
