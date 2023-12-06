#define _GNU_SOURCE
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_SEEDS 256
#define INPUT_FILE "input.txt"
#define TEST_FILE "test.txt"

typedef struct {
    long long list[MAX_SEEDS];
    long long range[MAX_SEEDS];
    int has_changed[MAX_SEEDS];
    int size;
} seeds;

int new_menu = 0;

seeds * check_seeds(char *line);
seeds * check_seeds_2(char *line);
int check_line(char *line, seeds * list);

int main(int argc, char const *argv[]) {
    FILE * fp;
    size_t len = 0;
    char * buff = NULL;
    long long result = 0;

    fp = fopen(INPUT_FILE, "r");
    if (fp == NULL) err(EXIT_FAILURE, "open failed");

    // Get firt line to save seeds
    getline(&buff, &len, fp);
    // seeds * seed_list = check_seeds(buff); // Part 1
    seeds * seed_list = check_seeds_2(buff); // Part 2

    while (getline(&buff, &len, fp) != -1) {
        check_line(buff, seed_list); // Part 1
    }

    for (size_t i = 0; i < seed_list->size; i++) {
        if (seed_list->list[i] < result || result == 0) {
            result = seed_list->list[i];
        }
    }
    
    free(buff);
    free(seed_list);
    fclose(fp);
    printf("Result: %lld\n", result);
    return 0;
}

// Part 1 ----------------------------------------------------------------------
seeds * check_seeds(char *line) {
    long long curr_num = 0;
    seeds * list = malloc(sizeof(seeds));
    memset(list, 0, sizeof(seeds));

    for (char * ptr = line; *ptr != '\0'; ptr++) {
        if (*ptr >= '0' && *ptr <= '9') {
            curr_num = 10 * curr_num + *ptr - '0';
        } else if (curr_num > 0) {
            list->list[list->size] = curr_num;
            list->size++;
            curr_num = 0;
        }
    }

    return list;
}

int check_line(char *line, seeds * list) {
    long long source = -1, destination = -1, length = -1;
    long long curr_num = 0;
    int num_started = 0;

    if (strlen(line) <= 1) new_menu = 2;
    else new_menu --;

    if (new_menu >= 0) {
        for (size_t i = 0; i < list->size; i++) list->has_changed[i] = 0;
        new_menu --;
        return 0;
    }

    for (char * ptr = line; *ptr != '\0'; ptr++) {
        if (*ptr >= '0' && *ptr <= '9') {
            curr_num = 10 * curr_num + *ptr - '0';
            num_started = 1;
        } else if (num_started) {
            if (destination == -1) destination = curr_num;
            else if (source == -1) source = curr_num;
            else length = curr_num;
            curr_num = 0;
            num_started = 0;
        }
    }

    long long new_value = -1;
    long long cut_place = 0; 

    for (size_t i = 0; i < list->size; i++) {
        if (!list->has_changed[i] && list->list[i] >= source && 
            list->list[i] <= source + length) 
        {
            // Begining inside
            // Change list value and check if some part of it is inside to separate them
            // 57 + 13 range = 53 + 8 range, cut in range - 9
            // Cut in 61
            new_value = list->list[i] - source + destination;
            cut_place = (source + length) - (list->range[i] + list->list[i]);
            if (cut_place < 0) {
                // We have to separate them
                list->list[list->size] = list->list[i] + list->range[i] + cut_place;
                list->range[list->size] = -cut_place - 1;
                // list->has_changed[list->size] = 2;
                list->size++;
                list->range[i] = list->range[i] + cut_place + 1;
            }

            list->list[i] = new_value;
            list->has_changed[i] = 1;
        } else if (list->list[i] < source && list->list[i] + list->range[i] >= source) {
            // Check if some part is inside
            // 57 + 13 range -> 11 + 42 range Separate in 2
            // 5 + 10 range -> 7 + 5 range Separate in 2
            // 5 + 4 range -> 7 + 5 range Separate in 2
            // 15 + 10 range -> 7 + 5 range Do nothing
            // We have to separate them
            list->list[list->size] = list->list[i] + source - list->list[i];
            list->range[list->size] = list->range[i] - (source - list->list[i]);
            list->size++;
            list->range[i] = source - list->list[i]; 
        }
    }

    return 0;
}
// Part 1 ----------------------------------------------------------------------

// Part 2 ----------------------------------------------------------------------
seeds * check_seeds_2(char *line) {
    long long curr_num = 0;
    int set_range = 0;
    seeds * list = malloc(sizeof(seeds));
    memset(list, 0, sizeof(seeds));

    for (char * ptr = line; *ptr != '\0'; ptr++) {
        if (*ptr >= '0' && *ptr <= '9') {
            curr_num = 10 * curr_num + *ptr - '0';
        } else if (curr_num > 0) {
            if (set_range) {
                list->range[list->size] = curr_num;
                set_range = 0;
                list->size++;
            } else {
                list->list[list->size] = curr_num;
                set_range = 1;
            }
            curr_num = 0;
        }
    }

    return list;
}
// Part 2 ----------------------------------------------------------------------
