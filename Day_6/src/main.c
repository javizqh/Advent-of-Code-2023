#define _GNU_SOURCE
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_TIMES 4
#define SPEED_INCREASE 1
#define INPUT_FILE "input.txt"
#define TEST_FILE "test.txt"

void get_data(char *line, long * array);
long get_data_2(char *line);
long get_best_combinations(long time, long dst);

int main(int argc, char const *argv[]) {
    FILE * fp;
    size_t len = 0;
    char * buff = NULL;
    long result = 1;

    fp = fopen(INPUT_FILE, "r");
    if (fp == NULL) err(EXIT_FAILURE, "open failed");

    // Part 1 ------------------------------------------------------------------
    // getline(&buff, &len, fp);
    // long times[MAX_TIMES];
    // get_data(buff, times);
    // getline(&buff, &len, fp);
    // long dsts[MAX_TIMES];
    // get_data(buff, dsts);

    // for (size_t i = 0; i < MAX_TIMES; i++) {
    //     result *= get_best_combinations(times[i], dsts[i]);
    // }
    // Part 1 ------------------------------------------------------------------
    // Part 2 ------------------------------------------------------------------
    getline(&buff, &len, fp);
    long time = get_data_2(buff);
    getline(&buff, &len, fp);
    long dst = get_data_2(buff);
    printf("time: %ld, dist: %ld\n", time, dst);
    result = get_best_combinations(time, dst);
    // Part 2 ------------------------------------------------------------------
    
    free(buff);
    fclose(fp);
    printf("Result: %ld\n", result);
    return 0;
}

// Part 1 ----------------------------------------------------------------------
void get_data(char *line, long * array) {
    long curr_num = 0, index = 0;

    for (char * ptr = line; *ptr != '\0'; ptr++) {
        if (*ptr >= '0' && *ptr <= '9') {
            curr_num = 10 * curr_num + *ptr - '0';
        } else if (curr_num > 0) {
            array[index] = curr_num;
            index++;
            curr_num = 0;
        }
    }

    if (curr_num > 0) {
        array[index] = curr_num;
        index++;
    }

    return;
}

long get_best_combinations(long time, long dst) {
    long n_good_combinations = 0;
    long speed = 0;

    for (long i = 0; i <= time; i++) {
        speed = i * SPEED_INCREASE;
        if ((time - i) * speed > dst) n_good_combinations++;
    }
    
    return n_good_combinations;
}
// Part 1 ----------------------------------------------------------------------

// Part 2 ----------------------------------------------------------------------
long get_data_2(char *line) {
    long curr_num = 0;

    for (char * ptr = line; *ptr != '\0'; ptr++) {
        if (*ptr >= '0' && *ptr <= '9') {
            curr_num = 10 * curr_num + *ptr - '0';
        }
    }

    return curr_num;
}

// Part 2 ----------------------------------------------------------------------
