#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H

#include <string.h>
#define NAME_LEN 21

typedef struct BusLine
{
    char name[NAME_LEN];
    int distance, duration;
} BusLine;
typedef enum SortType
{
    DISTANCE,
    DURATION
} SortType;

/**
 * Bubble sort algorithm
 */
void bubble_sort (BusLine *start, BusLine *end);

/**
 * Quick sort algorithm
 */
void quick_sort (BusLine *start, BusLine *end, SortType sort_type);

/**
 * Partition helper function
 */
BusLine *partition (BusLine *start, BusLine *end, SortType sort_type);

/**
 * Swap helper function
 */
void swap (BusLine *a, BusLine *b);

/**
 * Compare helper function
 */
int compare (BusLine *a, BusLine *b, SortType sort_type);

#endif //EX2_REPO_SORTBUSLINES_H
