#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 280
#define MAX_PEOPLE 550

/**
 * Function that adds a friend to the social network
 *
 * @param matrix - the adjacency matrix of the social network
 * @param ind1 - the index of the first person
 * @param ind2 - the index of the second person
*/
void add_friend(int ***matrix, int ind1, int ind2);

/**
 * Function that removes a friend from the social network
 *
 * @param matrix - the adjacency matrix of the social network
 * @param ind1 - the index of the first person
 * @param ind2 - the index of the second person
*/
void remove_friend(int ***matrix, int ind1, int ind2);

/**
 * Function that suggests friends for a person in the social network
 *
 * @param matrix - the adjacency matrix of the social network
 * @param ind1 - the index of the person
*/
void suggestions(int ***matrix, int ind1);

/**
 * Function that calculates the distance between two people
 * in the social network
 *
 * @param matrix - the adjacency matrix of the social network
 * @param ind1 - the index of the first person
 * @param ind2 - the index of the second person
 * @return the distance between the two people
*/
int distance(int ***matrix, int ind1, int ind2);

/**
 * Function that finds common friends between two people
 *
 * @param matrix - the adjacency matrix of the social network
 * @param ind1 - the index of the first person
 * @param ind2 - the index of the second person
*/
void common(int ***matrix, int ind1, int ind2);

/**
 * Function that finds the most popular friend of a person
 *
 * @param matrix - the adjacency matrix of the social network
 * @param ind1 - the index of the person
*/
void friends(int ***matrix, int ind1);

/**
 * Function that finds the most popular person in the social network
 *
 * @param matrix - the adjacency matrix of the social network
 * @param ind1 - the index of the person
*/
void popular(int ***matrix, int ind1);

/**
 * Function that handles the input for the friends task
 *
 * @param input - the input string
 * @param matrix - the adjacency matrix of the social network
*/
void handle_input_friends(char *input, int ***matrix);

#endif // FRIENDS_H
