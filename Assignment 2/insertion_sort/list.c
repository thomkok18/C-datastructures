/*H**********************************************************************
* FILENAME: list.c
*
* DESCRIPTION:
*   List datastructure.
*
* PUBLIC FUNCTIONS:
*   struct list *list_init(void)
*   struct node *list_new_node( num )
*   struct node *list_head( l )
*   struct node *list_next( n )
*   int list_add_front( l, n )
*   struct node *list_tail( l )
*   struct node *list_prev( l, n )
*   int list_add_back( l, n )
*   int list_node_get_value( n )
*   int list_node_set_value( n, value )
*   int list_unlink_node( l, n )
*   void list_free_node( n )
*   int list_cleanup( l )
*   int list_node_present( l, n )
*   int list_insert_after( l, n, m )
*   int list_insert_before( l, n, m )
*   size_t list_length( l )
*   struct node *list_get_ith( l, i )
*   struct list *list_cut_after( l, n )
*
* AUTHOR: Thom Kok (Student nr: 15316491)
*
* START DATE: 09-11-2023
*
*H*/

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct node {
    struct list *list;
    struct node *next;
    struct node *prev;
    int num;
};

struct list {
    struct node *head;
    struct node *tail;
    size_t length;
};

/**
 * Reserve bytes in memory for list.
 *
 * @return
 */
struct list *list_init(void) {
    struct list *l = malloc(sizeof(struct list));

    if (l == NULL) return NULL;

    l->length = 0;

    return l;
}

/**
 * Reserve bytes in memory for node and add num into it.
 *
 * @param num number in node.
 * @return
 */
struct node *list_new_node(int num) {
    struct node *n = malloc(sizeof(struct node));

    if (n == NULL) return NULL;

    n->list = NULL;
    n->prev = NULL;
    n->next = NULL;
    n->num = num;

    return n;
}

/**
 * Get first node in list.
 *
 * @param l the list.
 * @return
 */
struct node *list_head(const struct list *l) {
    if (l == NULL) return NULL;

    return l->head;
}

/**
 * Get node after this node.
 *
 * @param n
 * @return
 */
struct node *list_next(const struct node *n) {
    if (n == NULL) return NULL;
    if (n->next == NULL) return NULL;

    return n->next;
}

/**
 * Add node to front of the list.
 *
 * @param l the list.
 * @param n the node.
 * @return
 */
int list_add_front(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) return 1;

    if (l->length == 0) {
        l->head = n;
        l->tail = n;
        n->list = l;

        l->length++;

        return 0;
    } else {
        n->next = l->head;
        l->head->prev = n;
        l->head = n;
        n->list = l;

        l->length++;

        return 0;
    }
}

/**
 * Get last node in list.
 *
 * @param l the list.
 * @return
 */
struct node *list_tail(const struct list *l) {
    if (l == NULL) return NULL;

    return l->tail;
}

/**
 * Get previous node from list.
 *
 * @param l the list.
 * @param n the node.
 * @return
 */
struct node *list_prev(const struct list *l, const struct node *n) {
    if (l == NULL || n == NULL) return NULL;
    if (l != n->list || n->prev == NULL) return NULL;

    return n->prev;
}

/**
 * Add node to back of the list.
 *
 * @param l the list.
 * @param n the node.
 * @return
 */
int list_add_back(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) return 1;

    if (l->length == 0) {
        l->head = n;
        l->tail = n;
        n->list = l;

        l->length++;

        return 0;
    } else {
        n->prev = l->tail;
        l->tail->next = n;
        l->tail = n;
        n->list = l;

        l->length++;

        return 0;
    }
}

/**
 * Get num value from node.
 *
 * @param n the node.
 * @return
 */
int list_node_get_value(const struct node *n) {
    if (n == NULL) return 0;

    return n->num;
}

/**
 * Set num value for node.
 *
 * @param n
 * @param value
 * @return
 */
int list_node_set_value(struct node *n, int value) {
    if (n == NULL) return 1;

    n->num = value;

    return 0;
}

/**
 * Remove link between node and list.
 *
 * @param l the list.
 * @param n the node.
 * @return
 */
int list_unlink_node(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) return 1;
    if (l->length == 0 || l != n->list) return 1;

    struct node *prev_node = n->prev;
    struct node *next_node = n->next;

    if (prev_node != NULL && next_node != NULL) {
        prev_node->next = n->next;
        next_node->prev = n->prev;
    } else if (prev_node != NULL) {
        prev_node->next = n->next;
        l->tail = prev_node;
    } else if (next_node != NULL) {
        next_node->prev = n->prev;
        l->head = next_node;
    } else {
        l->head = NULL;
        l->tail = NULL;
    }

    n->list = NULL;
    n->prev = NULL;
    n->next = NULL;

    l->length--;

    return 0;
}

/**
 * Free memory from node.
 *
 * @param n the node.
 */
void list_free_node(struct node *n) {
    if (n != NULL) free(n);
}

/**
 * Free memory from list and sub variables.
 *
 * @param l the list.
 */
int list_cleanup(struct list *l) {
    if(l == NULL) return 1;

    if (l->length != 0) {
        struct node *nodes[l->length];

        if (l->head != NULL) {
            nodes[0] = l->head;

            // Get all nodes from list.
            for (size_t i = 1; i < l->length; i++) {
                nodes[i] = nodes[i - 1]->next;
            }

            // Free nodes from memory.
            for (size_t i = 0; i < l->length; i++) {
                list_free_node(nodes[i]);
            }
        }
    }

    if(l != NULL) free(l);

    return 0;
}

/**
 * Check if node is in the specified list.
 *
 * @param l the list.
 * @param n the node.
 * @return
 */
int list_node_present(const struct list *l, const struct node *n) {
    if (l == NULL || n == NULL) return -1;
    if (l != n->list) return 0;

    return 1;
}

/**
 * Insert node n after node m.
 *
 * @param l the list.
 * @param n the node you want to insert after m.
 * @param m the node before node n.
 * @return
 */
int list_insert_after(struct list *l, struct node *n, struct node *m) {
    if (l == NULL || n == NULL || m == NULL) return 1;
    if (l->length == 0 || l != m->list || l == n->list) return 1;

    struct node *next_node = m->next;

    if (next_node != NULL) {
        next_node->prev = n;

        n->next = next_node;
        n->prev = m;

        m->next = n;

        n->list = l;
        l->length++;

        return 0;
    } else {
        m->next = n;
        n->prev = m;

        n->list = l;

        l->tail = n;
        l->length++;

        return 0;
    }
}

/**
 * Insert node n before node m.
 *
 * @param l the list.
 * @param n the node you want to insert before m.
 * @param m the node after node n.
 * @return
 */
int list_insert_before(struct list *l, struct node *n, struct node *m) {
    if (l == NULL || n == NULL || m == NULL) return 1;
    if (l->length == 0 || l != m->list || l == n->list) return 1;

    struct node *prev_node = m->prev;

    if (prev_node != NULL) {
        prev_node->next = n;

        n->prev = prev_node;
        n->next = m;

        m->prev = n;

        n->list = l;
        l->length++;

        return 0;
    } else {
        n->next = m;
        m->prev = n;

        n->list = l;

        l->head = n;
        l->length++;

        return 0;
    }
}

/**
 * Get the length of the list.
 *
 * @param l the list.
 * @return
 */
size_t list_length(const struct list *l) {
    if (l == NULL) return 0;

    return l->length;
}

/**
 * Get node based on the index.
 *
 * @param l the list.
 * @param i the node index in the list.
 * @return
 */
struct node *list_get_ith(const struct list *l, size_t i) {
    if (l == NULL) return NULL;

    if (l->length != 0) {
        struct node *nodes[l->length];

        if (l->head != NULL) {
            nodes[0] = l->head;

            // Get all nodes from list.
            for (size_t n = 1; n < l->length; n++) {
                nodes[n] = nodes[n - 1]->next;
            }

            return nodes[i];
        }
    }

    return NULL;
}

/**
 * Cuts current list into two.
 *
 * @param l the list.
 * @param n the last node in list.
 * @return
 */
struct list *list_cut_after(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) return NULL;
    if (l->length < 2 || l->head == NULL) return NULL;

    struct node *nodes[l->length];

    nodes[0] = l->head;

    size_t node_position = 0;

    // Get all nodes from list.
    for (size_t i = 1; i < l->length; i++) {
        nodes[i] = nodes[i - 1]->next;

        if (nodes[i] == n) node_position = i + 1;
    }

    struct list *new_list = list_init();

    size_t new_list_length = 0;
    new_list->head = n->next;

    l->tail = n;
    n->next = NULL;

    // Move nodes from first list to the new list.
    for (size_t i = node_position; i < l->length; i++) {
        nodes[i]->list = new_list;

        new_list_length++;

        // Set tail and determine the lengths of both lists.
        if (i == l->length - 1) {
            new_list->tail = nodes[i];
            new_list->length = new_list_length;

            l->length = l->length - new_list_length;
        }
    }

    return new_list;
}
