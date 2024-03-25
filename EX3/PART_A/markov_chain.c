#include "markov_chain.h"
#include "linked_list.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool
#include <string.h>

#define ALLOCATION_ERROR_MESSAGE "Allocation failure: Failed to allocate"\
            "new memory\n"

/**
 * Function that randomizes a number
 * @param max_number
 * @return
 */
int get_random_number (int max_number) {
  return rand () % max_number;
}

/**
 * Function that creates and instantiates new markov node
 * @param data_ptr
 * @return
 */
MarkovNode *create_new_node (char *data_ptr) {
  // copy the data_ptr
  char *data_cpy = calloc (1, MAXWORDLEN + 1);
  if (data_cpy == NULL) {
    fprintf (stdout, ALLOCATION_ERROR_MESSAGE);
    free (data_cpy);  // unsuccessful allocation
    data_cpy = NULL;
    return NULL;
  }
  strcpy(data_cpy, data_ptr);
  MarkovNode *new_node = calloc (1, sizeof (struct MarkovNode));
  if (new_node == NULL) {
    fprintf (stdout, ALLOCATION_ERROR_MESSAGE);
    free (new_node);  // unsuccessful allocation
    new_node = NULL;
    return NULL;
  }
  // initialize the node
  new_node->data = data_cpy;
  new_node->frequencies_list = NULL;
  new_node->frequencies_list_len = 0;
  return new_node;
}

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
MarkovNode *get_first_random_node (MarkovChain *markov_chain) {
  Node *curr;
  while (true) {
    curr = markov_chain->database->first;
    int r = get_random_number (markov_chain->database->size);
//    r %= markov_chain->database->size;
    for (int i = 0; i < r; i++) {
      curr = curr->next;
    }
    if (curr->data->data[strlen (curr->data->data) - 1] == '.') {
      continue;
    }
    else {
      break;
    }
  }
  return curr->data;
}

int count_occurrences (MarkovNode *node) {
  int count = 0;
  for (int i = 0; i < node->frequencies_list_len; ++i) {
    count += node->frequencies_list[i].frequency;
  }
  return count;
}

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr) {
  int occ = count_occurrences (state_struct_ptr);
  int r = get_random_number (occ);
  int i = -1;
  int counter = 0;
  while (counter <= r) {
    ++i;
    for (int j = 0; j < state_struct_ptr->frequencies_list[i].frequency; ++j) {
      counter += 1;
    }
  }
  i = ((i < 0) == 1) ? 0 : i;
  return state_struct_ptr->frequencies_list[i].markov_node;
}

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with,
 *                   if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_random_sequence (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length) {
  MarkovNode *node = first_node;
  if (node == NULL) {
    node = get_first_random_node (markov_chain);
  }
  int i = 0;
  while (true) {
    printf ("%s", node->data);
    if (node->data[strlen (node->data) - 1] == '.' || i == max_length) {
      break;
    }
    printf (" ");
    ++i;
    node = get_next_random_node (node);
  }
}

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_markov_chain (MarkovChain **ptr_chain) {
  // initialize the parameters to go over the chain
  MarkovChain *chain = *ptr_chain;
  Node *current = chain->database->first;
  Node *next = NULL;
  int size = chain->database->size;
  // start going over the Markov chain
  for (int i = 0; i < size; ++i) {
    next = current->next;
    free (current->data->data); // free the data field of MarkovNode
    current->data->data = NULL;
    free (current->data->frequencies_list); // free the freq list
    current->data->frequencies_list = NULL;
    free (current->data); // free the data field of the Node
    current->data = NULL;
    free (current);  // free the current Node
    current = next;
  }
  free (chain->database); // free the LinkedList database
  chain->database = NULL;
  free (chain); // free the chain itself
  chain = NULL;
  *ptr_chain = NULL;  // remove the pointer to the chain
}

/**
 * TODO: write func description
 * @param first_node
 * @param second_node
 * @return
 */
bool counter_list_check (MarkovNode *first_node,
                         MarkovNode *second_node,
                         int size) {
  for (int i = 0; i < size; ++i) {
    if (first_node->frequencies_list[i].markov_node == second_node) {
      first_node->frequencies_list[i].frequency += 1;
      return true;
    }
  }
  return false;
}

/**
 * Add the second markov_node to the counter list of the first markov_node.
 * If already in list, update it's counter value.
 * @param first_node
 * @param second_node
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool
add_node_to_counter_list (MarkovNode *first_node, MarkovNode *second_node) {
  int size = first_node->frequencies_list_len;
  if (counter_list_check (first_node, second_node, size) == true) {
    return true;
  }
  int new_size;
  new_size = (size + 1) * (sizeof (struct MarkovNodeFrequency));
  MarkovNodeFrequencyType temp;
  if (first_node->frequencies_list == NULL) {
    temp = calloc (1, new_size);
  }
  else {
    temp = realloc (first_node->frequencies_list,
                    new_size);
  }
  if (temp == NULL) {
    free (temp);
    temp = NULL;
    return false;
  }
  temp[size].frequency = 1;
  temp[size].markov_node = second_node;
  first_node->frequencies_list = temp;
  first_node->frequencies_list_len++;

  return true;
}

/**
* Check if data_ptr is in database. If so, return the markov_node wrapping
 * it in the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node *get_node_from_database (MarkovChain *markov_chain, char *data_ptr) {
  Node *current = markov_chain->database->first;
  while (current != NULL) {
    if (current->data->data != NULL) {
      if (strcmp (current->data->data, data_ptr) == 0) {
        return current;
      }
    }
    current = current->next;
  }
  return NULL;
}

/**
* If data_ptr in markov_chain, return it's node. Otherwise, create new
 * node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
Node *add_to_database (MarkovChain *markov_chain, char *data_ptr) {
  Node *current = get_node_from_database (markov_chain, data_ptr);
  // check if node is in database
  if (current != NULL) {
    return current; // if node is already in database -> return it
  }
  MarkovNode *new_node = create_new_node (data_ptr);  // create node
  if ((new_node == NULL) ||
      (add (markov_chain->database, new_node) == FAILURE)) {
    free (new_node);
    new_node = NULL;
    return NULL;
  }
  return markov_chain->database->last;  // return the added node
}

