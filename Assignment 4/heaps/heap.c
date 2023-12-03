/*H**********************************************************************
* FILENAME: heap.c
*
* DESCRIPTION:
*   Heap datastructure.
*
* PUBLIC FUNCTIONS:
*   prioq *prioq_init( (*compare)(*, *) )
*   long int prioq_size( q )
*   int prioq_cleanup( q, free_func(*) )
*   int prioq_insert( q, p )
*   void *prioq_pop( q )
*
* PRIVATE FUNCTIONS:
*   static struct heap *( (*compare)(*, *) )
*   static int heap_cleanup( h, free_func(*) )
*   static void array_swap( a, i_1, i_2)
*   static int heap_insert( h, q )
*   static void *heap_pop( h )
*
* AUTHOR: Thom Kok (Student nr: 15316491)
*
* START DATE: 30-11-2023
*
*H*/

#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "prioq.h"

/**
 * Reserve bytes in memory for heap and variables if possible.
 *
 * @param compare function to compare data.
 * @return
 */
static struct heap *heap_init(int (*compare)(const void *, const void *)) {
    struct heap *h = malloc(sizeof(struct heap));

    if (h == NULL) return NULL;

    h->array = array_init(2);
    h->compare = compare;

    return h;
}

/**
 * Reserve bytes in memory for priority queue and variables if possible.
 *
 * @param compare function to compare data.
 * @return
 */
prioq *prioq_init(int (*compare)(const void *, const void *)) {
    return heap_init(compare);
}

/**
 * Return the current size of the priority queue.
 *
 * @param q the priority queue.
 * @return
 */
long int prioq_size(const prioq *q) {
    if (q == NULL) return 0;

    return array_size(q->array);
}

/**
 * Free memory from heap and sub variables.
 *
 * @param h the heap.
 * @param free_func function used to free data.
 * @return
 */
static int heap_cleanup(struct heap *h, void free_func(void *)) {
    if (h == NULL) return 1;

    if (free_func == NULL) free_func = free;

    array_cleanup(h->array, free_func);

    return 0;
}

/**
 * Free memory from priority queue.
 *
 * @param q the priority queue.
 * @param free_func function used to free data.
 * @return
 */
int prioq_cleanup(prioq *q, void free_func(void *)) {
    if (q == NULL) return 1;

    if (free_func == NULL) free_func = free;

    heap_cleanup(q, free_func);

    free(q);

    return 0;
}

/**
 * Swaps two elements in the array.
 *
 * @param a the array.
 * @param i_1 index of the first element.
 * @param i_2 index of the second element.
 */
static void array_swap(struct array *a, int i_1, int i_2) {
    if (a == NULL) return;

    // Handle invalid indices.
    if (i_1 < 0 || i_1 >= array_size(a) || i_2 < 0 || i_2 >= array_size(a)) return;

    // Swap elements at indices i_1 and i_2.
    void *temp = array_get(a, i_1);

    array_set(a, i_1, array_get(a, i_2));
    array_set(a, i_2, temp);
}

/**
 * Insert patient in the heap and bubble it up when you insert it at the bottom of the heap.
 *
 * @param h the heap.
 * @param p the patient.
 * @return
 */
static int heap_insert(struct heap *h, void *p) {
    if (h == NULL) return 1;

    // Add patient before sorting.
    array_append(h->array, p);

    // Get the index of the last element.
    long int current_index = array_size(h->array) - 1;

    // Perform heapify-up to maintain the heap property.
    while (current_index > 0) {
        int parent_index = ((int) current_index - 1) / 2;

        void *parent = array_get(h->array, parent_index);
        void *child = array_get(h->array, current_index);

        // Compare the current child with its parent and stop if done.
        if (h->compare(parent, child) > 0) {
            // Swap if the current child is smaller than its parent.
            array_swap(h->array, parent_index, (int) current_index);

            // Move up the heap.
            current_index = parent_index;
        } else {
            break;
        }
    }

    return 0;
}

/**
 * Inserts patient in priority queue.
 *
 * @param q the priority queue.
 * @param p the patient.
 * @return
 */
int prioq_insert(prioq *q, void *p) {
    if (q == NULL || p == NULL) return 1;

    return heap_insert(q, p);
}

/**
 * Remove one heap from the priority queue and sink it down when you pop the root of the heap.
 *
 * @param h the heap.
 * @return
 */
static void *heap_pop(struct heap *h) {
    if (h == NULL || h->array == NULL || array_size(h->array) == 0) return NULL;

    // Swap the root with the last element.
    array_swap(h->array, 0, (int) array_size(h->array) - 1);

    // Pop the last element (which was the root).
    void *popped = array_pop(h->array);

    long int current_index = 0;

    // Perform heapify-down to maintain the heap property.
    while (1) {
        long int smallest = current_index;
        long int left_child_index = 2 * (int) current_index + 1;
        long int right_child_index = 2 * (int) current_index + 2;

        void *current = array_get(h->array, smallest);
        void *left_child = array_get(h->array, left_child_index);
        void *right_child = array_get(h->array, right_child_index);

        // Check if the left child is smaller than the current smallest.
        if (left_child_index < array_size(h->array) && h->compare(left_child, current) < 0) {
            smallest = left_child_index;
            current = array_get(h->array, smallest);
        }

        // Check if the right child is smaller than the current smallest.
        if (right_child_index < array_size(h->array) && h->compare(right_child, current) < 0) {
            smallest = right_child_index;
        }

        // If the smallest is not the current index, swap and continue heapify-down.
        if (smallest != current_index) {
            array_swap(h->array, (int) current_index, (int) smallest);
            current_index = smallest;
        } else {
            break;
        }
    }

    return popped;
}

/**
 * Remove one priority queue and remove the heaps.
 *
 * @param q the priority queue.
 * @return
 */
void *prioq_pop(prioq *q) {
    if (q == NULL) return NULL;

    return heap_pop(q);
}
