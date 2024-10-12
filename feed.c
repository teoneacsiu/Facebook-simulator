#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "friends.h"
#include "posts.h"
#include "feed.h"
#include "users.h"

void feed(char *username, int feed_size, int **matrix, post *all_posts)
{
	post *aux = all_posts;
	int total_posts = 0;
	while (aux)
	{
		total_posts++;
		aux = aux->next;
	}
	post **post_array = (post **)malloc(total_posts * sizeof(post *));
	aux = all_posts;
	for (int i = 0; i < total_posts; i++)
	{
		post_array[i] = aux;
		aux = aux->next;
	}
	int user_id = get_user_id(username);
	int cnt = 0;
	for (int i = total_posts - 1; i >= 0 && cnt < feed_size; i--)
	{
		post *post = post_array[i];
		if (matrix[user_id][post->user_id] == 1 || user_id == post->user_id)
		{
			printf("%s: %s\n", get_user_name(post->user_id), post->title);
			cnt++;
		}
	}
	free(post_array);
}

void view_profile(char *username, post *all_posts)
{
	post *aux = all_posts;
	int user_id = get_user_id(username);
	while (aux) {
		if (aux->user_id == user_id)
			printf("Posted: %s\n", aux->title);
		aux = aux->next;
	}
	aux = all_posts;
	while (aux) {
		post *child = aux->first_child;
		while (child) {
			if (child->user_id == user_id) {
				printf("Reposted: %s\n", aux->title);
				break;
			}
			child = child->next;
		}
		aux = aux->next;
	}
}

void friends_repost(char *username, char *cmd, int **matrix, post *all_posts)
{
	int user_id = get_user_id(username);
	int post_id = atoi(cmd);
	post *aux = all_posts;
	while (aux)
	{
		if (aux->id == post_id)
		{
			post *child = aux->first_child;
			while (child)
			{
				int friend_id = child->user_id;
				if (matrix[user_id][friend_id] == 1)
				{
					printf("%s\n", get_user_name(friend_id));
				}
				child = child->next;
			}
			break;
		}
		aux = aux->next;
	}
}

int *get_friends(int user_id, int **matrix, int *num_friends)
{
	int *friends = (int *)malloc(MAX_PEOPLE * sizeof(int));
	*num_friends = 0;
	for (int i = 0; i < MAX_PEOPLE; i++)
	{
		if (matrix[user_id][i] == 1)
		{
			friends[(*num_friends)++] = i;
		}
	}
	return friends;
}

int is_clique(int **matrix, int *users, int num_users)
{
	for (int i = 0; i < num_users; i++)
	{
		for (int j = i + 1; j < num_users; j++)
		{
			if (matrix[users[i]][users[j]] != 1)
			{
				return 0;
			}
		}
	}
	return 1;
}

int compare_ints(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

void common_groups(char *username, int **matrix)
{
	int user_id = get_user_id(username);
	int num_friends;
	int *friends = get_friends(user_id, matrix, &num_friends);
	int *clique = (int *)malloc((num_friends + 1) * sizeof(int));
	int max_clique_size = 0;
	// Add the user to the initial clique
	clique[0] = user_id;
	max_clique_size = 1;
	// Try all subsets of friends to find the largest clique
	for (int i = 1; i < (1 << num_friends); i++)
	{
		int *temp_clique = (int *)malloc((num_friends + 1) * sizeof(int));
		int clique_size = 1;
		temp_clique[0] = user_id;
		for (int j = 0; j < num_friends; j++)
		{
			if (i & (1 << j))
				temp_clique[clique_size++] = friends[j];
		}
		if (is_clique(matrix, temp_clique, clique_size) &&
			clique_size > max_clique_size)
		{
			max_clique_size = clique_size;
			memcpy(clique, temp_clique, max_clique_size * sizeof(int));
		}
		free(temp_clique);
	}
	qsort(clique, max_clique_size, sizeof(int), compare_ints);
	printf("The closest friend group of %s is:\n", username);
	for (int i = 0; i < max_clique_size; i++)
		printf("%s\n", get_user_name(clique[i]));
	free(friends);
	free(clique);
}

void handle_input_feed(char *input, int ***matrix, post **all_posts)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	if (!cmd)
		return;
	if (!strcmp(cmd, "feed"))
	{
		cmd = strtok(NULL, "\n ");
		char *username = cmd;
		cmd = strtok(NULL, "\n ");
		int feed_size = atoi(cmd);
		feed(username, feed_size, *matrix, *all_posts);
	} else if (!strcmp(cmd, "view-profile")) {
		cmd = strtok(NULL, "\n ");
		view_profile(cmd, *all_posts);
	} else if (!strcmp(cmd, "friends-repost")) {
		cmd = strtok(NULL, "\n ");
		char *username = cmd;
		cmd = strtok(NULL, "\n ");
		friends_repost(username, cmd, *matrix, *all_posts);
	} else if (!strcmp(cmd, "common-group")) {
		cmd = strtok(NULL, "\n ");
		common_groups(cmd, *matrix);
	}
	free(commands);
}
