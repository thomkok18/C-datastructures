/*H**********************************************************************
* FILENAME: main.c
*
* DESCRIPTION:
*   Sort numbers using several command-line options.
*
* PUBLIC FUNCTIONS:
*   void ascending_order( l )
*   void descending_order( l )
*   void pair_wise_order( l )
*   void remove_odd( l )
*   void zip_alternating_order( l )
*   void insertion_sort( l, cfg )
*   int parse_options( cfg, argc, argv[] )
*   int main( argc, argv[] )
*
* AUTHOR: Thom Kok (Student nr: 15316491)
*
* START DATE: 09-11-2023
*
*H*/

#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#define BUF_SIZE 1024

char buf[BUF_SIZE];

typedef struct node {
    struct list *list;
    struct node *next;
    struct node *prev;
    int num;
} node;

typedef struct list {
    struct node *head;
    struct node *tail;
    size_t length;
} list;

struct config {
    /* Set to 1 if -d is specified, 0 otherwise. */
    int descending_order;

    // Set to 1 if -c is specified, 0 otherwise.
    int combine;

    // Set to 1 if -o is specified, 0 otherwise.
    int remove_odd;

    /* Set to 1 if -z is specified, 0 otherwise. */
    int zip_alternating;
};

/**
 * Order list in ascending order.
 *
 * @param l the list with nodes containing numbers.
 */
void ascending_order(struct list *l) {
    for (size_t l_i = 0; l_i < l->length; l_i++) {
        struct node *n = list_get_ith(l, l_i);

        if (n == NULL) break;

        for (size_t n_i = l_i; n_i--;) {
            if (n->prev == NULL) continue;

            struct node *prev_n = list_get_ith(l, n_i);

            if (n->num < prev_n->num) {
                list_unlink_node(l, n);
                list_insert_before(l, n, prev_n);
            }
        }
    }
}

/**
 * Order list in descending order.
 *
 * @param l the list with nodes containing numbers.
 */
void descending_order(struct list *l) {
    for (size_t l_i = 0; l_i < l->length; l_i++) {
        struct node *n = list_get_ith(l, l_i);

        if (n == NULL) break;

        for (size_t n_i = l_i; n_i--;) {
            if (n->prev == NULL) continue;

            struct node *prev_n = list_get_ith(l, n_i);

            if (n->num > prev_n->num) {
                list_unlink_node(l, n);
                list_insert_before(l, n, prev_n);
            }
        }
    }
}

/**
 * Order list pair-wise.
 *
 * @param l the list with nodes containing numbers.
 */
void pair_wise_order(struct list *l) {
    fprintf(stderr, "\n%p\n", (void *) l);
}

/**
 * Remove odd numbers from list.
 *
 * @param l the list with nodes containing numbers.
 */
void remove_odd(struct list *l) {
    fprintf(stderr, "\n%p\n", (void *) l);
}

/**
 * Split list into two and alternating between each number from both lists.
 *
 * @param l the list with nodes containing numbers.
 */
void zip_alternating_order(struct list *l) {
    fprintf(stderr, "\n%p\n", (void *) l);
}

/**
 * Sort list based on config.
 *
 * @param l the list.
 * @param cfg
 */
void insertion_sort(struct list *l, struct config *cfg) {
    if (l == NULL || cfg == NULL) return;

    if (cfg->descending_order) {
        descending_order(l);
    } else {
        ascending_order(l);
    }

    if (cfg->combine) {
        pair_wise_order(l);
    }

    if (cfg->remove_odd) {
        remove_odd(l);
    }

    if (cfg->zip_alternating) {
        zip_alternating_order(l);
    }
}

/**
 * Decide which parse option to use based on the chosen option.
 *
 * @param cfg config containing the sorting options.
 * @param argc count.
 * @param argv array.
 * @return
 */
int parse_options(struct config *cfg, int argc, char *argv[]) {
    memset(cfg, 0, sizeof(struct config));

    int c;

    while ((c = getopt(argc, argv, "dcoz")) != -1) {
        switch (c) {
        case 'd':
            cfg->descending_order = 1;
            break;
        case 'c':
            cfg->combine = 1;
            break;
        case 'o':
            cfg->remove_odd = 1;
            break;
        case 'z':
            cfg->zip_alternating = 1;
            break;
        default:
            fprintf(stderr, "invalid option: -%c\n", optopt);
            return 1;
        }
    }

    return 0;
}

/**
 * Tries to display values in the correct order based on string containing numbers.
 *
 * @param argc count.
 * @param argv array.
 * @return
 */
int main(int argc, char *argv[]) {
    struct config cfg;

    if (parse_options(&cfg, argc, argv) != 0) return 1;

    struct list *l = list_init();

    char *end_ptr;

    while (fgets(buf, BUF_SIZE, stdin)) {
        for (char *ptr = buf; *ptr != '\0';) {
            int num = (int) strtol(ptr, &end_ptr, 10);

            // If end_ptr moved to next char, because ptr is a number, add number to list.
            if (ptr != end_ptr) {
                struct node *n = list_new_node(num);

                list_add_front(l, n);
            }

            // Move the pointer to the next character after the number.
            ptr = end_ptr;

            // Skip whitespaces and new lines.
            while (*ptr == ' ' || *ptr == '\n') ptr++;
        }
    }

    insertion_sort(l, &cfg);

    // Output the sorted list.
    for (size_t i = 0; i < l->length; i++) {
        struct node *n = list_get_ith(l, i);

        fprintf(stdout, "%d\n", n->num);
    }

    list_cleanup(l);

    return 0;
}