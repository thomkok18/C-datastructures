/*H**********************************************************************
* FILENAME: tree.c
*
* DESCRIPTION:
*   Binary search tree datastructure.
*
* PUBLIC FUNCTIONS:
*   void tree_dot( tree, filename )
*   int tree_check( tree )
*   struct tree *tree_init( turbo )
*   int tree_insert( tree, data )
*   int tree_find( tree, data )
*   int tree_remove( tree, data )
*   void tree_print( tree )
*   void tree_cleanup( tree )
*
* PRIVATE FUNCTIONS:
*   static node *make_node( .. )
*   static int print_tree_dot_r( root, dotf )
*   static void tree_print_in_order( tree, root )
*   static void nodes_cleanup( n )
*
* AUTHOR: Thom Kok (Student nr: 15316491)
*
* START DATE: 08-12-2023
*
*H*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

/* C files can be modified anywhere.
 * So you can change or remove these structs to suit your needs. */
struct tree {
    struct node *root;
    size_t height;
};

struct node {
    int data;
    struct node *lhs;
    struct node *rhs;
};

typedef struct node node;

/* Unique id's for numbering nodes in dot format. */
static int global_node_counter = 0;

/**
 * Make node and add data to it.
 *
 * @param data the data.
 * @return
 */
static node *make_node(int data) {
    struct node *n = malloc(sizeof(struct node));

    if (n == NULL) return NULL;

    n->data = data;
    n->lhs = NULL;
    n->rhs = NULL;

    return n;
}

/**
 * Print the tree.
 *
 * @param root the root.
 * @param dotf the file.
 * @return
 */
static int print_tree_dot_r(node *root, FILE *dotf) {
    int left_id, right_id, my_id = global_node_counter++;

    if (root == NULL) {
        fprintf(dotf, "    %d [shape=point];\n", my_id);
        return my_id;
    }

    fprintf(dotf, "    %d [color=%s label=\"%d\"]\n", my_id, "black", root->data);

    left_id = print_tree_dot_r(root->lhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"l\"]\n", my_id, left_id);

    right_id = print_tree_dot_r(root->rhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"r\"]\n", my_id, right_id);

    return my_id;
}

/**
 * Print a dot representation of the tree to filename.
 *
 * @param tree the tree.
 * @param filename the file.
 */
void tree_dot(const struct tree *tree, const char *filename) {
    if (tree->root == NULL) return;

    node *root = tree->root;
    global_node_counter = 0;
    FILE *dotf = fopen(filename, "w");

    if (!dotf) {
        printf("error opening file: %s\n", filename);
        exit(1);
    }

    fprintf(dotf, "digraph {\n");

    if (root) print_tree_dot_r(root, dotf);

    fprintf(dotf, "}\n");

    fclose(dotf);
}

/**
 * Check properties of the BST and balancing properties if turbo is set.
 *
 * @param tree the tree.
 * @return
 */
int tree_check(const struct tree *tree) {
    /* ... OPTIONALLY IMPLEMENT TREE CHECKING HERE ... */
    return 0;
}

/**
 * Reserve bytes in memory for tree.
 *
 * @param turbo
 * @return
 */
struct tree *tree_init(int turbo) {
    struct tree *t = malloc(sizeof(struct tree));

    if (t == NULL) return NULL;

    t->root = NULL;
    t->height = 0;

    return t;
}

/**
 * Insert data into the tree.
 *
 * @param tree the tree.
 * @param data the data.
 * @return
 */
int tree_insert(struct tree *tree, int data) {
    if (tree == NULL) return -1;

    // Check if data already exists in the tree.
    if (tree_find(tree, data)) return 1;

    struct node *n = make_node(data);

    if (tree->root == NULL) {
        tree->root = n;
        return 0;
    }

    struct node *current = tree->root;

    // Add node to the tree.
    while (1) {
        if (data < current->data) {
            if (current->lhs == NULL) {
                current->lhs = n;
                break;
            } else {
                current = current->lhs;
            }
        } else {
            if (current->rhs == NULL) {
                current->rhs = n;
                break;
            } else {
                current = current->rhs;
            }
        }
    }

    tree->height++;

    return 0;
}

/**
 * Find data in the tree.
 *
 * @param tree the tree.
 * @param data the data.
 * @return
 */
int tree_find(struct tree *tree, int data) {
    if (tree == NULL) return 0;

    struct node *current = tree->root;

    // Go through all children.
    while (current != NULL) {
        if (data == current->data) {
            return 1;
        } else if (data < current->data) {
            current = current->lhs;
        } else {
            current = current->rhs;
        }
    }

    return 0;
}

/**
 * Remove data from the tree.
 *
 * @param tree the tree.
 * @param data the data.
 * @return
 */
int tree_remove(struct tree *tree, int data) {
    if (tree == NULL) return 1;

    // Find the node to be deleted.
    struct node *parent = NULL;
    struct node *current = tree->root;

    // Find node with data in tree.
    while (current != NULL && current->data != data) {
        parent = current;

        if (data < current->data) {
            current = current->lhs;
        } else {
            current = current->rhs;
        }
    }

    if (current == NULL) return 1;

    if (current->lhs == NULL && current->rhs == NULL) { // Node with no children (leaf node).
        // Free node based on if it is the root, left child or right child.
        if (parent == NULL) {
            free(tree->root);

            tree->root = NULL;
        } else if (parent->lhs == current) {
            free(parent->lhs);

            parent->lhs = NULL;
        } else {
            free(parent->rhs);

            parent->rhs = NULL;
        }
    } else if (current->lhs == NULL || current->rhs == NULL) { // Node with one child.
        // Get existing child.
        struct node *child = (current->lhs != NULL) ? current->lhs : current->rhs;

        // Free node and move child to parent.
        if (parent == NULL) {
            free(tree->root);

            tree->root = child;
        } else if (parent->lhs == current) {
            free(parent->lhs);

            parent->lhs = child;
        } else {
            free(parent->rhs);

            parent->rhs = child;
        }
    } else { // Node with two children.
        // Find the smallest node in the right subtree.
        struct node *successor = current->rhs;
        struct node *successorParent = current;

        while (successor->lhs != NULL) {
            successorParent = successor;
            successor = successor->lhs;
        }

        current->data = successor->data;

        // Remove the successor node and replace it.
        if (successorParent->lhs == successor) {
            free(successorParent->lhs);

            successorParent->lhs = successor->rhs;
        } else {
            free(successorParent->rhs);

            successorParent->rhs = successor->rhs;
        }
    }

    tree->height--;

    return 0;
}

/**
 * Print the contents of the tree in sorted order in one line per item.
 *
 * @param tree the tree height.
 * @param root the root of the tree.
 */
static void tree_print_in_order(size_t height, struct node *root) {
    if (root != NULL) {
        tree_print_in_order(height, root->lhs);

        printf("%d", root->data);

        if (height != 0) {
            printf( "\n");
        }

        tree_print_in_order(height, root->rhs);
    }
}

/**
 * Print the contents of the tree in sorted order in one line per item.
 *
 * @param tree the tree.
 */
void tree_print(const struct tree *tree) {
    if (tree == NULL) return;
    if (tree->root == NULL) return;

    tree_print_in_order(tree->height, tree->root);
}

/**
 * Clean nodes and its children.
 *
 * @param n the node.
 */
static void nodes_cleanup(struct node *n) {
    if (n != NULL) {
        nodes_cleanup(n->lhs);
        nodes_cleanup(n->rhs);
        free(n);
    }
}

/**
 * Free the tree and the nodes.
 *
 * @param tree the tree.
 */
void tree_cleanup(struct tree *tree) {
    if (tree == NULL) return;

    nodes_cleanup(tree->root);
    free(tree);
}
