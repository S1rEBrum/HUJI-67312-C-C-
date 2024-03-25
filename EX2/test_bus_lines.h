#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H

#include "sort_bus_lines.h"


int is_sorted (BusLine *start, BusLine *end, SortType sort_type);

int is_sorted_by_distance (BusLine *start, BusLine *end);

int is_sorted_by_duration (BusLine *start, BusLine *end);

int is_sorted_by_name (BusLine *start, BusLine *end);

int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);
#endif //EX2_REPO_TESTBUSLINES_H
