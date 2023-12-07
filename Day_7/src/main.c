#define _GNU_SOURCE
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_ARRAY_SIZE 1024
#define INPUT_FILE "input.txt"
#define TEST_FILE "test.txt"

// Part 1 ----------------------------------------------------------------------
// enum cards {
//     TWO = 0,
//     THREE,
//     FOUR,
//     FIVE,
//     SIX,
//     SEVEN,
//     EIGHT,
//     NINE,
//     T,
//     J,
//     Q,
//     K,
//     A
// };
// Part 1 ----------------------------------------------------------------------
// Part 2 ----------------------------------------------------------------------
enum cards {
    J = 0,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    T,
    Q,
    K,
    A
};
// Part 2 ----------------------------------------------------------------------


typedef struct hand {
    int bet;
    int hash[6]; // First character is type of hand and the rest is the hand
} hand;

typedef struct list {
    hand list[MAX_ARRAY_SIZE];
    int size;
} hand_list;

void get_hand(hand_list * list, char * line);
void get_hand_2(hand_list * list, char * line);

int main(int argc, char const *argv[]) {
    FILE * fp;
    size_t len = 0;
    char * buff = NULL;
    long result = 0;
    hand_list * list = malloc(sizeof(hand_list));
    memset(list, 0, sizeof(hand_list));
    list->size = 0;

    fp = fopen(INPUT_FILE, "r");
    if (fp == NULL) err(EXIT_FAILURE, "open failed");

    while (getline(&buff, &len, fp) != -1) {
        // get_hand(list, buff); // Part 1
        get_hand_2(list, buff); // Part 1
    }

    for (size_t i = 0; i < list->size; i++) {
        result += list->list[i].bet * (i + 1);
    }
    
    free(buff);
    free(list);
    fclose(fp);
    printf("Result: %ld\n", result);
    return 0;
}

// Part 1 ----------------------------------------------------------------------
void get_hand(hand_list * list, char * line) {
    int curr_num = 0, index = 1;
    int num_in_hand = 0, max_rep_in_hand = 0;
    int type;
    char * ptr;
    hand * h = malloc(sizeof(hand));
    memset(h, 0, sizeof(hand));

    for (ptr = line; *ptr != ' '; ptr++) {
        switch (*ptr){
        case '2':
            h->hash[index] = TWO;
            break;
        case '3':
            h->hash[index] = THREE;
            break;
        case '4':
            h->hash[index] = FOUR;
            break;
        case '5':
            h->hash[index] = FIVE;
            break;
        case '6':
            h->hash[index] = SIX;
            break;
        case '7':
            h->hash[index] = SEVEN;
            break;
        case '8':
            h->hash[index] = EIGHT;
            break;
        case '9':
            h->hash[index] = NINE;
            break;
        case 'T':
            h->hash[index] = T;
            break;
        case 'J':
            h->hash[index] = J;
            break;
        case 'Q':
            h->hash[index] = Q;
            break;
        case 'K':
            h->hash[index] = K;
            break;
        case 'A':
            h->hash[index] = A;
            break;
        }
        index++;
    }

    int repetitions[5] = {};
    for (size_t i = 1; i < 6; i++) {
        for (size_t j = 1; j < 6; j++) {
            if (h->hash[i] == h->hash[j] && repetitions[i - 1] >= 0) {
                repetitions[i - 1] += 1;
                if (i != j && repetitions[j - 1] == 0) repetitions[j - 1] = -1;
            }
        }
    }

    for (size_t i = 0; i < 5; i++) {
        if (repetitions[i] > 0) num_in_hand++;
        if (repetitions[i] > max_rep_in_hand) max_rep_in_hand = repetitions[i];
    }
    
    if (num_in_hand == 1) type = 6;
    else if (num_in_hand == 2 && max_rep_in_hand == 4) type = 5;
    else if (num_in_hand == 2 && max_rep_in_hand == 3) type = 4;
    else if (num_in_hand == 3 && max_rep_in_hand == 3) type = 3;
    else if (num_in_hand == 3 && max_rep_in_hand == 2) type = 2;
    else if (num_in_hand == 4 && max_rep_in_hand == 2) type = 1;
    else if (num_in_hand == 5) type = 0;

    h->hash[0] = type;

    ptr++;

    for (ptr; *ptr != '\0'; ptr++) {
        if (*ptr >= '0' && *ptr <= '9') {
            curr_num = 10 * curr_num + *ptr - '0';
        }
    }

    h->bet = curr_num;
    // Check in list
    if (list->size != 0) {
        for (int i = 0; i < list->size; i++) {
            for (int j = 0; j < 6; j++) {
                if (h->hash[j] < list->list[i].hash[j]) {
                    // Shift all elements
                    for (int k = list->size - 1; k >= i; k--) {
                        list->list[k + 1] = list->list[k];
                    }
                    list->list[i].bet = h->bet;
                    memcpy(list->list[i].hash, h->hash, sizeof(h->hash));
                    list->size++;
                    free(h);
                    return;
                } else if (h->hash[j] > list->list[i].hash[j]) break;
            }
        }
    }

    list->list[list->size].bet = h->bet;
    memcpy(list->list[list->size].hash, h->hash, sizeof(h->hash));
    list->size++;

    free(h);

    return;
}

// Part 1 ----------------------------------------------------------------------

// Part 2 ----------------------------------------------------------------------
void get_hand_2(hand_list * list, char * line) {
    int curr_num = 0, index = 1;
    int num_in_hand = 0, max_rep_in_hand = 0;
    int type;
    char * ptr;
    hand * h = malloc(sizeof(hand));
    memset(h, 0, sizeof(hand));

    for (ptr = line; *ptr != ' '; ptr++) {
        switch (*ptr){
        case '2':
            h->hash[index] = TWO;
            break;
        case '3':
            h->hash[index] = THREE;
            break;
        case '4':
            h->hash[index] = FOUR;
            break;
        case '5':
            h->hash[index] = FIVE;
            break;
        case '6':
            h->hash[index] = SIX;
            break;
        case '7':
            h->hash[index] = SEVEN;
            break;
        case '8':
            h->hash[index] = EIGHT;
            break;
        case '9':
            h->hash[index] = NINE;
            break;
        case 'T':
            h->hash[index] = T;
            break;
        case 'J':
            h->hash[index] = J;
            break;
        case 'Q':
            h->hash[index] = Q;
            break;
        case 'K':
            h->hash[index] = K;
            break;
        case 'A':
            h->hash[index] = A;
            break;
        }
        index++;
    }

    int repetitions[5] = {};
    int jokers = 0;
    for (size_t i = 1; i < 6; i++) {
        if (h->hash[i] == J) jokers++;
        
        for (size_t j = 1; j < 6; j++) {
            if (h->hash[i] == h->hash[j] && repetitions[i - 1] >= 0) {
                repetitions[i - 1] += 1;
                if (i != j && repetitions[j - 1] == 0) repetitions[j - 1] = -1;
            }
        }
    }

    for (size_t i = 0; i < 5; i++) {
        if (repetitions[i] > 0) num_in_hand++;
        if (repetitions[i] + jokers > max_rep_in_hand && h->hash[i + 1] != J) max_rep_in_hand = repetitions[i]  + jokers;
    }

    if (jokers > 0 && num_in_hand > 1) num_in_hand--;

    if (num_in_hand == 1) type = 6;
    else if (num_in_hand == 2 && max_rep_in_hand == 4) type = 5;
    else if (num_in_hand == 2 && max_rep_in_hand == 3) type = 4;
    else if (num_in_hand == 3 && max_rep_in_hand == 3) type = 3;
    else if (num_in_hand == 3 && max_rep_in_hand == 2) type = 2;
    else if (num_in_hand == 4 && max_rep_in_hand == 2) type = 1;
    else if (num_in_hand == 5) type = 0;

    h->hash[0] = type;

    ptr++;

    for (ptr; *ptr != '\0'; ptr++) {
        if (*ptr >= '0' && *ptr <= '9') {
            curr_num = 10 * curr_num + *ptr - '0';
        }
    }

    h->bet = curr_num;
    // Check in list
    if (list->size != 0) {
        for (int i = 0; i < list->size; i++) {
            for (int j = 0; j < 6; j++) {
                if (h->hash[j] < list->list[i].hash[j]) {
                    // Shift all elements
                    for (int k = list->size - 1; k >= i; k--) {
                        list->list[k + 1] = list->list[k];
                    }
                    list->list[i].bet = h->bet;
                    memcpy(list->list[i].hash, h->hash, sizeof(h->hash));
                    list->size++;
                    free(h);
                    return;
                } else if (h->hash[j] > list->list[i].hash[j]) break;
            }
        }
    }

    list->list[list->size].bet = h->bet;
    memcpy(list->list[list->size].hash, h->hash, sizeof(h->hash));
    list->size++;

    free(h);

    return;
}

// Part 2 ----------------------------------------------------------------------
