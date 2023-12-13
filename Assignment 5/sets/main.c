/*H**********************************************************************
* FILENAME: main.c
*
* DESCRIPTION:
*   Set manipulation program based on operands to manipulate a tree.
*
* PUBLIC FUNCTIONS:
*   void cleanup_and_fail( buf, s )
*   void execute_command( buf, s, num, command )
*   int main( void )
*
* AUTHOR: Thom Kok (Student nr: 15316491)
*
* START DATE: 08-12-2023
*
*H*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"

#define BUF_SIZE 256

/**
 * Clean up set or return failure.
 *
 * @param buf the buffer.
 * @param s the set.
 */
void cleanup_and_fail(char *buf, struct set *s) {
    if (s) set_cleanup(s);

    free(buf);

    exit(EXIT_FAILURE);
}

/**
 * Executing the commands.
 *
 * @param buf the buffer.
 * @param s the set.
 * @param num the number from the file.
 * @param command the command value.
 */
void execute_command(char *buf, struct set *s, int num, char *command) {
    switch (*command) {
        case '+':
            set_insert(s, num);
            break;
        case '-':
            set_remove(s, num);
            break;
        case '?':
            if (set_find(s, num)) {
                printf("found: %d\n", num);
            } else {
                printf("not found: %d\n", num);
            }

            break;
        case 'p':
            set_print(s);
            break;
        default:
            printf("Unknown command: %s\n", command);
            cleanup_and_fail(buf, s);
    }
}

/**
 * Try changing the tree, based on txt files.
 *
 * @return
 */
int main(void) {
    char *buf = malloc(BUF_SIZE);

    if (!buf) {
        perror("Could not allocate input buffer");
        return EXIT_FAILURE;
    }

    struct set *s = set_init(0);   /* initialize set with turbo turned off. */

    if (s == NULL) return EXIT_FAILURE;

    while (fgets(buf, BUF_SIZE, stdin)) {
        char *endptr;
        char *command;
        char *num_str;
        int num = 0;

        command = strtok(buf, " ");     /* get command: +,-,?,p */

        if (strchr("+-?", *command)) {  /* operation with operand */
            num_str = strtok(NULL, "\n");

            if (!num_str) {
                printf("set operation '%c' requires integer operand\n", *command);
                cleanup_and_fail(buf, s);
            }

            num = (int) strtol(num_str, &endptr, 10);

            if (*endptr != '\0') {
                printf("error converting '%s' to an integer\n", num_str);
                cleanup_and_fail(buf, s);
            }
        }

        execute_command(buf, s, num, command);
    }

    if (set_verify(s)) { /* debug function */
        fprintf(stderr, "Set implementation failed verification!\n");
    }

    free(buf);
    set_cleanup(s);

    return EXIT_SUCCESS;
}
