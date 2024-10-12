#ifndef POSTS_H
#define POSTS_H

#include <stdbool.h>

#define LIKED 1
#define UNLIKED 0
#define MAX_TITLE_LENGTH 280
#define MAX_USERS 1000

typedef struct post
{
	int id;
	char title[MAX_TITLE_LENGTH + 1];
	int user_id;
	int like_count;
	// Array to store user IDs who liked the post
	int liked_by[MAX_USERS];
	// Array to store like status for each user (LIKED or UNLIKED)
	int user_like_status[MAX_USERS];
	// Pointer to the parent post (for reposts)
	struct post *parent;
	// Pointer to the next post in the list
	struct post *next;
	// Pointer to the first child (repost) of this post
	struct post *first_child;
} post;

/**
 * @brief Creates a new post.
 * @param user_id The ID of the user creating the post.
 * @param title The title of the post.
 * @return A pointer to the newly created Post structure.
 */
post *create_new_post(int user_id, const char *title);

/**
 * @brief Adds a post to the list of all posts.
 * @param list The list of posts.
 * @param the_post The post to be added.
 */
void add_post_to_list(post **list, post *the_post);

/**
 * @brief Creates a new post with the given username and title.
 * @param username The username of the user creating the post.
 * @param title The title of the post.
 * @param all_posts Pointer to the first post in the linked list of all posts.
 */
void create_post(char *username, char *title, post **all_posts);

/**
 * @brief Finds a post by its ID.
 * @param list The list of posts.
 * @param id The ID of the post to be found.
 * @return A pointer to the found Post structure, or NULL if not found.
 */
post *find_post_by_id(post *list, int id);

/**
 * @brief Creates a repost of a post.
 * @param username The username of the user creating the repost.
 * @param post_id The ID of the original post.
 * @param repost_id The ID of the repost (if any) to be reposted.
 * @param all_posts Pointer to the first post in the linked list of all posts.
 */
void repost(char *username, int post_id, int repost_id, post **all_posts);

/**
 * @brief Finds the path from the root to the post with the given ID.
 * @param root The root post.
 * @param id The ID of the target post.
 * @param path The array to store the path.
 * @param path_len The length of the path.
 * @return 1 if the path is found, 0 otherwise.
 */
int find_path(post *root, int id, post **path, int *path_len);

/**
 * @brief Finds the common ancestor of two posts.
 * @param root The root post.
 * @param id1 The ID of the first post.
 * @param id2 The ID of the second post.
 * @return A pointer to the common ancestor post, or NULL if not found.
 */
post *find_common_ancestor(post *root, int id1, int id2);

/**
 * @brief Finds the first common repost of two reposts.
 * @param post_id The ID of the original post.
 * @param repost_id1 The ID of the first repost.
 * @param repost_id2 The ID of the second repost.
 * @param all_posts Pointer to the first post in the linked list of all posts.
 * @return The ID of the first common repost, or -1 if not found.
 */
int common_repost(int post_id, int repost_id1, int repost_id2,
				  post **all_posts);

/**
 * @brief Likes or unlikes a post or repost.
 * @param username The username of the user liking the post.
 * @param post_id The ID of the original post.
 * @param repost_id The ID of the repost (if any).
 * @param all_posts Pointer to the first post in the linked list of all posts.
 */
void like_post(char *username, int post_id, int repost_id, post **all_posts);

/**
 * @brief Gets the number of likes of a post or repost.
 * @param post_id The ID of the original post.
 * @param repost_id The ID of the repost (if any).
 * @param all_posts Pointer to the first post in the linked list of all posts.
 */
void get_likes(int post_id, int repost_id, post **all_posts);

/**
 * @brief Recursively deletes a post and all its reposts.
 * @param post The post to be deleted.
 */
void delete_post_recursive(post *post);

/**
 * @brief Deletes a post or repost.
 * @param post_id The ID of the original post.
 * @param repost_id The ID of the repost (if any).
 * @param all_posts Pointer to the first post in the linked list of all posts.
 */
void delete_post(int post_id, int repost_id, post **all_posts);

/**
 * @brief Recursively prints all reposts of a post.
 * @param post The post whose reposts are to be printed.
 */
void get_reposts_recursive(post *post);

/**
 * @brief Gets all reposts of a post or repost.
 * @param post_id The ID of the original post.
 * @param repost_id The ID of the repost (if any).
 * @param all_posts Pointer to the first post in the linked list of all posts.
 */
void get_reposts(int post_id, int repost_id, post **all_posts);

/**
 * @brief Handles the input commands for posts.
 * @param input The input command string.
 * @param all_posts Pointer to the first post in the linked list of all posts.
 */
void handle_input_posts(char *input, post **all_posts);

#endif // POSTS_H
