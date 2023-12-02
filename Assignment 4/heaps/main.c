/*H**********************************************************************
* FILENAME: main.c
*
* DESCRIPTION:
*   Waiting queue manager to handle patient queues by name in alphabetical order.
*
* PUBLIC FUNCTIONS:
*   int main( argc, argv[] )
*
* PRIVATE FUNCTIONS:
*   static int parse_options( cfg, argc, argv[] )
*   static int compare_patient_name( a, b )
*   static int compare_patient_age( a, b )
*
* AUTHOR: Thom Kok (Student nr: 15316491)
*
* START DATE: 30-11-2023
*
*H*/

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "prioq.h"

#define BUF_SIZE 1024

static char buf[BUF_SIZE];

struct config {
    /* You can ignore these options until/unless you implement the */
    /* bonus features. */

    /* Set to 1 if -y is specified, 0 otherwise. */
    int year;
};

static int parse_options(struct config *cfg, int argc, char *argv[]);

typedef struct {
    char *name;
    /* ... CODE MISSING HERE .... */
} patient_t;

/**
 * Compare the name of two patients.
 *
 * @param a patient A.
 * @param b patient B.
 * @return
 */
static int compare_patient_name(const void *a, const void *b) {
    return strcmp(((const patient_t *) a)->name, ((const patient_t *) b)->name);
}

/**
 * Compare the age of two patients.
 *
 * @param a patient A.
 * @param b patient B.
 * @return
 */
static int compare_patient_age(const void *a, const void *b) {
    /* ... CODE MISSING HERE .... */
    return 0;
}

int main(int argc, char *argv[]) {
    char *token, *name_cpy;
    prioq *queue;
    struct config cfg;

    if (parse_options(&cfg, argc, argv) != 0) return EXIT_FAILURE;

    if (cfg.year) {
        queue = prioq_init(&compare_patient_age);
    } else {
        queue = prioq_init(&compare_patient_name);
    }

    // TODO: Clean priority queue later.
    prioq_cleanup(queue, NULL);

    /* ... CODE MISSING HERE .... */

    for (int iterations = 0;;) {
        while (1) {
            char *s = fgets(buf, BUF_SIZE, stdin);

            if (s == NULL) {
                fprintf(stderr, "Unexpected end of file. exiting\n");
                /* ... CODE MISSING HERE .... */
                return EXIT_FAILURE;
            }

            /* ... CODE MISSING HERE .... */
        }

        printf(".\n"); /* End turn. */

        if (++iterations == 10) {

            /* ... CODE MISSING HERE .... */

            break;
        }
    }

    /* ... CODE MISSING HERE .... */

    return EXIT_SUCCESS;
}

int parse_options(struct config *cfg, int argc, char *argv[]) {
    memset(cfg, 0, sizeof(struct config));

    int c;

    while ((c = getopt(argc, argv, "y")) != -1) {
        switch (c) {
        case 'y':
            cfg->year = 1;
            break;
        default:
            fprintf(stderr, "invalid option: -%c\n", optopt);
            return 1;
        }
    }

    return 0;
}
