#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "users.h"
#include "posts.h"

post *create_new_post(int user_id, const char *title)
{
	post *new_post = (post *)malloc(sizeof(post));
	static int current_post_id = 1;
	new_post->id = current_post_id++;
	if (title) {
		strncpy(new_post->title, title, MAX_TITLE_LENGTH);
		new_post->title[MAX_TITLE_LENGTH] = '\0';
	} else {
		new_post->title[0] = '\0';
	}
	new_post->user_id = user_id;
	new_post->like_count = 0;
	memset(new_post->liked_by, 0, sizeof(new_post->liked_by));
	memset(new_post->user_like_status, UNLIKED,
		   sizeof(new_post->user_like_status));
	new_post->parent = NULL;
	new_post->next = NULL;
	new_post->first_child = NULL;
	return new_post;
}

// Helper function to add a post to a list
void add_post_to_list(post **list, post *the_post)
{
	if (!(*list))
		*list = the_post;
	else
	{
		post *temp = *list;
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = the_post;
	}
}

// Function to create a new post
void create_post(char *username, char *title, post **all_posts)
{
	int user_id = get_user_id(username);
	if (user_id == -1)
	{
		printf("User %s not found.\n", username);
		return;
	}
	post *new_post = create_new_post(user_id, title);
	add_post_to_list(all_posts, new_post);
	printf("Created %s for %s\n", title, username);
}

// Helper function to find a post by its ID
post *find_post_by_id(post *list, int id)
{
	if (!list)
		return NULL;
	post *temp = list;
	while (temp)
	{
		if (temp->id == id)
			return temp;
		post *found_in_children = find_post_by_id(temp->first_child, id);
		if (found_in_children)
			return found_in_children;
		temp = temp->next;
	}
	return NULL;
}

// Function to repost an existing post
void repost(char *username, int post_id, int repost_id, post **all_posts)
{
	int user_id = get_user_id(username);
	if (user_id == -1)
	{
		printf("User %s not found.\n", username);
		return;
	}
	post *original_post = find_post_by_id(*all_posts, post_id);
	if (!original_post)
	{
		printf("Post with ID %d not found.\n", post_id);
		return;
	}
	post *target_post = original_post;
	if (repost_id != 0)
	{
		target_post = find_post_by_id(original_post->first_child, repost_id);
		if (!target_post)
		{
			printf("Repost with ID %d not found.\n", repost_id);
			return;
		}
	}
	post *new_repost = create_new_post(user_id, NULL);
	new_repost->parent = target_post;
	add_post_to_list(&target_post->first_child, new_repost);
	printf("Created repost #%d for %s\n", new_repost->id, username);
}

// Helper function to find a path in the tree
int find_path(post *root, int id, post **path, int *path_len)
{
	if (!root)
		return 0;
	path[*path_len] = root;
	(*path_len)++;
	if (root->id == id)
		return 1;
	post *temp = root->first_child;
	while (temp)
	{
		if (find_path(temp, id, path, path_len))
			return 1;
		temp = temp->next;
	}
	(*path_len)--;
	return 0;
}

// Helper function to find the common ancestor of two posts
post *find_common_ancestor(post *root, int id1, int id2)
{
	post *path1[100];
	post *path2[100];
	int path_len1 = 0, path_len2 = 0;
	if (!find_path(root, id1, path1, &path_len1) ||
		!find_path(root, id2, path2, &path_len2))
		return NULL;
	int i = 0;
	for (i = 0; i < path_len1 && i < path_len2; i++)
	{
		if (path1[i] != path2[i])
			break;
	}
	return path1[i - 1];
}

// Function to find the common repost between two reposts
int common_repost(int post_id, int repost_id1, int repost_id2, post **all_posts)
{
	post *original_post = find_post_by_id(*all_posts, post_id);
	if (!original_post)
	{
		printf("Post with ID %d not found.\n", post_id);
		return -1;
	}
	post *common_ancestor = find_common_ancestor(original_post,
												 repost_id1, repost_id2);
	if (common_ancestor)
	{
		printf("The first common repost of %d and %d is %d\n",
			   repost_id1, repost_id2, common_ancestor->id);
		return common_ancestor->id;
	}
	printf("No common repost found.\n");
	return -1;
}

// Function to like a post
void like_post(char *username, int post_id, int repost_id, post **all_posts)
{
	int user_id = get_user_id(username);
	if (user_id == -1)
	{
		printf("User %s not found.\n", username);
		return;
	}
	post *target_post = find_post_by_id(*all_posts, post_id);
	if (!target_post)
	{
		printf("Post with ID %d not found.\n", post_id);
		return;
	}
	if (repost_id != 0)
	{
		target_post = find_post_by_id(target_post->first_child, repost_id);
		if (!target_post)
		{
			printf("Repost with ID %d not found.\n", repost_id);
			return;
		}
	}
	// Determine the correct title to display
	const char *display_title = NULL;
	if (repost_id == 0) {
		display_title = target_post->title;
	} else if (target_post->title[0] != '\0') {
		display_title = target_post->title;
	} else {
		post *parent = target_post->parent;
		while (parent && parent->title[0] == '\0')
			parent = parent->parent;
		display_title = parent ? parent->title : "Unknown";
	}
	if (target_post->user_like_status[user_id] == LIKED)
	{
		target_post->user_like_status[user_id] = UNLIKED;
		target_post->like_count--;
		if (repost_id == 0)
			printf("User %s unliked post %s\n", username, target_post->title);
		else
			printf("User %s unliked repost %s\n", username, display_title);
	} else {
		target_post->user_like_status[user_id] = LIKED;
		target_post->like_count++;
		if (repost_id == 0)
			printf("User %s liked post %s\n", username, target_post->title);
		else
			printf("User %s liked repost %s\n", username, display_title);
	}
}

// Function to get the number of likes for a post
void get_likes(int post_id, int repost_id, post **all_posts)
{
	post *target_post = find_post_by_id(*all_posts, post_id);
	if (!target_post)
	{
		printf("Post with ID %d not found.\n", post_id);
		return;
	}
	if (repost_id != 0)
	{
		target_post = find_post_by_id(target_post->first_child, repost_id);
		if (!target_post)
		{
			printf("Repost with ID %d not found.\n", repost_id);
			return;
		}
	}
	if (repost_id == 0)
		printf("Post %s has %d likes\n",
			   target_post->title, target_post->like_count);
	else
		printf("Repost #%d has %d likes\n",
			   repost_id, target_post->like_count);
}

void delete_post_recursive(post *post)
{
	if (!post)
		return;
	delete_post_recursive(post->first_child);
	delete_post_recursive(post->next);
	free(post);
}

void delete_post(int post_id, int repost_id, post **all_posts)
{
	post *target_post = find_post_by_id(*all_posts, post_id);
	post *prev_post = NULL;
	post *temp = *all_posts;
	if (!target_post)
	{
		printf("Post with ID %d not found.\n", post_id);
		return;
	}
	if (repost_id != 0)
	{
		post *repost = target_post->first_child;
		post *prev_repost = NULL;
		while (repost && repost->id != repost_id)
		{
			prev_repost = repost;
			repost = repost->next;
		}
		if (repost) {
			if (!prev_repost)
				target_post->first_child = repost->next;
			else
				prev_repost->next = repost->next;
			delete_post_recursive(repost->first_child);
			free(repost);
			printf("Deleted repost #%d of post %s\n", repost_id,
				   target_post->title);
			return;
		}
		printf("Repost with ID %d not found.\n", repost_id);
	}
	while (temp && temp->id != post_id)
	{
		prev_post = temp;
		temp = temp->next;
	}
	if (!temp)
	{
		printf("Post with ID %d not found.\n", post_id);
		return;
	}
	if (!prev_post)
		*all_posts = temp->next;
	else
		prev_post->next = temp->next;
	delete_post_recursive(temp->first_child);
	free(temp);
}

void get_reposts_recursive(post *post)
{
	if (!post)
		return;
	printf("Repost #%d by %s\n", post->id, get_user_name(post->user_id));
	get_reposts_recursive(post->first_child);
	get_reposts_recursive(post->next);
}

void get_reposts(int post_id, int repost_id, post **all_posts)
{
	post *target_post = find_post_by_id(*all_posts, post_id);
	if (!target_post)
	{
		printf("Post with ID %d not found.\n", post_id);
		return;
	}
	if (repost_id != 0) {
		target_post = find_post_by_id(target_post->first_child, repost_id);
		if (!target_post) {
			printf("Repost with ID %d not found.\n", repost_id);
			return;
		}
		printf("Repost #%d by %s\n", target_post->id,
			   get_user_name(target_post->user_id));
	} else {
		printf("%s - Post by %s\n", target_post->title,
			   get_user_name(target_post->user_id));
	}
	get_reposts_recursive(target_post->first_child);
}

void handle_input_posts(char *input, post **all_posts)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, " ");
	if (!cmd)
	{
		free(commands);
		return;
	}
	if (!strcmp(cmd, "create"))
	{
		char *username = strtok(NULL, " ");
		char *title = strtok(NULL, "\n");
		if (username && title)
			create_post(username, title, all_posts);
		else
			printf("Invalid input for create command.\n");
	} else if (!strcmp(cmd, "repost")) {
		char *username = strtok(NULL, " ");
		int post_id = atoi(strtok(NULL, " "));
		char *repost_id_str = strtok(NULL, " ");
		int repost_id = repost_id_str ? atoi(repost_id_str) : 0;
		if (username)
			repost(username, post_id, repost_id, all_posts);
		else
			printf("Invalid input for repost command.\n");
	} else if (!strcmp(cmd, "common-repost")) {
		int post_id = atoi(strtok(NULL, " "));
		int repost_id1 = atoi(strtok(NULL, " "));
		int repost_id2 = atoi(strtok(NULL, " "));
		common_repost(post_id, repost_id1, repost_id2, all_posts);
	} else if (!strcmp(cmd, "like")) {
		char *username = strtok(NULL, " ");
		int post_id = atoi(strtok(NULL, " "));
		char *repost_id_str = strtok(NULL, " ");
		int repost_id = repost_id_str ? atoi(repost_id_str) : 0;
		like_post(username, post_id, repost_id, all_posts);
	} else if (!strcmp(cmd, "ratio")) {
		int post_id = atoi(strtok(NULL, " "));
		post *target_post = find_post_by_id(*all_posts, post_id);
		if (!target_post)
		{
			printf("Post with ID %d not found.\n", post_id);
			return;
		}
		post *max_repost = NULL;
		int max_likes = target_post->like_count;
		post *temp = target_post->first_child;
		while (temp)
		{
			if (temp->like_count > max_likes)
			{
				max_repost = temp;
				max_likes = temp->like_count;
			}
			temp = temp->next;
		}
		if (max_repost)
			printf("Post %d got ratio'd by repost %d\n",
				   post_id, max_repost->id);
		else
			printf("The original post is the highest rated\n");
	} else if (!strcmp(cmd, "delete")) {
		int post_id = atoi(strtok(NULL, " "));
		char *repost_id_str = strtok(NULL, " ");
		int repost_id = repost_id_str ? atoi(repost_id_str) : 0;
		delete_post(post_id, repost_id, all_posts);
	} else if (!strcmp(cmd, "get-likes")) {
		int post_id = atoi(strtok(NULL, " "));
		char *repost_id_str = strtok(NULL, " ");
		int repost_id = repost_id_str ? atoi(repost_id_str) : 0;
		get_likes(post_id, repost_id, all_posts);
	} else if (!strcmp(cmd, "get-reposts")) {
		int post_id = atoi(strtok(NULL, " "));
		char *repost_id_str = strtok(NULL, " ");
		int repost_id = repost_id_str ? atoi(repost_id_str) : 0;
		get_reposts(post_id, repost_id, all_posts);
	}
	free(commands);
}
