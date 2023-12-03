#define _GNU_SOURCE
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024
#define INPUT_FILE "input.txt"
#define TEST_FILE "test.txt"

typedef struct num{
    int value;
    int line;
    int start_col;
    int end_col;
    int is_adjacent;
    struct num * next;
} number;

typedef struct sym{
    int line;
    int column;
    int n_adj;
    int ratio;
    struct sym * next;
} symbol;

number * new_number(number * old_number) {
    number * num = malloc(sizeof(number));
    if (num == NULL) return NULL;
    memset(num, 0, sizeof(number));

    old_number->next = num;
    old_number->is_adjacent = 0;
    num->is_adjacent = 0;

    return num;
}

symbol * new_symbol(symbol * old_symbol) {
    symbol * sym = malloc(sizeof(symbol));
    if (sym == NULL) return NULL;
    memset(sym, 0, sizeof(symbol));

    old_symbol->next = sym;
    sym->line = -10;
    sym->column = -10;
    old_symbol->n_adj = 0;
    old_symbol->ratio = 0;

    return sym;
}

int check_if_adyacent(number *first_num, symbol *first_sym);
int check_if_adyacent_2(number *first_num, symbol *first_sym);

int main(int argc, char const *argv[]) {
    FILE * fp;
    size_t len = 0;
    char * buff = NULL;
    int result = 0;
    number * nums = malloc(sizeof(number));
    number * curr_num = nums;
    symbol * symbols = malloc(sizeof(symbol));
    symbol * curr_sym = symbols;

    fp = fopen(INPUT_FILE, "r");
    if (fp == NULL) err(EXIT_FAILURE, "open failed");

    int i = 0;
    while (getline(&buff, &len, fp) != -1) {
        int value = 0;
        char *ptr = NULL;
        int start_col = 0;
        int col = 0;

        for (ptr = buff; *ptr != '\0'; ptr++) {
            if (*ptr >= '0' && *ptr <= '9') {
                // Number found
                if (value == 0) start_col = col;
                value = value * 10 + *ptr - '0';
            // Part 1 ----------------------------------------------------------
            // } else if (*ptr != '.' && *ptr != '\n') {
            //     // Symbol found
            //     curr_sym->line = i;
            //     curr_sym->column = col;
            //     curr_sym = new_symbol(curr_sym);
            //     if (value > 0) {
            //         curr_num->value = value;
            //         curr_num->line = i;
            //         curr_num->start_col = start_col;
            //         curr_num->end_col = col - 1;
            //         curr_num = new_number(curr_num);
            //         value = 0;
            //     }
            // Part 1 ----------------------------------------------------------
            // Part 2 ----------------------------------------------------------
            } else if (*ptr == '*') {
                // Symbol found
                curr_sym->line = i;
                curr_sym->column = col;
                curr_sym = new_symbol(curr_sym);
                if (value > 0) {
                    curr_num->value = value;
                    curr_num->line = i;
                    curr_num->start_col = start_col;
                    curr_num->end_col = col - 1;
                    curr_num = new_number(curr_num);
                    value = 0;
                }
            // Part 2 ----------------------------------------------------------
            } else {
                if (value > 0) {
                    curr_num->value = value;
                    curr_num->line = i;
                    curr_num->start_col = start_col;
                    curr_num->end_col = col - 1;
                    curr_num = new_number(curr_num);
                    value = 0;
                }
            }
            col++;
        }
        i ++;
    }

    // Now iterate through the llists to find adyacents
    // result = check_if_adyacent(nums, symbols); // Part 1
    result = check_if_adyacent_2(nums, symbols); // Part 2

    free(buff);
    fclose(fp);
    printf("Result: %d\n", result);
    return 0;
}

// Part 1 ----------------------------------------------------------------------
int check_if_adyacent(number *first_num, symbol *first_sym) {
    int result = 0;
    number * next_num = first_num;
    number * free_num = NULL;
    symbol * next_sym = first_sym;
    symbol * free_sym = NULL;

    for (next_num = first_num; next_num != NULL; next_num = next_num->next) {
        // Check if symbol exists in line -1, 0, 1 between start_col - 1 and end_col + 1
        for (next_sym = first_sym; next_sym != NULL; next_sym = next_sym->next) {
            if (next_sym->line >= next_num->line - 1 && next_sym->line <= next_num->line + 1 &&
                next_sym->column >= next_num->start_col - 1 && next_sym->column <= next_num->end_col + 1) {
                if (next_num->is_adjacent == 0) result += next_num->value;
                next_num->is_adjacent = 1;
            }
        }
    }

    for (next_num = first_num; next_num != NULL;) {
        free_num = next_num;
        next_num = next_num->next;
        free(free_num);
    }

    for (next_sym = first_sym; next_sym != NULL;) {
        free_sym = next_sym;
        next_sym = next_sym->next;
        free(free_sym);
    }

    return result;
}

// Part 1 ----------------------------------------------------------------------

// Part 2 ----------------------------------------------------------------------
int check_if_adyacent_2(number *first_num, symbol *first_sym) {
    int result = 0;
    number * next_num = first_num;
    number * free_num = NULL;
    symbol * next_sym = first_sym;
    symbol * free_sym = NULL;

    for (next_num = first_num; next_num != NULL; next_num = next_num->next) {
        // Check if symbol exists in line -1, 0, 1 between start_col - 1 and end_col + 1
        for (next_sym = first_sym; next_sym != NULL; next_sym = next_sym->next) {
            if (next_sym->line >= next_num->line - 1 && next_sym->line <= next_num->line + 1 &&
                next_sym->column >= next_num->start_col - 1 && next_sym->column <= next_num->end_col + 1) {
                next_num->is_adjacent = 1;
                if (next_sym->n_adj == 0) next_sym->ratio = next_num->value;
                else next_sym->ratio *= next_num->value;
                next_sym->n_adj++;
            }
        }
    }

    for (next_num = first_num; next_num != NULL;) {
        free_num = next_num;
        next_num = next_num->next;
        free(free_num);
    }

    for (next_sym = first_sym; next_sym != NULL;) {
        free_sym = next_sym;
        if (next_sym->n_adj == 2) result += next_sym->ratio;
        next_sym = next_sym->next;
        free(free_sym);
    }

    return result;
}
// Part 2 ----------------------------------------------------------------------
