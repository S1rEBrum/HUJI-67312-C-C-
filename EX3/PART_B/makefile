tweets: tweets_generator.o markov_chain.o linked_list.o
	gcc -Wall -Wextra -std=c99 tweets_generator.o markov_chain.o linked_list.o -o tweets_generator

make_snake: snakes_and_ladders.o markov_chain.o linked_list.o
	gcc -Wall -Wextra -Werror -std=c99 snakes_and_ladders.o markov_chain.o linked_list.o -o snakes_and_ladders

tweets_generator.o: tweets_generator.c
	gcc -Wall -Wextra -Werror -std=c99 -c tweets_generator.c -o tweets_generator.o

snakes_and_ladders.o: snakes_and_ladders.c
	gcc -Wall -Wextra -Werror -std=c99 -c snakes_and_ladders.c -o snakes_and_ladders.o

markov_chain.o: markov_chain.c
	gcc -Wall -Wextra -Werror -std=c99 -c markov_chain.c -o markov_chain.o

linked_list.o: linked_list.c
	gcc -Wall -Wextra -Werror -std=c99 -c linked_list.c -o linked_list.o


