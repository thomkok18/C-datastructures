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
*   static void cleanup_patient( p )
*   static int add_patient( queue, token )
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
    char *age;
    char *duration;
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
    const patient_t *patient_a = (const patient_t *)a;
    const patient_t *patient_b = (const patient_t *)b;

    // Convert age strings to integers.
    long int age_a = strtol(patient_a->age, NULL, 10);
    long int age_b = strtol(patient_b->age, NULL, 10);

    if (age_a < age_b) {
        return -1;
    } else if (age_a > age_b) {
        return 1;
    }

    // Ages are equal, compare names alphabetically.
    return compare_patient_name(a, b);
}

/**
 * Cleanup the patient.
 *
 * @param p the patient.
 */
static void cleanup_patient(patient_t *p) {
    free(p->name);
    free(p->age);
    free(p->duration);
    free(p);
}

/**
 * Add the patient to the priority queue.
 *
 * @param queue the priority queue.
 * @param token a token of the full string (containing patient name).
 * @return
 */
static int add_patient(prioq *queue, char *token) {
    if (queue == NULL || token == NULL) return 1;

    patient_t *new_patient = malloc(sizeof(patient_t));

    if (new_patient == NULL) return 1;

    new_patient->name = NULL;
    new_patient->age = NULL;
    new_patient->duration = NULL;

    new_patient->name = malloc(strlen(token) + 1);

    if (new_patient->name != NULL) strcpy(new_patient->name, token);

    if (new_patient->name == NULL) {
        free(new_patient);

        return 1;
    }

    // Extract age.
    token = strtok(NULL, " ");

    if (token != NULL) {
        new_patient->age = malloc(strlen(token) + 1);

        if (new_patient->age != NULL) strcpy(new_patient->age, token);
    }

    // Extract duration.
    token = strtok(NULL, " ");

    if (token != NULL) {
        new_patient->duration = malloc(strlen(token) + 1);

        if (new_patient->duration != NULL) strcpy(new_patient->duration, token);
    }

    // Insert into the priority queue.
    if (prioq_insert(queue, new_patient) != 0) {
        cleanup_patient(new_patient);

        return 1;
    }

    return 0;
}

/**
 * Print patient queues.
 *
 * @param argc count.
 * @param argv array.
 * @return
 */
int main(int argc, char *argv[]) {
    prioq *queue;
    struct config cfg;

    if (parse_options(&cfg, argc, argv) != 0) return EXIT_FAILURE;

    if (cfg.year) {
        queue = prioq_init(&compare_patient_age);
    } else {
        queue = prioq_init(&compare_patient_name);
    }

    if (queue == NULL) {
        fprintf(stderr, "Failed to initialize priority queue.\n");

        return EXIT_FAILURE;
    }

    for (int iterations = 0;;) {
        while (1) {
            char *s = fgets(buf, BUF_SIZE, stdin);

            if (s == NULL) {
                fprintf(stderr, "Unexpected end of file. exiting\n");

                prioq_cleanup(queue, NULL);

                return EXIT_FAILURE;
            }

            if (s[0] == '.') break;

            // Get token of string and start with name.
            char *patient_name = strtok(buf, " ");
            add_patient(queue, patient_name);
        }

        patient_t *patient = prioq_pop(queue);

        if (patient) {
            if (patient->duration != NULL) {
                for (int i = 1; i < strtol(patient->duration, NULL, 10); i++) {
                    printf(".\n");
                }
            }

            if (patient->name != NULL) printf("%s\n", patient->name);

            printf(".\n"); // Patient is done at the doctor.

            cleanup_patient(patient);
        } else {
            printf(".\n"); // Doctor has no patient.
        }

        // Show patients not treated after waiting in queue and clean the queue.
        if (++iterations == 10) {
            while (prioq_size(queue) > 0) {
                patient_t *p = prioq_pop(queue);

                if (p->name != NULL) printf("%s\n", p->name);

                cleanup_patient(p);
            }

            prioq_cleanup(queue, NULL);

            break;
        }
    }

    return EXIT_SUCCESS;
}

/**
 * Parse the options given.
 *
 * @param cfg the config.
 * @param argc count.
 * @param argv array.
 * @return
 */
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
