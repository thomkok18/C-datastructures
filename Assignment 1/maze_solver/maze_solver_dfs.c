#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "stack.h"

#define STACK_SIZE 100000000

#define NOT_FOUND (-1)
#define ERROR (-2)

typedef struct maze {
    int n;
    int start_index;
    int finish_index;
    char *data;
} maze;

typedef struct stack {
    int *data;
    int pushes;
    int pops;
    int size;
    int capacity;
} stack;

/**
 * Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occurred.
 *
 * @param m the maze.
 * @return
 */
int dfs_solve(struct maze *m) {
    if(m == NULL) return ERROR;

    struct stack *path = stack_init(STACK_SIZE);

    int loc_row = maze_row(m, m->start_index);
    int loc_col = maze_col(m, m->start_index);

    maze_set(m, loc_row, loc_col, VISITED);
    stack_push(path, maze_index(m, loc_row, loc_col));

    int row_offsets[] = {-1, 1, 0, 0};
    int col_offsets[] = {0, 0, -1, 1};

    char direction[4];
    int choices = 0;
    int path_length;

    for (int i = 0; i < STACK_SIZE; i++) {
        // Looking around for possible directions and the destination.
        for (int d = 0; d < N_MOVES; d++) {
            int new_row = loc_row + row_offsets[d];
            int new_col = loc_col + col_offsets[d];

            direction[d] = maze_get(m, new_row, new_col);

            if (maze_at_destination(m, new_row, new_col)) {
                path_length = path->size;

                for (int p = 0; p < path->size; p++) {
                    if (stack_empty(path)) break;

                    loc_row = maze_row(m, path->data[p]);
                    loc_col = maze_col(m, path->data[p]);

                    path->data[p] = PATH;

                    maze_set(m, loc_row, loc_col, PATH);
                }

                stack_cleanup(path);

                return path_length;
            }

            if (direction[d] == FLOOR) choices++;
        }

        // If there are multiple FLOOR choices to go to, add TO_VISIT.
        if (choices > 1) {
            for (int d = 0; d < N_MOVES; d++) {
                int new_row = loc_row + row_offsets[d];
                int new_col = loc_col + col_offsets[d];

                direction[d] = maze_get(m, new_row, new_col);

                if (direction[d] == FLOOR) {
                    maze_set(m, new_row, new_col, TO_VISIT);
                }
            }
        }

        // If FLOOR or TO_VISIT move to it.
        for (int d = 0; d < N_MOVES; d++) {
            if (direction[d] == FLOOR || direction[d] == TO_VISIT) {
                int new_row = loc_row + row_offsets[d];
                int new_col = loc_col + col_offsets[d];

                maze_set(m, new_row, new_col, VISITED);

                loc_row = new_row;
                loc_col = new_col;

                stack_push(path, maze_index(m, new_row, new_col));

                goto find_new_direction;
            }
        }

        // Reached dead end. Time to backtrack and look around for junction.
        for (int p = 0; p < path->size; p++) {
            int previous_row = maze_row(m, path->data[path->size]);
            int previous_col = maze_col(m, path->data[path->size]);

            for (int d = 0; d < N_MOVES; d++) {
                int new_row = loc_row + row_offsets[d];
                int new_col = loc_col + col_offsets[d];

                direction[d] = maze_get(m, new_row, new_col);

                if (direction[d] == TO_VISIT) {
                    stack_push(path, maze_index(m, previous_row, previous_col));

                    maze_set(m, new_row, new_col, VISITED);

                    loc_row = new_row;
                    loc_col = new_col;

                    stack_push(path, maze_index(m, loc_row, loc_col));

                    goto find_new_direction;
                }
            }

            stack_pop(path);

            loc_row = maze_row(m, path->data[path->size]);
            loc_col = maze_col(m, path->data[path->size]);
        }

        find_new_direction: continue;
    }

    stack_cleanup(path);

    return NOT_FOUND;
}

/**
 * Tries to display the maze and showing the path to destination.
 *
 * @return
 */
int main(void) {
    /* read maze */
    struct maze *m = maze_read();

    if (!m) {
        printf("Error reading maze\n");

        return 1;
    }

    /* solve maze */
    int path_length = dfs_solve(m);

    if (path_length == ERROR) {
        printf("dfs failed\n");
        maze_cleanup(m);

        return 1;
    } else if (path_length == NOT_FOUND) {
        printf("no path found from start to destination\n");
        maze_cleanup(m);

        return 1;
    }

    printf("dfs found a path of length: %d\n", path_length);

    /* print maze */
    maze_print(m, false);
    maze_output_ppm(m, "out.ppm");

    maze_cleanup(m);
    return 0;
}
