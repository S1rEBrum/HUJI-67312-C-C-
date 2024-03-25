#include "cipher.h"
#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// numbers definitions
#define LINE_SIZE 1024
#define TEST_ARGS_NUM 2
#define ENCODE_ARGS_NUM 5
#define NUM_TESTS 10
#define BASE 10

// word definitions
#define TEST_ARG "test"
#define ENCODE "encode"
#define DECODE "decode"
#define BACKSLASH "/"

// error strings definitions
#define ARGS_NUM_ERROR  "The program receives 1 or 4 arguments only.\n"
#define TEST_ERROR "Usage: cipher test\n"
#define INVALID_COMMAND_ERROR "The given command is invalid.\n"
#define INVALID_VALUE_ERROR "The given shift value is invalid.\n"
#define INVALID_FILE_ERROR "The given file is invalid.\n"

// function to check if the argv[2] - the shift number is integer
int is_integer (const char *str)
{
  if (strlen (str) == 0)
  {
    return 0;
  }
  int i = 0;
  if (str[0] == '-' || str[0] == '+')
  {
    i = 1;
  }
  for (; str[i] != '\0'; i++)
  {
    if (str[i] < '0' || str[i] > '9')
    {
      return 0;
    }
  }
  return 1;
}

// function to check if the arguments are correct
int check_args (int argc, char *argv[])
{
  if (argc != TEST_ARGS_NUM && argc != ENCODE_ARGS_NUM)
  {
    fprintf (stderr, ARGS_NUM_ERROR);
    return EXIT_FAILURE;
  }
  if (argc == TEST_ARGS_NUM && strcmp (argv[1], TEST_ARG) != 0)
  {
    fprintf (stderr, TEST_ERROR);
    return EXIT_FAILURE;
  }
  if (argc == ENCODE_ARGS_NUM)
  {
    if (strcmp (argv[1], ENCODE) != 0 && strcmp (argv[1], DECODE) != 0)
    {
      fprintf (stderr, INVALID_COMMAND_ERROR);
      return EXIT_FAILURE;
    }
    if (!(is_integer (argv[2])))
    {
      fprintf (stderr, INVALID_VALUE_ERROR);
      return EXIT_FAILURE;

    }
    FILE *in;
    in = fopen (argv[3], "r");
    if (in == NULL)
    {
      fprintf (stderr, INVALID_FILE_ERROR);
      return EXIT_FAILURE;
    }
    if ((strchr (argv[3], '/')!=NULL) | (strchr (argv[4], '/')!=NULL))
    {
      fprintf (stderr, INVALID_FILE_ERROR);
      return EXIT_FAILURE;
    }
    fclose (in);
  }
  return EXIT_SUCCESS;
}

// function that interacts with the prompt: test
int tests ()
{
  int tests[NUM_TESTS] = {
      test_encode_non_cyclic_lower_case_positive_k (),
      test_encode_cyclic_lower_case_special_char_positive_k (),
      test_encode_non_cyclic_lower_case_special_char_negative_k (),
      test_encode_cyclic_lower_case_negative_k (),
      test_encode_cyclic_upper_case_positive_k (),
      test_decode_non_cyclic_lower_case_positive_k (),
      test_decode_cyclic_lower_case_special_char_positive_k (),
      test_decode_non_cyclic_lower_case_special_char_negative_k (),
      test_decode_cyclic_lower_case_negative_k (),
      test_decode_cyclic_upper_case_positive_k ()
  };

  for (int i = 0; i < NUM_TESTS; i++)
  {
    if (tests[i])
    {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

// write from one file to the other
void write_to_file(int k, char command[], FILE *in_file, FILE *out_file) {
  char line[LINE_SIZE + 1] = {0};
  while (fgets (line, LINE_SIZE + 1, in_file) != NULL)
  {
    if (strcmp (command, ENCODE) == 0)
    {
      encode (line, k);
      fputs (line, out_file);
    }
    else
    {
      decode (line, k);
      fputs (line, out_file);
    }
  }
}

// CLI function
int cli (char command[], char shift_num[], char in[], char out[])
{
  int k = strtol (shift_num, NULL, BASE);

  FILE *in_file;
  FILE *out_file;

  in_file = fopen (in, "r");
  out_file = fopen (out, "w");

  write_to_file (k, command, in_file, out_file);

  fclose (in_file);
  fclose (out_file);

  return EXIT_SUCCESS;
}

// main
int main (int argc, char *argv[])
{
  // check arguments
  if (check_args (argc, argv))
  {
    return EXIT_FAILURE;
  }

  // run tests
  if (argc == TEST_ARGS_NUM)
  {
    tests ();
  }

  // run CLI
  if (argc == ENCODE_ARGS_NUM)
  {
    cli (argv[1], argv[2], argv[3], argv[4]);
  }

  return EXIT_SUCCESS;
}
