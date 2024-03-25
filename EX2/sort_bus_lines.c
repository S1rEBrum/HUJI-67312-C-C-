#include "sort_bus_lines.h"

/**
 * Swap helper function:
 * swaps two elements using temporary variable
 */
void swap (BusLine *a, BusLine *b)
{
  BusLine temp = *a;
  *a = *b;
  *b = temp;
}

/**
 * Compare helper function:
 * given two lines and sorting type,
 * returns the comparison between values:
 * distance or duration accordingly
 */
int compare (BusLine *a, BusLine *b, SortType sort_type)
{
  switch (sort_type)
  {
    case DISTANCE:
      return a->distance - b->distance;
    case DURATION:
      return a->duration - b->duration;
    default:
      return 0;  // Unsupported SortType, no specific ordering
  }
}

/**
 * Partition helper function:
 * Takes two pointers i and j (low and high)
 * Pointer i will move forward and find the first element > pivot
 * Pointer j will move backward and find the first element < pivot
 * after swapping, continue till j<i
 * return the partition index

 */
BusLine *partition (BusLine *start, BusLine *end, SortType sort_type)
{
  BusLine *pivot = end;
  BusLine *i = start - 1;

  for (BusLine *j = start; j < end; j++)
  {
    if (compare (j, pivot, sort_type) < 0)
    {
      i++;
      swap (i, j);
    }
  }

  swap (i + 1, end);
  return i + 1;
}

/**
 * Quick sort algorithm:
 * divide-and-conquer type of algorithm
 * selecting pivot point,
 * divide the array to two sub arrays and continue
 * dividing till all the sub-arrays are sorted
 */
void quick_sort (BusLine *start, BusLine *end, SortType sort_type)
{
  if (start < end)
  {
    BusLine *pivot = partition (start, end, sort_type);

    quick_sort (start, pivot - 1, sort_type);
    quick_sort (pivot + 1, end, sort_type);
  }
}

/**
 * Bubble sort algorithm:
 * go over all the elements in 2 loops,
 * check if the adjacent elements are sorted,
 * if not - swap and repeat until the full array is sorted
 */

void bubble_sort (BusLine *start, BusLine *end)
{
  for (BusLine *i = start; i < end; ++i)
  {
    for (BusLine *j = start; j < end; ++j)
    {
      if (strcmp (j->name, (j + 1)->name) > 0)
      {
        BusLine temp;
        memcpy(&temp, j, sizeof (BusLine));
        memcpy(j, j + 1, sizeof (BusLine));
        memcpy(j + 1, &temp, sizeof (BusLine));
      }
    }
  }
}

