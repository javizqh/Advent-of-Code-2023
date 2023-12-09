#define _GNU_SOURCE
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_SEQUENCE_SIZE 32
#define INPUT_FILE "input.txt"
#define TEST_FILE "test.txt"

typedef struct extrapolate_info {
    long seq[MAX_SEQUENCE_SIZE];
    int size;
    long result;
} extrapolate;

int read_line(char * line);
extrapolate get_difference(extrapolate a);
extrapolate get_difference_2(extrapolate a);

int main(int argc, char const *argv[]) {
    FILE * fp;
    size_t len = 0;
    char * buff = NULL;
    long long result = 0;

    fp = fopen(INPUT_FILE, "r");
    if (fp == NULL) err(EXIT_FAILURE, "open failed");

    while (getline(&buff, &len, fp) != -1) {
        result += read_line(buff);
    }
    
    free(buff);
    fclose(fp);
    printf("Result: %lld\n", result);
    return 0;
}

int read_line(char * line) {
    extrapolate sequence;
    sequence.size = 0;
    sequence.result = 0;
    long curr_num = 0;
    int num_started = 0;
    int sign = 1;

    for (char *ptr = line; *ptr != '\0'; ptr++) {
        if (*ptr >= '0' && *ptr <= '9') {
            curr_num = 10 * curr_num + *ptr - '0';
            num_started = 1;
        } else if (*ptr == '-') {
            sign = -1;
        } else if (num_started) {
            sequence.seq[sequence.size] = curr_num * sign;
            sequence.size++;
            curr_num = 0;
            num_started = 0;
            sign = 1;
        }
    }

    if (num_started) {
        sequence.seq[sequence.size] = curr_num;
        sequence.size++;
    }
    
    // return get_difference(sequence).result; // Part 1
    return get_difference_2(sequence).result; // Part 2
}

// Part 1 ----------------------------------------------------------------------
extrapolate get_difference(extrapolate a) {
    extrapolate diff;
    diff.size = a.size - 1;
    diff.result = 0;
    int call_next = 0;

    for (size_t i = 0; i < diff.size; i++) {
        diff.seq[i] = a.seq[i + 1] - a.seq[i];
        if (diff.seq[i] != 0) call_next = 1;
    }
    
    if (call_next) {
        extrapolate result = get_difference(diff);
        diff.result = result.result + a.seq[a.size - 1];
    } else {
        diff.result = diff.seq[diff.size - 1] + a.seq[diff.size];
    }

    return diff;
}
// Part 1 ----------------------------------------------------------------------
// Part 2 ----------------------------------------------------------------------
extrapolate get_difference_2(extrapolate a) {
    extrapolate diff;
    diff.size = a.size - 1;
    diff.result = 0;
    int call_next = 0;

    for (size_t i = 0; i < diff.size; i++) {
        diff.seq[i] = a.seq[i + 1] - a.seq[i];
        if (diff.seq[i] != 0) call_next = 1;
    }
    
    if (call_next) {
        extrapolate result = get_difference_2(diff);
        diff.result = a.seq[0] - result.result;
    } else {
        diff.result = a.seq[0];
    }

    return diff;
}
// Part 2 ----------------------------------------------------------------------
