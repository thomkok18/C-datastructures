#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct queue {
    int *data;
    int pushes;
    int pops;
    size_t first_in_line;
    size_t length;
    size_t capacity;
};

/**
 * Reserve bytes in memory for queue and variables if possible.
 *
 * @param capacity the amount of integers savable in queue.
 */
struct queue *queue_init(size_t capacity) {
    if (!capacity) capacity = 1;

    struct queue *qu = malloc(sizeof(struct queue));

    if (qu == NULL) return NULL;

    qu->data = malloc(sizeof(int) * capacity);

    if (qu->data == NULL) {
        queue_cleanup(qu);

        return NULL;
    }

    qu->pushes = 0;
    qu->pops = 0;
    qu->first_in_line = 0;
    qu->length = 0;
    qu->capacity = capacity;

    return qu;
}

/**
 * Free memory from queue and sub variables.
 *
 * @param q the queue.
 */
void queue_cleanup(struct queue *q) {
    if(q == NULL) return;

    if(q->data != NULL) free(q->data);

    free(q);
}

/**
 * Shows stats about the queue.
 *
 * @param q the stack.
 */
void queue_stats(const struct queue *q) {
    if (q == NULL) return;

    fprintf(stderr,"stats %d %d %d\n", q->pushes, q->pops, abs(q->pops - (int) q->length));
}

/**
 * Push one character into the queue. Return 0 if successful else 1.
 *
 * @param q the queue.
 * @param c the character pushed into the queue.
 * @return
 */
int queue_push(struct queue *q, int c) {
    if (q == NULL) return 1;

    // If size is going to exceed, add more memory.
    if ((q->length + 1) >= q->capacity) {
        int *temp = realloc(q->data, sizeof(int) * ((q->length + 1) * 2));

        if (temp == NULL) return 0;

        q->capacity = q->length * 2;
        q->data = temp;
    }

    // If length of queue is going to exceed capacity, move start queue to position 0.
    if ((q->length + q->first_in_line) == q->capacity) {
        for (size_t i = 0; i < q->length; i++) {
            q->data[i] = q->data[i + q->first_in_line];

            q->data[i + q->first_in_line] = '\0';
        }

        q->first_in_line = 0;
    }

    q->data[q->length + q->first_in_line] = c;

    q->length++;
    q->pushes++;

    return 0;
}

/**
 * Remove one character from the queue. If empty return -1.
 *
 * @param q the queue.
 * @return
 */
int queue_pop(struct queue *q) {
    if (q == NULL) return -1;

    if (q->length == 0) return -1;

    int first_character = q->data[q->first_in_line];

    q->data[q->first_in_line] = '\0';

    q->length--;
    q->pops++;
    q->first_in_line++;

    return first_character;
}

/**
 * Get first item of queue. If empty return -1.
 *
 * @param q the queue.
 * @return
 */
int queue_peek(const struct queue *q) {
    if (q == NULL) return -1;
    if (q->length == 0) return -1;

    return q->data[q->first_in_line];
}

/**
 * Check if queue is empty or not.
 *
 * @param q the queue.
 * @return
 */
int queue_empty(const struct queue *q) {
    if (q == NULL) return -1;
    if (q->length == 0) return 1;
    if (q->length > 0) return 0;

    return -1;
}

/**
 * Return the current length of the queue.
 *
 * @param q the queue.
 * @return
 */
size_t queue_size(const struct queue *q) {
    if (q == NULL) return 0;

    return q->length;
}
