/*H**********************************************************************
* FILENAME: hash_table.c
*
* DESCRIPTION:
*   Hash table datastructure.
*
* PUBLIC FUNCTIONS:
*   struct table *table_init( capacity, max_load_factor, (*hash_func)() )
*   int table_insert( table, key, value )
*   struct array *table_lookup( table, key )
*   double table_load_factor( table )
*   int table_delete( table, key )
*   void table_cleanup( table )
*
* PRIVATE FUNCTIONS:
*   static int resize_table( table )
*
* AUTHOR: Thom Kok (Student nr: 15316491)
*
* START DATE: 20-11-2023
*
*H*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "array.h"
#include "hash_table.h"

struct table {
    /* The (simple) array used to index the table */
    struct node **node_array;

    /* The function used for computing the hash values in this table */
    unsigned long (*hash_func)(const unsigned char *);

    /* Maximum load factor after which the table array should be resized */
    double max_load_factor;

    /* Capacity of the array used to index the table */
    unsigned long capacity;

    /* Current number of elements stored in the table */
    unsigned long load;
};

/* Note: This struct should be a *strong* hint to a specific type of hash table
 * You may implement other options, if you can build them in such a way they
 * pass all tests. However, the other options are generally harder to code. */
struct node {
    /* The string of characters that is the key for this node */
    char *key;

    /* A resizing array, containing all integer values for this key */
    struct array *array;

    /* Next pointer */
    struct node *next;
};

/**
 * Reserve bytes in memory for table and variables if possible.
 *
 * Note: The 'array' member of struct table is a pointer to a block of
 * memory that contains pointers to struct nodes. Make sure that the struct
 * node pointers in this block are all set to NULL. A good way to do this is
 * with the calloc() function. Check the manual page 'man calloc' for its usage.
 *
 * @param capacity Capacity of the array used to index the table.
 * @param max_load_factor Maximum load factor after which the table array should be resized.
 * @param hash_func The function used for computing the hash values in this table.
 * @return
 */
struct table *table_init(
    unsigned long capacity,
    double max_load_factor,
    unsigned long (*hash_func)(const unsigned char *)
) {
    if (!capacity) capacity = 1;

    struct table *t = malloc(sizeof(struct table));
    struct node **n = calloc(capacity, sizeof(struct node *));

    if (t == NULL || n == NULL) return NULL;

    t->load = 0;
    t->node_array = n;
    t->capacity = capacity;
    t->max_load_factor = max_load_factor;
    t->hash_func = hash_func;

    return t;
}

/**
 * Resize the table if it is going to exceed max load factor.
 *
 * @param t the table.
 * @return
 */
static int resize_table(struct table *t) {
    if (t == NULL) return 1;

    struct node **temp = calloc((t->load + 1) * 2, sizeof(struct node *));

    if (temp == NULL) return 1;

    // Rehash and redistribute existing keys.
    for (size_t i = 0; i < t->capacity; i++) {
        struct node *n = t->node_array[i];

        // If node exists change index.
        if (n != NULL) {
            unsigned long new_index = t->hash_func((unsigned char *) n->key) % ((t->load + 1) * 2);

            temp[new_index] = n;
        }
    }

    free(t->node_array);

    t->node_array = temp;
    t->capacity = (t->load + 1) * 2;

    return 0;
}

/**
 * Inserts key and value in a table node.
 *
 * @param t the table.
 * @param key the key of characters.
 * @param value the value
 * @return
 */
int table_insert(struct table *t, const char *key, int value) {
    if (t == NULL || strlen(key) == 0) return 1;

    // Allocate memory for n->key.
    char *str = malloc(strlen(key) + 1);

    if (str == NULL) return 1;

    // Copy the key into allocated memory.
    strcpy(str, key);

    // If size is going to exceed, add more memory.
    if ((double) (t->load + 1) / (double) t->capacity >= t->max_load_factor) {
        if (resize_table(t)) {
            free(str);

            return 1;
        }
    }

    unsigned long index = t->hash_func((unsigned char *) str) % t->capacity;

    struct node *checkNode = t->node_array[index];

    // Check if a node with the same string already exists.
    while (checkNode != NULL && checkNode->key != NULL && strcmp(checkNode->key, str) == 0) {
        array_append(checkNode->array, value);

        t->load++;

        free(str);

        return 0;
    }

    // Prepare new node to be added.
    struct node *n = malloc(sizeof(struct node));

    if (n == NULL) {
        free(str);

        return 1;
    }

    n->key = str;
    n->array = array_init(t->capacity);
    n->next = NULL;

    array_append(n->array, value);

    // Add node as first node or last node.
    if (checkNode == NULL) {
        t->node_array[index] = n;
    } else {
        while (checkNode->next != NULL) checkNode = checkNode->next;

        checkNode->next = n;
    }

    t->load++;

    return 0;
}

/**
 * Get array with integer values from table by key.
 *
 * @param t the table.
 * @param key the key you want to find in the table.
 * @return
 */
struct array *table_lookup(const struct table *t, const char *key) {
    if (t == NULL) return NULL;
    if (t->load == 0) return NULL;

    for (long unsigned int node_i = 0; node_i < t->capacity; node_i++) {
        if (t->node_array[node_i] == NULL) continue;

        struct node *checkNode = t->node_array[node_i];

        while (checkNode != NULL) {
            if (checkNode->key != NULL && strcmp(checkNode->key, key) == 0) return checkNode->array;

            checkNode = checkNode->next;
        }
    }

    return NULL;
}

/**
 * Get load factor of table.
 *
 * @param t the table.
 * @return
 */
double table_load_factor(const struct table *t) {
    if (t == NULL) return -1.0;
    if (t->capacity <= 0) return -1.0;

    return (double) t->load / (double) t->capacity;
}

/**
 * Remove the key and value from the hash table.
 *
 * @param t the table.
 * @param key the key.
 * @return
 */
int table_delete(struct table *t, const char *key) {
    if (t == NULL || strlen(key) == 0) return -1;

    unsigned long index = t->hash_func((unsigned char *) key) % t->capacity;

    struct node *current = t->node_array[index];

    struct node *prev = NULL;

    while (current != NULL) {
        if (current->key != NULL && strcmp(current->key, key) == 0) {
            // If key is found at first node or in the node chain.
            if (prev == NULL) {
                t->node_array[index] = current->next;
            } else {
                prev->next = current->next;
            }

            free(current->key);
            array_cleanup(current->array);
            free(current);

            t->load--;

            return 0;
        }

        // Move to the next node.
        prev = current;
        current = current->next;
    }

    return 1;
}

/**
 * Free memory from table and sub variables.
 *
 * @param t the table.
 */
void table_cleanup(struct table *t) {
    if(t == NULL) return;

    for (size_t node_i = 0; node_i < t->capacity; ++node_i) {
        struct node *n = t->node_array[node_i];

        // Free data if node is found.
        while (n != NULL) {
            struct node *temp = n;

            free(temp->key);
            array_cleanup(temp->array);

            // Prepare loop to go to next node.
            n = n->next;

            free(temp);
        }
    }

    free(t->node_array);
    free(t);
}
