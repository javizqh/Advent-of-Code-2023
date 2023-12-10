#define _GNU_SOURCE
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_MAP_LENGTH 140 // 140 input
#define MAX_MAP_HEIGHT 140 // 140 input
#define INPUT_FILE "input.txt"
#define TEST_FILE "test.txt"

enum directions {
    NORTH = 0,
    SOUTH,
    WEST,
    EAST
};

enum moves {
    NOTHING = 0,
    VERTICAL,
    HORIZONTAL,
    NORTH_TO_WEST,
    NORTH_TO_EAST,
    SOUTH_TO_WEST,
    SOUTH_TO_EAST,
    START
};

typedef struct node {
    int type;
    struct node *north, *south, *east, *west;
    int is_path;
} node;

typedef struct maze {
    node *map[MAX_MAP_LENGTH][MAX_MAP_HEIGHT];
    node *start;
} maze;

node *new_node(int type) {
    node * n = malloc(sizeof(node));
    memset(n, 0, sizeof(node));

    switch (type) {
    case '.':
        n->type = NOTHING;
        break;
    case '|':
        n->type = VERTICAL;
        break;
    case '-':
        n->type = HORIZONTAL;
        break;
    case 'F':
        n->type = SOUTH_TO_EAST;
        break;
    case 'J':
        n->type = NORTH_TO_WEST;
        break;
    case 'L':
        n->type = NORTH_TO_EAST;
        break;
    case '7':
        n->type = SOUTH_TO_WEST;
        break;
    case 'S':
        n->type = START;
        break;
    }
    n->north = NULL;
    n->south = NULL;
    n->west = NULL;
    n->east = NULL;

    return n;
}

int find_path(maze * main_maze);
int find_area(maze * main_maze);

int main(int argc, char const *argv[]) {
    FILE * fp;
    size_t len = 0;
    char * buff = NULL;
    long long result = 0;
    maze * main_maze = malloc(sizeof(maze));
    int i = 0, j = 0;
    char *ptr = NULL;

    fp = fopen(INPUT_FILE, "r");
    if (fp == NULL) err(EXIT_FAILURE, "open failed");

    while (getline(&buff, &len, fp) != -1) {
        if (strlen(buff) < 2) break;
        j = 0;
        for (ptr = buff; *ptr != '\0'; ptr++) {
            if (*ptr == '\n') break;
            main_maze->map[i][j] = new_node(*ptr);

            if (main_maze->map[i][j]->type == START) { 
                main_maze->start = main_maze->map[i][j];
            }

            if (i >= 1) {
                main_maze->map[i][j]->north = main_maze->map[i - 1][j];
                if (main_maze->map[i - 1][j]) {
                    main_maze->map[i - 1][j]->south = main_maze->map[i][j];
                }
            } 
            if (j >= 1) {
                main_maze->map[i][j]->west = main_maze->map[i][j - 1];
                if (main_maze->map[i][j - 1]) {
                    main_maze->map[i][j - 1]->east = main_maze->map[i][j];
                }
            }
            j++;
        }
        i++;
    }

    // result = find_path(main_maze); // Part 1
    result = find_area(main_maze); // Part 2

    for (size_t x = 0; x < i; x++) {
        for (size_t y = 0; y < j; y++) {
            free(main_maze->map[x][y]);
        }
    }

    free(main_maze);
    
    free(buff);
    fclose(fp);
    printf("Result: %lld\n", result);
    return 0;
}

// Part 1 ----------------------------------------------------------------------
int decide_direction(int type, int last_direction) {
    switch (type) {
    case VERTICAL:
    case HORIZONTAL:
        return last_direction;
    case NORTH_TO_WEST:
        if (last_direction == SOUTH) return WEST;
        if (last_direction == EAST) return NORTH;
    case NORTH_TO_EAST:
        if (last_direction == SOUTH) return EAST;
        if (last_direction == WEST) return NORTH;
    case SOUTH_TO_WEST:
        if (last_direction == NORTH) return WEST;
        if (last_direction == EAST) return SOUTH;
    case SOUTH_TO_EAST:
        if (last_direction == NORTH) return EAST;
        if (last_direction == WEST) return SOUTH;
    }
    return -1;
}

node * move(node * original, int direction) {
    if (direction == NORTH) return original->north;
    if (direction == SOUTH) return original->south;
    if (direction == WEST) return original->west;
    if (direction == EAST) return original->east;
    return NULL;
}

int find_path(maze * main_maze) {
    int distance = 0;
    int last_direction_1 = -1, last_direction_2 = 0;
    node * path_1 = main_maze->start, * path_2 = NULL;
    
    // Find the first step for the 2 paths
    if (main_maze->start->north->type != NOTHING && main_maze->start->north->type != HORIZONTAL) {
        if (last_direction_1 == -1) last_direction_1 = NORTH;
        else last_direction_2 = NORTH;
    }
    if (main_maze->start->south->type != NOTHING && main_maze->start->south->type != HORIZONTAL) {
        if (last_direction_1 == -1) last_direction_1 = SOUTH;
        else last_direction_2 = SOUTH;
    } 
    if (main_maze->start->west->type != NOTHING) {
        if (main_maze->start->west->type == HORIZONTAL ||
            main_maze->start->west->type == NORTH_TO_EAST ||
            main_maze->start->west->type == SOUTH_TO_EAST) 
        {
            if (last_direction_1 == -1) last_direction_1 = WEST;
            else last_direction_2 = WEST;
        }
    } 
    if (main_maze->start->east->type != NOTHING) {
        if (main_maze->start->east->type == HORIZONTAL ||
            main_maze->start->east->type == NORTH_TO_WEST ||
            main_maze->start->east->type == SOUTH_TO_WEST) 
        {
            if (last_direction_1 == -1) last_direction_1 = EAST;
            else last_direction_2 = EAST;
        }
    }  

    // Start from start and go with the 2 paths, when node is equal then return distance.
    while (path_1 != path_2) {
        if (distance == 0) {
            path_1 = move(main_maze->start, last_direction_1);
            path_2 = move(main_maze->start, last_direction_2);
        } else {
            last_direction_1 = decide_direction(path_1->type, last_direction_1);
            last_direction_2 = decide_direction(path_2->type, last_direction_2);

            path_1 = move(path_1, last_direction_1);
            path_2 = move(path_2, last_direction_2);
        }
        distance++;
    }
    
    return distance;
}
// Part 1 ----------------------------------------------------------------------
// Part 2 ----------------------------------------------------------------------
struct big_maze{
    int map[MAX_MAP_LENGTH * 3][MAX_MAP_HEIGHT * 3];
};

struct big_maze * recursive(struct big_maze *maze, int x, int y) {
    maze->map[x][y] = 1;
    if (x > 0 && maze->map[x - 1][y] == 0) maze = recursive(maze, x - 1, y);
    if (x < MAX_MAP_LENGTH * 3 - 1 && maze->map[x + 1][y] == 0) maze = recursive(maze, x + 1, y);
    if (y > 0 && maze->map[x][y - 1] == 0) maze = recursive(maze, x, y - 1);
    if (y < MAX_MAP_HEIGHT * 3 - 1 && maze->map[x][y + 1] == 0) maze = recursive(maze, x, y + 1);

    return maze;
}

int find_area(maze * main_maze) {
    // Generate 3x3 of each node, then create map and use bfs to paint all of the exterior
    // Pipe V: [(0,1,0),(0,1,0),(0,1,0)]
    // Pipe H: [(0,0,0),(1,1,1),(0,0,0)]
    // Pipe L: [(0,1,0),(0,1,1),(0,0,0)]
    // Pipe J: [(0,1,0),(1,1,0),(0,0,0)]
    // Pipe 7: [(0,0,0),(1,1,0),(0,1,0)]
    // Pipe F: [(0,0,0),(0,1,1),(0,1,0)]
    // Pipe S: [(0,1,0),(1,1,1),(0,1,0)]
    // Then check 3x3 squares that are empty
    int distance = 0;
    struct big_maze *maze = malloc(sizeof(struct big_maze));
    memset(maze, 0, sizeof(struct big_maze));

    int last_direction_1 = -1;
    node * path_1 = main_maze->start;

    
    // Find the first step for the 2 paths
    if (main_maze->start->north && main_maze->start->north->type != NOTHING && main_maze->start->north->type != HORIZONTAL) {
        if (last_direction_1 == -1) last_direction_1 = NORTH;
    }
    if (main_maze->start->south->type != NOTHING && main_maze->start->south->type != HORIZONTAL) {
        if (last_direction_1 == -1) last_direction_1 = SOUTH;
    } 
    if (main_maze->start->west->type != NOTHING) {
        if (main_maze->start->west->type == HORIZONTAL ||
            main_maze->start->west->type == NORTH_TO_EAST ||
            main_maze->start->west->type == SOUTH_TO_EAST) 
        {
            if (last_direction_1 == -1) last_direction_1 = WEST;
        }
    } 
    if (main_maze->start->east->type != NOTHING) {
        if (main_maze->start->east->type == HORIZONTAL ||
            main_maze->start->east->type == NORTH_TO_WEST ||
            main_maze->start->east->type == SOUTH_TO_WEST) 
        {
            if (last_direction_1 == -1) last_direction_1 = EAST;
        }
    }  

    // Start from start and go with the 2 paths, when node is equal then return distance.
    path_1 = move(main_maze->start, last_direction_1);
    path_1->is_path = 1;

    while (path_1 != main_maze->start) {
        last_direction_1 = decide_direction(path_1->type, last_direction_1);
        path_1 = move(path_1, last_direction_1);
        path_1->is_path = 1;
    }


    for (int i = 0; i < MAX_MAP_LENGTH; i++) {
        for (int j = 0; j < MAX_MAP_HEIGHT; j++) {
            switch (main_maze->map[i][j]->type) {
            case VERTICAL:
                if(main_maze->map[i][j]->is_path) {
                    maze->map[i * 3][j * 3 + 1] = 9;
                    maze->map[i * 3 + 1][j * 3 + 1] = 9;
                    maze->map[i * 3 + 2][j * 3 + 1] = 9;
                }
                break;
            case HORIZONTAL:
                if(main_maze->map[i][j]->is_path) {
                    maze->map[i * 3 + 1][j * 3] = 9;
                    maze->map[i * 3 + 1][j * 3 + 1] = 9;
                    maze->map[i * 3 + 1][j * 3 + 2] = 9;
                }
                break;
            case SOUTH_TO_EAST:
                // L
                if(main_maze->map[i][j]->is_path) {
                    maze->map[i * 3 + 1][j * 3 + 1] = 9;
                    maze->map[i * 3 + 1][j * 3 + 2] = 9;
                    maze->map[i * 3 + 2][j * 3 + 1] = 9;
                }
                break;
            case NORTH_TO_WEST:
                // 7
                if(main_maze->map[i][j]->is_path) {
                    maze->map[i * 3][j * 3 + 1] = 9;
                    maze->map[i * 3 + 1][j * 3] = 9;
                    maze->map[i * 3 + 1][j * 3 + 1] = 9;
                }
                break;
            case NORTH_TO_EAST:
                if(main_maze->map[i][j]->is_path) {
                    maze->map[i * 3][j * 3 + 1] = 9;
                    maze->map[i * 3 + 1][j * 3 + 1] = 9;
                    maze->map[i * 3 + 1][j * 3 + 2] = 9;
                }
                break;
            case SOUTH_TO_WEST:
                if(main_maze->map[i][j]->is_path) {
                    maze->map[i * 3 + 1][j * 3 + 1] = 9;
                    maze->map[i * 3 + 1][j * 3] = 9;
                    maze->map[i * 3 + 2][j * 3 + 1] = 9;
                }
                break;
            case START:
                maze->map[i * 3][j * 3 + 1] = 9;
                maze->map[i * 3 + 1][j * 3] = 9;
                maze->map[i * 3 + 1][j * 3 + 2] = 9;
                maze->map[i * 3 + 2][j * 3 + 1] = 9;
                break;
            }
        } 
    }
    
    // Start at 0,0 then put 1 in that one, then go to valid nexts and do the same until no more valids are found
    maze = recursive(maze, 0, 0);
    // Print map
    // for (size_t i = 0; i < MAX_MAP_LENGTH * 3; i++) {
    //     for (size_t j = 0; j < MAX_MAP_HEIGHT * 3; j++) {
    //         if (maze->map[i][j] != 0) printf("%d", maze->map[i][j]);
    //         else printf(".");
    //     }
    //     printf("\n");
    // }

    for (size_t i = 0; i < MAX_MAP_LENGTH; i++) {
        for (size_t j = 0; j < MAX_MAP_HEIGHT; j++) {
            if (maze->map[i * 3][j * 3] == 0 && maze->map[i * 3 + 1][j * 3] == 0 && maze->map[i * 3 + 2][j * 3] == 0 &&
                maze->map[i * 3][j * 3 + 1] == 0 && maze->map[i * 3 + 1][j * 3 + 1] == 0 && maze->map[i * 3 + 2][j * 3 + 1] == 0 &&
                maze->map[i * 3][j * 3 + 2] == 0 && maze->map[i * 3 + 1][j * 3 + 2] == 0 && maze->map[i * 3 + 2][j * 3 + 2] == 0)
            {
                distance++;
            }
        }
    }

    free(maze);
    return distance;
}
// Part 2 ----------------------------------------------------------------------
