#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "queue.h"

#define QUEUE_SIZE 100000000

#define NOT_FOUND (-1)
#define ERROR (-2)

typedef struct maze {
    int n;
    int start_index;
    int finish_index;
    char *data;
} maze;

typedef struct queue {
    int *data;
    int pushes;
    int pops;
    int firstInLine;
    int length;
    int capacity;
} queue;

/**
 * Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occurred.
 *
 * @param m the maze.
 * @return
 */
int bfs_solve(struct maze *m) {
    if(m == NULL) return ERROR;

    struct queue *path = queue_init(QUEUE_SIZE);

    int locRow = maze_row(m, m->start_index);
    int locCol = maze_col(m, m->start_index);

    maze_set(m, locRow, locCol, VISITED);
    queue_push(path, maze_index(m, locRow, locCol));
    path->length++;

    int row_offsets[] = {-1, 1, 0, 0};
    int col_offsets[] = {0, 0, -1, 1};

    char direction[4];
    int choices = 0;
    int pathLength;

    for (int i = 0; i < QUEUE_SIZE; i++) {
        // Looking around for possible directions and the destination.
        for (int d = 0; d < N_MOVES; d++) {
            int newRow = locRow + row_offsets[d];
            int newCol = locCol + col_offsets[d];

            direction[d] = maze_get(m, newRow, newCol);

            if (maze_at_destination(m, newRow, newCol)) {
                pathLength = path->length;

                for (int p = 0; p < path->length; p++) {
                    if (queue_empty(path)) break;

                    locRow = maze_row(m, path->data[p]);
                    locCol = maze_col(m, path->data[p]);

                    path->data[p] = PATH;

                    maze_set(m, locRow, locCol, PATH);
                }

                queue_cleanup(path);

                return pathLength;
            }

            if (direction[d] == FLOOR) choices++;
        }

        // If there are multiple FLOOR choices to go to, add TO_VISIT.
        if (choices > 1) {
            for (int d = 0; d < N_MOVES; d++) {
                int newRow = locRow + row_offsets[d];
                int newCol = locCol + col_offsets[d];

                direction[d] = maze_get(m, newRow, newCol);

                if (direction[d] == FLOOR) {
                    maze_set(m, newRow, newCol, TO_VISIT);
                }
            }
        }

        // If FLOOR or TO_VISIT move to it.
        for (int d = 0; d < N_MOVES; d++) {
            if (direction[d] == FLOOR || direction[d] == TO_VISIT) {
                int newRow = locRow + row_offsets[d];
                int newCol = locCol + col_offsets[d];

                maze_set(m, newRow, newCol, VISITED);

                locRow = newRow;
                locCol = newCol;

                queue_push(path, maze_index(m, newRow, newCol));

                goto findNewDirection;
            }
        }

        // Reached dead end. Time to backtrack and look around for junction.
        for (int p = 0; p < path->length; p++) {
            int previousRow = maze_row(m, path->data[path->length]);
            int previousCol = maze_col(m, path->data[path->length]);

            for (int d = 0; d < N_MOVES; d++) {
                int newRow = locRow + row_offsets[d];
                int newCol = locCol + col_offsets[d];

                direction[d] = maze_get(m, newRow, newCol);

                if (direction[d] == TO_VISIT) {
                    queue_push(path, maze_index(m, previousRow, previousCol));

                    maze_set(m, newRow, newCol, VISITED);

                    locRow = newRow;
                    locCol = newCol;

                    queue_push(path, maze_index(m, locRow, locCol));

                    goto findNewDirection;
                }
            }

            queue_pop(path);

            locRow = maze_row(m, path->data[path->length]);
            locCol = maze_col(m, path->data[path->length]);
        }

        findNewDirection: continue;
    }

    queue_cleanup(path);

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
    int path_length = bfs_solve(m);

    if (path_length == ERROR) {
        printf("bfs failed\n");
        maze_cleanup(m);

        return 1;
    } else if (path_length == NOT_FOUND) {
        printf("no path found from start to destination\n");
        maze_cleanup(m);

        return 1;
    }

    printf("bfs found a path of length: %d\n", path_length);

    /* print maze */
    maze_print(m, false);
    maze_output_ppm(m, "out.ppm");

    maze_cleanup(m);
    return 0;
}
