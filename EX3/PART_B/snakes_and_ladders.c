#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY (-1)
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20

#define ARGSAMOUNT 3
#define BASE 10
#define USAGE_ERROR "Usage: the arguments must be: seed and path number. \n"
#define RANDOM_WALK "Random Walk "
#define REG_NODE_PRINT "[%d] -> "
#define END_NODE_PRINT "[%d]"
#define LADDER_NODE_PRINT "[%d]-ladder to %d -> "
#define SNAKE_NODE_PRINT "[%d]-snake to %d -> "

/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents
    // the jump of the ladder in case there is one from this square
    int snake_to;  // snake_to represents
    // the jump of the snake in case there is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

//---------------------------------------------------------
// SNAKES AND LADDERS HELPERS
#include <stdbool.h>

// declarations
static void snl_print (gen_data ptr);
static int snl_comp (gen_data ptr1, gen_data ptr2);
static void snl_free (gen_data ptr);
static gen_data snl_cpy (gen_data ptr);
static bool snl_last (gen_data ptr);

// functions
/**
 * print function for the cell struct data type
 * @param ptr
 */
static void snl_print (gen_data ptr) {
  Cell *c = (Cell *) ptr;
  if (c->ladder_to >= 0) {  // if there is ladder
    printf (LADDER_NODE_PRINT, c->number, c->ladder_to);
  }
  else if (c->snake_to >= 0) {  // if there is snake
    printf (SNAKE_NODE_PRINT, c->number, c->snake_to);
  }
  else if (c->number == BOARD_SIZE) {
    printf (END_NODE_PRINT, c->number);
  }
  else {  // regular cell
    printf (REG_NODE_PRINT, c->number);
  }
}

/**
 * comparison function for cell struct data type
 * @param ptr1
 * @param ptr2
 * @return
 */
static int snl_comp (gen_data ptr1, gen_data ptr2) {
  Cell *c1 = (Cell *) ptr1;
  Cell *c2 = (Cell *) ptr2;
  return (c1->number - c2->number);

}

/**
 * free function for cell struct data type
 * @param ptr
 */
static void snl_free (gen_data ptr) {
  if (ptr != NULL) {
    free (ptr);
  }
}

/**
 * Copy function for cell struct data type
 * @param ptr
 * @return
 */
static gen_data snl_cpy (gen_data ptr) {
  Cell *dest = calloc (1, sizeof (struct Cell));
  if (dest == NULL) {
    free (dest);
    dest = NULL;
    return NULL;
  }
  memcpy(dest, (Cell *) ptr, sizeof (struct Cell));
  return dest;
}

/**
 * Is last function for cell struct data type
 * @param ptr
 * @return
 */
static bool snl_last (gen_data ptr) {
  Cell *c1 = (Cell *) ptr;
  return (c1->number == BOARD_SIZE);
}
//---------------------------------------------------------

/**
 * Function to print the created path
 * @param markov_chain
 * @param count
 */
static void print_path (MarkovChain *markov_chain, int count) {
  MarkovNode *first = markov_chain->database->first->data;
  for (int tnum = 0; tnum < count; tnum++) {
    printf (RANDOM_WALK);
    printf ("%d: ", tnum + 1);
    generate_random_sequence (markov_chain,
                              first,
                              MAX_GENERATION_LENGTH - 1);
    printf ("\n");
  }
}

/**
* Function that creates empty Markov chain and instantiates all the fields
* field
* @return: the created Markov chain
*/
static MarkovChain *create_chain () {
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
  chain->copy_func = snl_cpy;
  chain->comp_func = snl_comp;
  chain->print_func = snl_print;
  chain->free_data = snl_free;
  chain->is_last = snl_last;
  return chain;
}

/**
 * Function that checks if the amount of the arguments given is correct
 * @param arg: argc of the program
 * @return:  1 if the arguments amount is wrong, 0 if it is 3
 */
static int check_args (int arg) {
  if (arg != ARGSAMOUNT) {
    fprintf (stdout, USAGE_ERROR);
    return FAILURE;
  }
  return SUCCESS;
}

/**
 * Function that instantiates the seed
 * @param arg: the seed parameter from argv[]
 */
static void create_seed (char *arg) {
  size_t seed = strtol (arg, NULL, BASE);  // get the seed
  srand (seed); // randomize seed
}

/** Error handler **/
static int handle_error (char *error_msg, MarkovChain **database) {
  printf ("%s", error_msg);
  if (database != NULL) {
    free_markov_chain (database);
  }
  return EXIT_FAILURE;
}

static int create_board (Cell *cells[BOARD_SIZE]) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    cells[i] = malloc (sizeof (Cell));
    if (cells[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free (cells[j]);
      }
      handle_error (ALLOCATION_ERROR_MESSAGE, NULL);
      return EXIT_FAILURE;
    }
    *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
  }

  for (int i = 0; i < NUM_OF_TRANSITIONS; i++) {
    int from = transitions[i][0];
    int to = transitions[i][1];
    if (from < to) {
      cells[from - 1]->ladder_to = to;
    }
    else {
      cells[from - 1]->snake_to = to;
    }
  }
  return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database (MarkovChain *markov_chain) {
  Cell *cells[BOARD_SIZE];
  if (create_board (cells) == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }
  MarkovNode *from_node = NULL, *to_node = NULL;
  size_t index_to;
  for (size_t i = 0; i < BOARD_SIZE; i++) {
    add_to_database (markov_chain, cells[i]);
  }

  for (size_t i = 0; i < BOARD_SIZE; i++) {
    from_node = get_node_from_database (markov_chain, cells[i])->data;

    if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY) {
      index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
      to_node = get_node_from_database (markov_chain, cells[index_to])
          ->data;
      add_node_to_counter_list (from_node, to_node, markov_chain);
    }
    else {
      for (int j = 1; j <= DICE_MAX; j++) {
        index_to = ((Cell *) (from_node->data))->number + j - 1;
        if (index_to >= BOARD_SIZE) {
          break;
        }
        to_node = get_node_from_database (markov_chain, cells[index_to])
            ->data;
        add_node_to_counter_list (from_node, to_node, markov_chain);
      }
    }
  }
  // free temp arr
  for (size_t i = 0; i < BOARD_SIZE; i++) {
    free (cells[i]);
  }
  return EXIT_SUCCESS;
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main (int argc, char *argv[]) {
  // check arguments amount
  if (check_args (argc) == FAILURE) {
    return EXIT_FAILURE;
  }
  create_seed (argv[1]);  // initialize seed
  int paths_to_generate = (int) strtol (argv[2], NULL, BASE);
  // TRy to the markov chain
  MarkovChain *markov_chain = create_chain ();
  if (markov_chain == NULL) {
    free_markov_chain (&markov_chain);
    return EXIT_FAILURE;
  }
  // try to fill the database
  if (fill_database (markov_chain) == FAILURE) {
    free_markov_chain (&markov_chain);
    return EXIT_FAILURE;
  }
  print_path (markov_chain, paths_to_generate);
  free_markov_chain (&markov_chain);
  return EXIT_SUCCESS;
}
