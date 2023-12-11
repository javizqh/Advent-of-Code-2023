#define _GNU_SOURCE
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_GALAXIES 512
#define MAX_COLUMNS 256
// #define GALAXY_EXPANSION 1 // Part 1
#define GALAXY_EXPANSION 1000000 - 1 // Part 2
#define INPUT_FILE "input.txt"
#define TEST_FILE "test.txt"

typedef struct galaxy {
    unsigned long long x, y;
} galaxy;

typedef struct galaxy_map {
    galaxy * map[MAX_GALAXIES];
    int size;
} galaxy_map_t;

galaxy *new_galaxy(int x, int y) {
    galaxy * n = malloc(sizeof(galaxy));
    memset(n, 0, sizeof(galaxy));

    n->x = x;
    n->y = y;

    return n;
}

unsigned long long my_abs(unsigned long long x1, unsigned long long x2) {
    if (x2 > x1) return x2 - x1;
    else return x1 - x2;
}

unsigned long long get_Manhattan_dst(galaxy * g1, galaxy * g2) {
    return my_abs(g1->x, g2->x) + my_abs(g1->y, g2->y);
}

int main(int argc, char const *argv[]) {
    FILE * fp;
    size_t len = 0;
    char * buff = NULL;
    unsigned long long result = 0;
    galaxy_map_t * galaxy_map = malloc(sizeof(galaxy_map_t));
    memset(galaxy_map, 0, sizeof(galaxy_map_t));
    int col_not_found[MAX_COLUMNS] = {0}, row_not_found[MAX_COLUMNS] = {0};
    int x = 0, y = 0;

    fp = fopen(INPUT_FILE, "r");
    if (fp == NULL) err(EXIT_FAILURE, "open failed");

    while (getline(&buff, &len, fp) != -1) {
        // Iterate and store galaxies and also store x, y where there is none
        row_not_found[y] = y;
        x = 0;

        for (char *ptr = buff; *ptr != '\0'; ptr++) {
            if (*ptr == '\n') break;
            if (col_not_found[x] == 0) col_not_found[x] = x;
            if (*ptr == '#') {
                col_not_found[x] = -1;
                row_not_found[y] = -1;
                galaxy_map->map[galaxy_map->size] = new_galaxy(x, y);
                galaxy_map->size++;
            }
            x++;
        }
        y++;
    }

    // Iterate again and set the real values to the galaxies
    unsigned long long increment = 0;
    int has_expanded = 0;
    for (size_t i = 0; i < x; i++) {
        has_expanded = 0;
        for (size_t j = 0; j < galaxy_map->size; j++) {
            if (col_not_found[i] <= 0 || (i == 0 && col_not_found[i] == 0)) break;
            if (galaxy_map->map[j]->x > col_not_found[i] + increment) {
                galaxy_map->map[j]->x += GALAXY_EXPANSION;
                has_expanded = 1;
            }
        }
        if (has_expanded) increment += GALAXY_EXPANSION;
    }

    increment = 0;
    for (size_t i = 0; i < y; i++) {
        has_expanded = 0;
        for (size_t j = 0; j < galaxy_map->size; j++) {
            if (row_not_found[i] <= 0 || (i == 0 && row_not_found[i] == 0)) break;
            if (galaxy_map->map[j]->y > row_not_found[i] + increment) {
                galaxy_map->map[j]->y += GALAXY_EXPANSION;
                has_expanded = 1;
            }
        }
        if (has_expanded) increment += GALAXY_EXPANSION;
    }

    // Now iterate i and j = i; and get the Manhattan distance between them
    for (size_t i = 0; i < galaxy_map->size; i++) {
        for (size_t j = i + 1; j < galaxy_map->size; j++) {
            result += get_Manhattan_dst(galaxy_map->map[i], galaxy_map->map[j]);
        }
        free(galaxy_map->map[i]);
    }

    free(galaxy_map);
    free(buff);
    fclose(fp);
    printf("Result: %lld\n", result);
    return 0;
}

// Part 1 ----------------------------------------------------------------------
// Part 1 ----------------------------------------------------------------------
// Part 2 ----------------------------------------------------------------------
// Part 2 ----------------------------------------------------------------------
