#define _GNU_SOURCE
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_NAME_SIZE 4
#define INPUT_FILE "input.txt"
#define TEST_FILE "test.txt"

typedef struct node {
    char name[MAX_NAME_SIZE];
    struct node * right;
    struct node * left;
} node;

node * new_node(char name[MAX_NAME_SIZE]) {
    node * n = malloc(sizeof(node));
    if (n == NULL) exit(EXIT_FAILURE);
    memset(n, 0, sizeof(node));

    strcpy(n->name, name);
    n->right = NULL;
    n->left = NULL;

    return n;
}

int in_list(node * tree[MAX_BUFFER_SIZE], char name[MAX_NAME_SIZE], int index) {
    for (size_t i = 0; i < index; i++){
        if (strcmp(tree[i]->name, name) == 0) return i;
    }
    return -1;
}

long long gcd(long x, long y) {
    if (y == 0) return x;
    return gcd(y, x % y);
}

int main(int argc, char const *argv[]) {
    FILE * fp;
    size_t len = 0;
    char * buff = NULL;
    long long result = 0;
    node * tree[MAX_BUFFER_SIZE];
    char name[MAX_NAME_SIZE];
    char * ptr = NULL;
    int map_size = 0;

    fp = fopen(INPUT_FILE, "r");
    if (fp == NULL) err(EXIT_FAILURE, "open failed");

    // Get instructions
    getline(&buff, &len, fp);
    char instructions[MAX_BUFFER_SIZE];
    strcpy(instructions, buff);
    getline(&buff, &len, fp);

    int tree_size = 0;
    int index, l_index, r_index;
    while (getline(&buff, &len, fp) != -1) {
        strncpy(name, buff, 3);
        if (name[2] == 'A') map_size++;
        index = in_list(tree, name, tree_size);
        if (index < 0) {
            tree[tree_size] = new_node(name);
            index = tree_size;
            tree_size++;
        }

        ptr = buff + 7;
        strncpy(name, ptr, 3);
        l_index = in_list(tree, name, tree_size);
        if (l_index < 0) {
            tree[index]->left = new_node(name);
            tree[tree_size] = tree[index]->left;
            tree_size++;
        } else {
            tree[index]->left = tree[l_index];
        }

        ptr = ptr + 5;
        strncpy(name, ptr, 3);
        r_index = in_list(tree, name, tree_size);
        if (r_index < 0) {
            tree[index]->right = new_node(name);
            tree[tree_size] = tree[index]->right;
            tree_size++;
        } else {
            tree[index]->right = tree[r_index];
        }
    }
    // Part 1 ------------------------------------------------------------------
    // node * curr = NULL;
    // for (size_t i = 0; i < tree_size; i++) {
    //     if(strcmp(tree[i]->name, "AAA") == 0) curr = tree[i];
    // }

    // int inst_index = 0;
    // while (strcmp(curr->name, "ZZZ") != 0) {
    //     if (instructions[inst_index] == 'R') curr = curr->right;
    //     else if (instructions[inst_index] == 'L') curr = curr->left;
    //     inst_index = (inst_index + 1) % (strlen(instructions) - 1);
    //     result++;
    // }
    // Part 1 ------------------------------------------------------------------
    // Part 2 ------------------------------------------------------------------
    node * map[map_size];
    int steps[map_size];
    int map_index = 0;
    for (size_t i = 0; i < tree_size; i++) {
        if(tree[i]->name[2] == 'A') {
            map[map_index] = tree[i];
            steps[map_index] = 0;
            map_index++;
        }
    }
    
    int inst_index = 0;

    for (size_t i = 0; i < map_size; i++) {
        node * curr = map[i];
        inst_index = 0;
        result = 0;
        while (curr->name[2] != 'Z') {
            if (instructions[inst_index] == 'R') curr = curr->right;
            else if (instructions[inst_index] == 'L') curr = curr->left;
            inst_index = (inst_index + 1) % (strlen(instructions) - 1);
            result++;
        }
        steps[i] = result;
    }

    long lcm = steps[0];
    long n1, n2, gcd_val;
    for (size_t i = 1; i < map_size; i++) {
        n1 = lcm;
        n2 = steps[i];
        gcd_val = gcd(n1, n2);
        lcm = (lcm * steps[i]) / gcd_val;
    }

    result = lcm;
    // Part 2 ------------------------------------------------------------------
    
    free(buff);
    for (size_t i = 0; i < tree_size; i++) free(tree[i]);  
    fclose(fp);
    printf("Result: %lld\n", result);
    return 0;
}
