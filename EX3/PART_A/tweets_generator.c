#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "markov_chain.h"

#define ARGSAMOUNT 5
#define BASE 10

#define READMODE "r"
#define SPACE " \r\n"
#define FULLSTOP '.'
#define TWEET "Tweet "

#define OPEN_FILE_ERROR "Error: could not open the file. \n"
#define USAGE_ERROR "Usage: the arguments must be: seed, tweets number, " \
"words to read, path to file and words to read (optional)."

/**
 * Function that prints tweets
 * @param markov_chain: Markov chain where the data is saved
 * @param count: amount of tweets that needs to be created
 */
void print_tweet (MarkovChain *markov_chain, int count) {
  for (int tnum = 0; tnum < count; tnum++) {
    printf (TWEET);
    printf ("%d: ", tnum + 1);
    generate_random_sequence (markov_chain,
                              NULL,
                              MAXWORDLEN);
    printf ("\n");
  }
}

/**
 * Function that checks if the amount of the arguments given is correct
 * @param arg: argc of the program
 * @return:  1 if the arguments amount is wrong, 0 if 4 or 5
 */
int check_args (int arg) {
  if (arg != ARGSAMOUNT - 1 && arg != ARGSAMOUNT) {
    fprintf (stdout, USAGE_ERROR);
    return FAILURE;
  }
  return SUCCESS;
}

/**
 * Function that decides to read the full text or only part of it
 * @param arg1: argc
 * @param arg2: argv[]
 * @return: -1 if read all the file, amount of words if given
 */
size_t get_words_to_read (int arg1, char *arg2) {
  size_t r;
  if (arg1 == ARGSAMOUNT) {
    r = (size_t) strtol (arg2, NULL, BASE);
  }
  if (arg1 == ARGSAMOUNT - 1) {
    r = -1;
  }
  return r;
}

/**
 * Function that instantiates the seed
 * @param arg: the seed parameter from argv[]
 */
void create_seed (char *arg) {
  size_t seed = strtol (arg, NULL, BASE);  // get the seed
  srand (seed); // randomize seed
}

/**
 * Function that tries to open the file given
 * @param f: pointer to pointer to file
 * @param filename: the filename from the argv[]
 * @return: 1 if did not succeed in opening the file, 0 otherwise
 */
int open_file (FILE **f, char *filename) {
  *f = fopen (filename, READMODE);
  if (*f == NULL) // if did not succeed
  {
    fprintf (stdout, OPEN_FILE_ERROR);  // print error and exit
    fclose (*f);
    return FAILURE;
  }
  return SUCCESS;
}

/**
 * Function that creates empty Markov chain and instantiates the database
 * field
 * @return: the created Markov chain
 */
MarkovChain *create_chain () {
  MarkovChain *chain = calloc (1, sizeof (struct MarkovChain));
  if (chain == NULL) {
    fprintf (stdout, ALLOCATION_ERROR_MESSAGE);
    free_markov_chain (&chain);
    chain = NULL;
    return NULL;
  }
  LinkedList *list = calloc (1, sizeof (struct LinkedList));
  if (list == NULL) {
    fprintf (stdout, ALLOCATION_ERROR_MESSAGE);
    free (list);
    list = NULL;
    return NULL;
  }
  chain->database = list;
  return chain;
}

/**
 * Function that fill the database (Linked List) with the words from the
 * file given
 * @param fp: pointer to file from which the words are read
 * @param words_to_read: amount of words to read (-1 if all, exact number if
 * it is given)
 * @param markov_chain: instantiated Markov chain to which the data is saved
 * @return: 1 if did not succeed, 0 otherwise
 */
int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain) {
  char input[MAXSENTENCELEN + 1] = {0};
  char *word = NULL, *next = NULL;
  Node *node1 = NULL, *node2 = NULL;
  int words_read = 0;
  while (fgets (input, MAXSENTENCELEN + 1, fp) != NULL) {
    word = strtok (input, SPACE);
    ++words_read;
    while (word != NULL) {
      node1 = add_to_database (markov_chain, word);
      if (node1 == NULL) {
        free_markov_chain (&markov_chain);
        return FAILURE;
      }
      if (words_read == words_to_read) {
        return SUCCESS;
      }
      next = strtok (NULL, SPACE);
      ++words_read;
      if (next != NULL) {
        node2 = add_to_database (markov_chain, next);
        if (node2 == NULL) {
          free_markov_chain (&markov_chain);
          return FAILURE;
        }
        if (node1->data->data[strlen (word) - 1] != FULLSTOP) {
          add_node_to_counter_list (node1->data, node2->data);
        }
        node1 = node2;
        word = next;
        if (words_read == words_to_read) {
          return SUCCESS;
        }
      }
      else {
        if (words_read == words_to_read) {
          return SUCCESS;
        }
        node1 = node2;
        word = next;
      }
    }
  }
  return SUCCESS;
}

/**
 * Main function: reads the arguments, checks them for validity, opens the
 * file, instantiates the database and saves it into Markov chain and then
 * generates and prints the wanted amount of tweets
 * @param argc: amount of parameters given to the program
 * @param argv: the parameters
 * @return: 1 if the program did not succeed, 0 otherwise
 */
int main (int argc, char *argv[]) {
  // check arguments amount
  if (check_args (argc) == FAILURE) {
    return EXIT_FAILURE;
  }
  // get the amount of words to read
  size_t words_to_read = get_words_to_read (argc,
                                            argv[ARGSAMOUNT - 1]);
  create_seed (argv[1]);
  FILE *f = NULL;
  if (open_file (&f, argv[3]) == FAILURE) // try to open the file
  {
    fclose (f);
    return EXIT_FAILURE;
  }
  MarkovChain *markov_chain = create_chain ();
  if (markov_chain == NULL) {
    free_markov_chain (&markov_chain);
    fclose (f);
    return EXIT_FAILURE;
  }
  if (fill_database (f,
                     (int) words_to_read, markov_chain) == FAILURE) {
    free_markov_chain (&markov_chain);
    fclose (f);
    return EXIT_FAILURE;
  }
  int count = (int) strtol (argv[2], NULL, BASE);
  print_tweet (markov_chain, count);
  free_markov_chain (&markov_chain);
  fclose (f);
  return EXIT_SUCCESS;
}

