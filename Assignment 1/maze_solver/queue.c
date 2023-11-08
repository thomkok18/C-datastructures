#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct queue {
    int *data;
    int pushes;
    int pops;
    size_t firstInLine;
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

    if (qu == NULL) {
        //fprintf(stderr,"Not enough space for stack.\n");
        return NULL;
    }

    qu->data = malloc(sizeof(int) * capacity);

    if (qu->data == NULL) {
        queue_cleanup(qu);

        //fprintf(stderr,"Not enough space for data or size.\n");
        return NULL;
    } else {
        qu->pushes = 0;
        qu->pops = 0;
        qu->firstInLine = 0;
        qu->length = 0;
        qu->capacity = capacity;

        return qu;
    }
}

/**
 * Free memory from queue and sub variables.
 *
 * @param q the queue.
 */
void queue_cleanup(struct queue *q) {
    if(q == NULL) return;
    if(q->data != NULL) free(q->data);
    if(q != NULL) free(q);
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

        if (temp == NULL) {
            //fprintf(stderr,"Memory Re-allocation failed. Unable to add value to queue.\n");
            return 0;
        } else {
            q->capacity = q->length * 2;
            q->data = temp;

            //fprintf(stderr,"Memory successfully re-allocated for push.\n");
        }
    }

    // If length of queue is going to exceed capacity, move start queue to position 0.
    if ((q->length + q->firstInLine) == q->capacity) {
        for (size_t i = 0; i < q->length; i++) {
            q->data[i] = q->data[i + q->firstInLine];

            q->data[i + q->firstInLine] = '\0';
        }

        q->firstInLine = 0;
    }

    q->data[q->length + q->firstInLine] = c;

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

    if (q->length == 0) {
        //fprintf(stderr,"Queue is already empty.\n");
        return -1;
    } else {
        int firstCharacter = q->data[q->firstInLine];

        q->data[q->firstInLine] = '\0';

        q->length--;
        q->pops++;
        q->firstInLine++;

        return firstCharacter;
    }
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

    return q->data[q->firstInLine];
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
