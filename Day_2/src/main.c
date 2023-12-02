#define _GNU_SOURCE
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024
#define INPUT_FILE "input.txt"

typedef struct {
    const char * str;
    int max;
} color;

color colors[3] = {{"red", 12}, {"green", 13}, {"blue", 14}};


int is_legal_throw(char *line);
int get_min_dice_throw(char *line);

int main(int argc, char const *argv[]) {
    FILE * fp;
    size_t len = 0;
    char * buff = NULL;
    int result = 0;

    fp = fopen(INPUT_FILE, "r");
    if (fp == NULL) err(EXIT_FAILURE, "open failed");

    while (getline(&buff, &len, fp) != -1) {
        // result += is_legal_throw(buff); // Part 1
        result += get_min_dice_throw(buff); // Part 2
    }

    free(buff);
    fclose(fp);
    printf("Result: %d\n", result);
    return 0;
}

// Part 1 ----------------------------------------------------------------------
int is_legal_throw(char *line) {
    // Part 1
    int id = 0;
    int value = 0;
    char *ptr = NULL;
    char *color_ptr = NULL;

    for (ptr = line; *ptr != ':'; ptr++) {
        if (*ptr >= '0' && *ptr <= '9') {
            id = 10 * id + (*ptr - '0');
        }
    }


    char * start_line = ptr;
    for (int i = 0; i < 3; i++) {
        start_line = ptr;
        while((color_ptr = strstr(start_line,colors[i].str)) != NULL) {
            value = 0;
            start_line = color_ptr + 1;
            color_ptr -= 2;
            for (color_ptr; *color_ptr != ' '; color_ptr--) {} // Go back to number start
            for (color_ptr++; *color_ptr != ' '; color_ptr++) {
                value = 10 * value + *color_ptr - '0' ;
            }
            if (value > colors[i].max) return 0;
        }
    }

    return id;
}
// Part 1 ----------------------------------------------------------------------

// Part 2 ----------------------------------------------------------------------
int get_min_dice_throw(char *line) {
    // Part 1
    int value = 0;
    char *color_ptr = NULL;
    color max_colors[3] = {{"red", 0}, {"green", 0}, {"blue", 0}};

    char * start_line = line;
    for (int i = 0; i < 3; i++) {
        start_line = line;
        while((color_ptr = strstr(start_line,max_colors[i].str)) != NULL) {
            value = 0;
            start_line = color_ptr + 1;
            color_ptr -= 2;
            for (color_ptr; *color_ptr != ' '; color_ptr--) {} // Go back to number start
            for (color_ptr++; *color_ptr != ' '; color_ptr++) {
                value = 10 * value + *color_ptr - '0' ;
            }
            if (value > max_colors[i].max) max_colors[i].max = value;
        }
    }

    return max_colors[0].max * max_colors[1].max * max_colors[2].max;
}
// Part 2 ----------------------------------------------------------------------
