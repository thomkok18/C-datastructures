/*H**********************************************************************
* FILENAME: array.c
*
* DESCRIPTION:
*   Array datastructure.
*
* PUBLIC FUNCTIONS:
*   struct array *array_init( initial_capacity )
*   void array_cleanup( array )
*   int *array_get( array, index )
*   int array_append( array, element )
*   unsigned long array_size( array )
*
* AUTHOR: Thom Kok (Student nr: 15316491)
*
* START DATE: 20-11-2023
*
*H*/

#include <stdio.h>
#include <stdlib.h>

#include "array.h"

struct array {
    int *data;
    size_t length;
    size_t capacity;
};

/**
 * Reserve bytes in memory for array and variables if possible.
 *
 * @param initial_capacity the amount of characters savable in array.
 * @return
 */
struct array *array_init(unsigned long initial_capacity) {
    if (!initial_capacity) initial_capacity = 1;

    struct array *a = malloc(sizeof(struct array));

    if (a == NULL) return NULL;

    a->data = malloc(sizeof(int) * initial_capacity);

    if (a->data == NULL) {
        array_cleanup(a);

        return NULL;
    }

    a->length = 0;
    a->capacity = initial_capacity;

    return a;
}

/**
 * Free memory from array and sub variables.
 *
 * @param a the array.
 */
void array_cleanup(struct array *a) {
    if(a == NULL) return;

    if(a->data != NULL) free(a->data);

    free(a);
}

/**
 * Get value from array by index.
 *
 * @param a the array.
 * @param index the position of the index.
 * @return
 */
int array_get(const struct array *a, unsigned long index) {
    if (a == NULL) return -1;
    if (index > a->length) return -1;

    return a->data[index];
}

/**
 * Add element to array. Return 0 if successful else 1.
 *
 * Note: Although this operation might require the array to be resized and
 * copied, in order to make room for the added element, it is possible to do
 * this in such a way that the amortized complexity is still O(1).
 * Make sure your code is implemented in such a way to guarantee this.
 *
 * @param a
 * @param elem
 * @return
 */
int array_append(struct array *a, int elem) {
    if (a == NULL) return 1;

    // If size is going to exceed, add more memory.
    if ((a->length + 1) >= a->capacity) {
        int *temp = realloc(a->data, sizeof(int) * ((a->length + 1) * 2));

        if (temp == NULL) return 0;

        a->capacity = (a->length + 1) * 2;
        a->data = temp;
    }

    a->data[a->length] = elem;

    a->length++;

    return 0;
}

/**
 * Return the current size of the array.
 *
 * @param a the array.
 * @return
 */
unsigned long array_size(const struct array *a) {
    if (a == NULL) return 0;

    return a->length;
}
