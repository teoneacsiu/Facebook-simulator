/**
 * The entrypoint of the homework. Every initialization must be done here
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"

/**
 * Initializez every task based on which task we are running
 */
void init_tasks(void)
{
#ifdef TASK_1

#endif

#ifdef TASK_2

#endif

#ifdef TASK_3

#endif
}

/**
 * Entrypoint of the program, compiled with different defines for each task
 */
int main(void)
{
	init_users();
	init_tasks();
	char *result = NULL, *input = (char *)malloc(MAX_COMMAND_LEN);
	int **matrix = malloc(MAX_PEOPLE * sizeof(int *));
	for (int i = 0; i < MAX_PEOPLE; i++)
		matrix[i] = calloc(MAX_PEOPLE, sizeof(int));
	post *all_posts = NULL;
	while (1)
	{
		result = fgets(input, MAX_COMMAND_LEN, stdin);
		// If fgets returns null, we reached EOF
		if (!result)
			break;
#ifdef TASK_1
		handle_input_friends(input, &matrix);
#endif

#ifdef TASK_2
		handle_input_posts(input, &all_posts);
#endif

#ifdef TASK_3
		handle_input_feed(input, &matrix, &all_posts);
#endif
	}
	for (int i = 0; i < MAX_PEOPLE; i++)
		free(matrix[i]);
	free(matrix);
	delete_post_recursive(all_posts);
	free(input);
	free_users();
	return 0;
}
