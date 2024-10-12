#ifndef FEED_H
#define FEED_H

/**
 * @brief Displays the feed of a given user.
 *
 * This function displays the most recent posts in the feed of a given user,
 * considering the feed size and the user's friends' posts.
 *
 * @param username The username of the user whose feed is to be displayed.
 * @param feed_size The maximum number of posts to display in the feed.
 * @param matrix A matrix representing the friendship connections between users.
 * @param all_posts Pointer to the first post in the linked list of all posts.
 */
void feed(char *username, int feed_size, int **matrix, post *all_posts);

/**
 * @brief Displays the profile of a given user.
 *
 * This function displays the posts made by the user and the posts reposted
 * by the user.
 *
 * @param username The username of the user whose profile is to be viewed.
 * @param all_posts Pointer to the first post in the linked list of all posts.
 */
void view_profile(char *username, post *all_posts);

/**
 * @brief Displays friends who reposted a specific post.
 *
 * This function displays the friends of a user who reposted a specific post.
 *
 * @param username The username of the user.
 * @param cmd The command containing the post ID.
 * @param matrix A matrix representing the friendship connections between users.
 * @param all_posts Pointer to the first post in the linked list of all posts.
 */
void friends_repost(char *username, char *cmd, int **matrix, post *all_posts);

/**
 * @brief Helper function to get the list of friends for a user.
 *
 * This function retrieves the list of friends for a given user.
 *
 * @param user_id The ID of the user.
 * @param matrix A matrix representing the friendship connections between users.
 * @param num_friends Pointer to an integer to store the number of friends.
 * @return Returns an array of user IDs representing the friends of the user.
 */
int *get_friends(int user_id, int **matrix, int *num_friends);

/**
 * @brief Helper function to check if a given user is in a clique.
 *
 * This function checks if a given set of users forms a clique based on
 * the friendship matrix.
 *
 * @param matrix A matrix representing the friendship connections between users.
 * @param users An array of user IDs representing the potential clique members.
 * @param num_users The number of users in the array.
 * @return Returns 1 if the users form a clique, otherwise returns 0.
 */
int is_clique(int **matrix, int *users, int num_users);

/**
 * @brief Helper function to compare two integers.
 *
 * This function is used for sorting integers in ascending order.
 *
 * @param a Pointer to the first integer.
 * @param b Pointer to the second integer.
 * @return Returns a negative value if a is less than b, a positive value if
 * a is greater than b, and 0 if they are equal.
 */
int compare_ints(const void *a, const void *b);

/**
 * @brief Finds the common friend group of a user.
 *
 * This function finds and displays the common friend group of a user.
 *
 * @param username The username of the user.
 * @param matrix A matrix representing the friendship connections
 * between users.
 */
void common_groups(char *username, int **matrix);

/**
 * @brief Parses and handles user input related to feed operations.
 *
 * This function parses the input command and calls corresponding
 * feed-related functions.
 *
 * @param input The input string containing the command.
 * @param matrix Pointer to the matrix representing the friendship connections
 * between users.
 * @param all_posts Pointer to the first post in the linked list of all posts.
 */
void handle_input_feed(char *input, int ***matrix, post **all_posts);

#endif // FEED_H
