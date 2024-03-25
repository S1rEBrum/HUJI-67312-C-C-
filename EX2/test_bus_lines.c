#include "test_bus_lines.h"
#include "sort_bus_lines.h"

#include <stdlib.h>

/**
 * Helper function:
 * checks if the array is sorted accordingly to sort type (distance or duration),
 * using compare function from sort_bus_lines.c
 */
int is_sorted (BusLine *start, BusLine *end, SortType sort_type)
{
  for (BusLine *current = start; current < end - 1; current++)
  {
    if (compare (current, current + 1, sort_type) > 0)
    {
      return 0;
    }
  }
  return 1;
}

/**
 * Test function:
 * checks if the array is sorted by DISTANCE using quick sort
 */
int is_sorted_by_distance (BusLine *start, BusLine *end)
{
  quick_sort (start, end - 1, DISTANCE);
  return is_sorted (start, end, DISTANCE);
}

/**
 * Test function:
 * checks if the array is sorted by DURATION using quick sort
 */
int is_sorted_by_duration (BusLine *start, BusLine *end)
{
  quick_sort (start, end - 1, DURATION);
  return is_sorted (start, end, DURATION);
}

/**
 * Test function:
 * checks if the array is sorted by NAME using bubble sort
 */
int is_sorted_by_name (BusLine *start, BusLine *end)
{
  bubble_sort (start, end);
  for (BusLine *i = start; i < end - 1; ++i)
  {
    if (strcmp (i->name, (i + 1)->name) > 0)
    {
      return 0;
    }
  }
  return 1;
}

/**
 * Test function:
 * checks if the array has changed.
 * The definition of array that changed
 * is that the NAMES of the bus lines changed
 * (the duration and distance can change
 * and that does not interest us in this particular function)
 */

int is_equal (BusLine *start_sorted, BusLine *end_sorted,
              BusLine *start_original, BusLine *end_original)
{
  if (end_sorted - start_sorted != end_original - start_original)
  {
    return 0;  // Arrays have different lengths
  }
  int *name_counts_sorted = (int *) calloc (
      end_sorted - start_sorted, sizeof (int));

  for (BusLine *sorted = start_sorted; sorted < end_sorted; sorted++)
  {
    for (BusLine *original = start_original;
         original < end_original; original++)
    {
      if (name_counts_sorted[sorted - start_sorted] == 0
          && strcmp (sorted->name, original->name) == 0)
      {
        // Match found, increment count and break
        name_counts_sorted[sorted - start_sorted]++;
        break;
      }
    }
  }
  for (BusLine *original = start_original; original < end_original; original++)
  {
    int count = 0;
    for (BusLine *sorted = start_sorted; sorted < end_sorted; sorted++)
    {
      if (strcmp (sorted->name, original->name) == 0)
      {
        count++;
      }
    }

    if (count != name_counts_sorted[original - start_original])
    {
      free (name_counts_sorted);
      return 0;
    }
  }

  free (name_counts_sorted);
  return 1;
}