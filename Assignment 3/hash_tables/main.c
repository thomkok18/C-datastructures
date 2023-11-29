#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "array.h"
#include "hash_func.h"
#include "hash_table.h"

#define LINE_LENGTH 256

#define TABLE_START_SIZE 256
#define MAX_LOAD_FACTOR 0.6
#define HASH_FUNCTION hash_too_simple

#define START_TESTS 2
#define MAX_TESTS 2
#define HASH_TESTS 1

/**
 * Replace every non-ascii char with a space and lowercase every char.
 *
 * @param line the line of characters.
 */
static void cleanup_string(char *line) {
    for (char *c = line; *c != '\0'; c++) {
        *c = (char) tolower(*c);

        if (!isalpha(*c)) *c = ' ';
    }
}

/**
 * Return a pointer to a heap allocated string with all the characters we use as word delimiters.
 * Return NULL on failure.
 *
 * @return
 */
static char *calc_delim(void) {
    const size_t ascii_table_size = 128;

    char *res = malloc(sizeof(char) * ascii_table_size);

    if (!res) return NULL;

    int res_index = 0;

    for (unsigned char c = 1; c < ascii_table_size; c++) {
        if (!isalpha(c)) res[res_index++] = (char) c; /* if it's not an alpha it's a delimiter char. */
    }

    res[res_index] = '\0';

    return res;
}

/**
 * Creates a hash table with a word index for the specified file and parameters.
 * Return a pointer to hash table or NULL if an error occurred.
 *
 * @param filename the name of the file to find words.
 * @param start_size the size of the hash table.
 * @param max_load the max load factor for the hash table.
 * @param hash_func the hash function used to save words.
 * @return
 */
static struct table *create_from_file(
    char *filename,
    unsigned long start_size,
    double max_load,
    unsigned long (*hash_func)(const unsigned char *)
) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) return NULL;

    char *line = malloc(LINE_LENGTH * sizeof(char));

    if (!line) {
        fclose(fp);

        return NULL;
    }

    struct table *hash_table = table_init(start_size, max_load, hash_func);

    if (hash_table == NULL) {
        fclose(fp);
        free(line);

        return NULL;
    }

    size_t line_number = 1;

    // Calculate the delimiter string.
    char *delimiter = calc_delim();

    while (fgets(line, LINE_LENGTH, fp)) {
        // Remove newline character at the end of the line.
        size_t len = strlen(line);

        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Modify string to clean it up.
        cleanup_string(line);

        char *word = strtok(line, delimiter);

        while (word != NULL) {
            table_insert(hash_table, word, (int) line_number);

            // Get the next word.
            word = strtok(NULL, delimiter);
        }

        line_number++;
    }

    fclose(fp);

    free(line);
    free(delimiter);

    return hash_table;
}

/**
 * Reads words from stdin and prints line lookup results per word. Return 0 if successful and 1 on failure.
 *
 * @param hash_table the hash table.
 * @return
 */
static int stdin_lookup(struct table *hash_table) {
    char *line = malloc(LINE_LENGTH * sizeof(char));

    if (!line) return 1;

    while (fgets(line, LINE_LENGTH, stdin)) {
        char *word = strtok(line, " \t\n");

        for (int i = 0; word[i]; i++) {
            word[i] = (char) tolower(word[i]);
        }

        while (word != NULL) {
            // Lookup the word in the hash table.
            struct array *result = table_lookup(hash_table, word);

            // Print the word and the corresponding line numbers.
            if (result != NULL) {
                for (size_t i = 0; i < array_size(result); i++) {
                    printf("* %d\n", array_get(result, i));
                }
            } else {
                printf("Word: %s, Not Found\n", word);
            }

            // Get the next word.
            word = strtok(NULL, " \t\n");
        }
    }

    free(line);

    return 0;
}

/**
 * Measure time of execution.
 *
 * Here you can edit the hash table testing parameters: Starting size,
 * maximum load factor and hash function used, and see the effect on the time it takes to build the table.
 * You can edit the tested values in the 3 arrays below.
 * If you change the number of elements in the array, change the defined constants at the top of the file too,
 * to change the size of the arrays.
 *
 * @param filename the file for finding the words.
 */
static void timed_construction(char *filename) {
    unsigned long start_sizes[START_TESTS] = { 2, 65536 };
    double max_loads[MAX_TESTS] = { 0.2, 1.0 };
    unsigned long (*hash_funcs[HASH_TESTS])(const unsigned char *) = { hash_too_simple };

    for (int i = 0; i < START_TESTS; i++) {
        for (int j = 0; j < MAX_TESTS; j++) {
            for (int k = 0; k < HASH_TESTS; k++) {
                clock_t start = clock();

                struct table *hash_table = create_from_file(filename, start_sizes[i], max_loads[j], hash_funcs[k]);

                clock_t end = clock();

                printf("Start: %ld\tMax: %.1f\tHash: %d\t -> Time: %ld "
                       "microsecs\n",
                       start_sizes[i], max_loads[j], k, end - start);

                table_cleanup(hash_table);
            }
        }
    }
}

/**
 * Create and use hash table to find words.
 *
 * @param argc count.
 * @param argv array.
 * @return
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: %s text_file [-t]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 3 && !strcmp(argv[2], "-t")) {
        timed_construction(argv[1]);
    } else {
        struct table *hash_table = create_from_file(argv[1], TABLE_START_SIZE, MAX_LOAD_FACTOR, HASH_FUNCTION);

        if (hash_table == NULL) {
            printf("An error occurred creating the hash table, exiting..\n");
            return EXIT_FAILURE;
        }

        if (stdin_lookup(hash_table) != 0) {
            table_cleanup(hash_table);
            return EXIT_FAILURE;
        }

        table_cleanup(hash_table);
    }

    return EXIT_SUCCESS;
}
