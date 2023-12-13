/*H**********************************************************************
* FILENAME: set.c
*
* DESCRIPTION:
*   Set datastructure.
*
* PUBLIC FUNCTIONS:
*   struct set *set_init( turbo )
*   int set_insert( s, num )
*   int set_find( s, num )
*   int set_remove( s, num )
*   void set_cleanup( s )
*   void set_print( s )
*   int set_verify( s )
*
* AUTHOR: Thom Kok (Student nr: 15316491)
*
* START DATE: 08-12-2023
*
*H*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "tree.h"

struct set {
    struct tree *tree;
    size_t size;
    size_t capacity;
    int turbo;
};

/**
 * Reserve bytes in memory for set.
 *
 * @param turbo use slower or faster tree datastructure.
 * @return
 */
struct set *set_init(int turbo) {
    struct set *s = malloc(sizeof(struct set));

    if (s == NULL) return NULL;

    s->tree = NULL;
    s->size = 0;
    s->capacity = 0;
    s->turbo = turbo;

    return s;
}

/**
 * Insert number into the set.
 *
 * @param s the set.
 * @param num the number.
 * @return
 */
int set_insert(struct set *s, int num) {
    if (s == NULL) return -1;

    if (s->tree == NULL) {
        s->tree = tree_init(s->turbo);

        if (s->tree == NULL) return -1;
    }

    if (tree_insert(s->tree, num) == 0) {
        s->size++;
        return 0;
    } else {
        return 1;
    }
}

/**
 * Find the number in the set.
 *
 * @param s the set.
 * @param num the number.
 * @return
 */
int set_find(struct set *s, int num) {
    if (s == NULL) return 0;
    if (s->tree == NULL) return 0;

    return tree_find(s->tree, num);
}

/**
 * Remove the number in the set.
 *
 * @param s the set.
 * @param num the number.
 * @return
 */
int set_remove(struct set *s, int num) {
    if (s == NULL) return 1;
    if (s->tree == NULL) return 1;

    if (tree_remove(s->tree, num) == 0) {
        s->size--;
        return 0;
    } else {
        return 1;
    }
}

/**
 * Free memory from set.
 *
 * @param s the set.
 */
void set_cleanup(struct set *s) {
    if (s != NULL) return;

    if (s->tree != NULL) tree_cleanup(s->tree);

    free(s);
}

/**
 * Print the contents of the set in sorted order.
 *
 * @param s the set.
 */
void set_print(const struct set *s) {
    if (s == NULL) return;

    tree_print(s->tree);
}

/**
 * Verify the set.
 *
 * @param s the set.
 * @return
 */
int set_verify(const struct set *s) {
    /* ... OPTIONALLY IMPLEMENT SET CHECKING HERE ... */
    return 0;
}
