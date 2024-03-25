#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sort_bus_lines.h"
#include "test_bus_lines.h"

#define BUFFERSIZE 63
#define MAXLENGTH 21
#define POINTERERROR 1
#define NAMEERROR 2
#define DISTANCEERROR 3
#define DURATIONERROR 4
#define CHECKSUCCESS 0

#define ENTERLINESSTR "Enter number of lines. Then enter\n"
#define NUMLINESERROR "ERROR: please enter a positive integer \n"
#define INPUTERROR "Error reading input \n"
#define ENTERINFOSTR "Enter line info. Then enter\n"
#define USAGEERROR "USAGE: you need to put only one argument \n"

#define POINTERERRORMSG "ERROR: There are no lines available \n"
#define NAMEERRORMSG "ERROR: The name is invalid \n"
#define DISTANCEERRORMSG "ERROR: The distance parameter is invalid \n"
#define DURATIONERRORMSG "ERROR: The duration parameter is invalid \n"

#define BYDISTANCE "by_distance"
#define BYDURATION "by_duration"
#define BYNAME "by_name"
#define TEST "test"

#define TEST1 1
#define TEST2 2
#define TEST3 3
#define TEST4 4
#define TEST5 5
#define TEST6 6

#define TESTPASSED "TEST %d PASSED: "
#define TESTFAILED "TEST %d FAILED: "
#define DISTANCE "Sorted by distance \n"
#define DURATION "Sorted by duration \n"
#define NAME "Sorted by name \n"
#define NOTDISTANCE "Not sorted by distance \n"
#define NOTDURATION "Not sorted by duration \n"
#define NOTNAME "Not sorted by name \n"
#define EQUAL "The array has not changed \n"
#define NOTEQUAL "The array has changed \n"

#define BASE10 10
#define MAXDISTANCE 1000
#define MAXDURATION 100
#define MINDURATION 10

void print_line (BusLine *lines, int num_lines)
{
  for (int i = 0; i < num_lines; ++i)
  {
    printf ("%s,%d,%d\n",
            lines[i].name, lines[i].distance, lines[i].duration);
  }
}

int check_input (const char *name)
{
  for (int i = 0; (i <= MAXLENGTH) && (name[i] != '\0'); i++)
  {
    if (!((name[i] >= 'a' && name[i] <= 'z')
          || (name[i] >= '0' && name[i] <= '9')))
    {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int check_name_parameter (BusLine *line, char *token)
{
  if (!token)
  {
    return EXIT_FAILURE;
  }
  if (check_input (token))
  {
    return EXIT_FAILURE;
  }
  char *copy_check = strcpy (line->name, token);
  if (!copy_check)
  {
    return EXIT_FAILURE;
  }
  return CHECKSUCCESS;
}

int get_line_parameters (BusLine *line, char buffer[BUFFERSIZE])
{
  char *token = NULL;

  if (!line)
  {
    printf (POINTERERRORMSG);
    return POINTERERROR;
  }

  token = strtok (buffer, ",");
  if (check_name_parameter (line, token))
  {
    printf (NAMEERRORMSG);
    return NAMEERROR;
  }

  token = strtok (NULL, ",");
  int distance;
  distance = strtol (token, NULL, BASE10);
  if ((distance < 0) || (distance > MAXDISTANCE))
  {
    printf (DISTANCEERRORMSG);
    return DISTANCEERROR;
  }
  line->distance = distance;

  token = strtok (NULL, ",");
  int duration = strtol (token, NULL, BASE10);
  if ((duration < MINDURATION) || (duration > MAXDURATION))
  {
    printf (DURATIONERRORMSG);
    return DURATIONERROR;
  }
  line->duration = duration;

  return CHECKSUCCESS;
}

int get_lines_input (char *lines_buffer)
{
  int num;
  while (1)
  {
    printf (ENTERLINESSTR);
    if (fgets (lines_buffer, sizeof (lines_buffer), stdin) == NULL)
    {
      printf (INPUTERROR);
      return 0;
    }

    if (sscanf (lines_buffer, "%d", &num) == 1 && num > 0)
    {
      break;
    }
    else
    {
      printf (NUMLINESERROR);
    }
  }
  return num;
}

int get_parameters_input (BusLine *lines, char *buffer, int num_lines)
{
  for (int i = 0; i < num_lines; ++i)
  {
    printf (ENTERINFOSTR);

    if (fgets (buffer, BUFFERSIZE, stdin) == NULL)
    {
      printf (INPUTERROR);
      free(lines);
      lines = NULL;
      return EXIT_FAILURE;
    }

    size_t length = strlen (buffer);
    if (length > 0 && buffer[length - 1] == '\n')
    {
      buffer[length - 1] = '\0';
    }

    BusLine new_line;
    int parse_result = get_line_parameters (&new_line, buffer);

    if (parse_result == 0)
    {
      lines[i] = new_line;

    }
    else
    {
      --i;
    }
  }
  return EXIT_SUCCESS;
}

int check_if_equal (BusLine *line_copy, BusLine *lines, int num_lines)
{
  if (is_equal (line_copy,
                line_copy + num_lines - 1,
                lines,
                lines + num_lines - 1))
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

int check_if_sorted_by_distance (BusLine *line, int num_lines)
{
  if (is_sorted_by_distance (line, line + num_lines - 1))
  {
    printf (TESTPASSED, TEST1);
    printf (DISTANCE);
  }
  else
  {
    printf (TESTFAILED, TEST1);
    printf (NOTDISTANCE);
    return 0;
  }
  return 1;
}

int check_if_sorted_by_duration (BusLine *line, int num_lines)
{
  if (is_sorted_by_duration (line, line + num_lines - 1))
  {
    printf (TESTPASSED, TEST3);
    printf (DURATION);
  }
  else
  {
    printf (TESTFAILED, TEST3);
    printf (NOTDURATION);
    free (line);
    return 0;
  }
  return 1;
}

int check_if_sorted_by_name (BusLine *line, int num_lines)
{
  if (is_sorted_by_name (line, line + num_lines - 1))
  {
    printf (TESTPASSED, TEST5);
    printf (NAME);
  }
  else
  {
    printf (TESTFAILED, TEST5);
    printf (NOTNAME);
    free (line);
    return 0;
  }
  return 1;
}

int check_if_is_equal (BusLine *line, int num_lines, int test)
{
  if (!check_if_equal (line, line, num_lines))
  {
    printf (TESTPASSED, test);
    printf (EQUAL);
  }
  else
  {
    printf (TESTFAILED, test);
    printf (NOTEQUAL);
    free (line);
    return 0;
  }
  return 1;
}

void test_mode (BusLine *lines, int num_lines)
{
  BusLine *line_copy;
  line_copy = calloc (num_lines, sizeof (BusLine));
  line_copy = memcpy(line_copy, lines, num_lines * sizeof (BusLine));
  check_if_sorted_by_distance (line_copy, num_lines);
  check_if_is_equal (line_copy, num_lines, TEST2);
  check_if_sorted_by_duration (line_copy, num_lines);
  check_if_is_equal (line_copy, num_lines, TEST4);
  check_if_sorted_by_name (line_copy, num_lines);
  check_if_is_equal (line_copy, num_lines, TEST6);
  free(line_copy);
  line_copy = NULL;
}

int do_tests (char *param, BusLine *lines, int num_lines)
{
  if (strcmp (param, BYDISTANCE) == 0)
  {
    quick_sort (lines, lines + num_lines - 1, 0);
    print_line (lines, num_lines);
  }
  else if (strcmp (param, BYDURATION) == 0)
  {
    quick_sort (lines, lines + num_lines - 1, 1);
    print_line (lines, num_lines);
  }
  else if (strcmp (param, BYNAME) == 0)
  {
    bubble_sort (lines, lines + num_lines - 1);
    print_line (lines, num_lines);
  }
  else if (strcmp (param, TEST) == 0)
  {
    test_mode (lines, num_lines);
  }
  else

  {
    printf (USAGEERROR);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int main (int argc, char *argv[])
{
  int num_lines = 0;
  BusLine *lines;

  char buffer[BUFFERSIZE];
  char lines_buffer[MAXLENGTH];

  if (argc == 1 || argc > 2) {
    printf(USAGEERROR);
    return EXIT_FAILURE;
  }

  int temp = get_lines_input (lines_buffer);
  if (temp)
  {
    num_lines = temp;
  }

  lines = (BusLine *) malloc ((num_lines) * sizeof (BusLine));

  if (lines == NULL) {
    free(lines);
    lines = NULL;
    return EXIT_FAILURE;
  }

  get_parameters_input (lines, buffer, num_lines);

  do_tests (argv[1], lines, num_lines);

  free (lines);
  lines = NULL;
  return EXIT_SUCCESS;
}