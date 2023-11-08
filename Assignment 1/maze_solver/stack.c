#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

struct stack {
    int *data;
    int pushes;
    int pops;
    size_t size;
    size_t capacity;
};

/**
 * Reserve bytes in memory for stack and variables if possible.
 *
 * @param capacity the amount of integers savable in stack.
 */
struct stack *stack_init(size_t capacity) {
    if (!capacity) capacity = 1;

    struct stack *st = malloc(sizeof(struct stack));

    if (st == NULL) {
        //fprintf(stderr,"Not enough space for stack.\n");
        return NULL;
    }

    st->data = malloc(sizeof(int) * capacity);

    if (st->data == NULL) {
        stack_cleanup(st);

        //fprintf(stderr,"Not enough space for data or size.\n");
        return NULL;
    } else {
        st->pushes = 0;
        st->pops = 0;
        st->size = 0;
        st->capacity = capacity;

        return st;
    }
}

/**
 * Free memory from stack and sub variables.
 *
 * @param s the stack.
 */
void stack_cleanup(struct stack *s) {
    if(s == NULL) return;
    if(s->data != NULL) free(s->data);
    if(s != NULL) free(s);
}

/**
 * Shows stats about the stack.
 *
 * @param s the stack.
 */
void stack_stats(const struct stack *s) {
    if (s == NULL) return;

    fprintf(stderr,"stats %d %d %d\n", s->pushes, s->pops, abs(s->pops - (int) s->size));
}

/**
 * Push one character into the stack. Return 0 if successful else 1.
 *
 * @param s the stack.
 * @param c the character pushed into the stack.
 * @return
 */
int stack_push(struct stack *s, int c) {
    if (s == NULL) return 1;

    // If size is going to exceed, add more memory.
    if ((s->size + 1) >= s->capacity) {
        int *temp = realloc(s->data, sizeof(int) * ((s->size + 1) * 2));

        if (temp == NULL) {
            //fprintf(stderr,"Memory Re-allocation failed. Unable to add value to stack.\n");
            return 0;
        } else {
            s->capacity = (s->size + 1) * 2;
            s->data = temp;

            //fprintf(stderr,"Memory successfully re-allocated for push.\n");
        }
    }

    s->data[s->size] = c;

    s->size++;
    s->pushes++;

    return 0;
}

/**
 * Remove one character from the stack. If empty return -1.
 *
 * @param s the stack.
 * @return
 */
int stack_pop(struct stack *s) {
    if (s == NULL) return -1;

    if (s->size == 0) {
        //fprintf(stderr,"Stack is already empty.\n");
        return -1;
    } else {
        s->data[s->size] = '\0';

        s->size--;
        s->pops++;

        return s->data[s->size];
    }
}

/**
 * Get top item of stack. If empty return -1.
 *
 * @param s the stack.
 * @return
 */
int stack_peek(const struct stack *s) {
    if (s == NULL) return -1;
    if (s->size == 0) return -1;

    return s->data[s->size - 1];
}

/**
 * Check if stack is empty or not.
 *
 * @param s the stack.
 * @return
 */
int stack_empty(const struct stack *s) {
    if (s == NULL) return -1;
    if (s->size == 0) return 1;
    if (s->size > 0) return 0;

    return -1;
}

/**
 * Return the current size of the stack.
 *
 * @param s the stack.
 * @return
 */
size_t stack_size(const struct stack *s) {
    if (s == NULL) return 0;

    return s->size;
}
