#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "users.h"

void add_friend(int ***matrix, int ind1, int ind2)
{
	(*matrix)[ind1][ind2] = 1;
		(*matrix)[ind2][ind1] = 1;
		printf("Added connection %s - %s\n",
			   get_user_name(ind1), get_user_name(ind2));
}

void remove_friend(int ***matrix, int ind1, int ind2)
{
	(*matrix)[ind1][ind2] = 0;
	(*matrix)[ind2][ind1] = 0;
	printf("Removed connection %s - %s\n",
		   get_user_name(ind1), get_user_name(ind2));
}

void suggestions(int ***matrix, int ind1)
{
	int freq[MAX_PEOPLE] = {0};
	int ind2 = 0, i, j;
	for (i = 0; i < MAX_PEOPLE; i++)
		if ((*matrix)[ind1][i] == 1)
			for (j = 0; j < MAX_PEOPLE; j++)
				if ((*matrix)[i][j] == 1 && j != ind1 &&
					(*matrix)[ind1][j] == 0 && freq[j] == 0) {
					ind2 = 1;
					freq[j] = 1;
				}
	if (ind2 == 0)
		printf("There are no suggestions for %s\n", get_user_name(ind1));
	else
	{
		printf("Suggestions for %s:\n", get_user_name(ind1));
		for (j = 0; j < MAX_PEOPLE; j++)
			if (freq[j] == 1)
				printf("%s\n", get_user_name(j));
	}
}

int distance(int ***matrix, int ind1, int ind2)
{
	int *visited = (int *)calloc(MAX_PEOPLE, sizeof(int));
	int *queue = (int *)calloc(MAX_PEOPLE, sizeof(int));
	int *dist = (int *)calloc(MAX_PEOPLE, sizeof(int));
	int i, j, front = 0, rear = 0;
	for (i = 0; i < MAX_PEOPLE; i++)
		dist[i] = -1;
	queue[rear++] = ind1;
	visited[ind1] = 1;
	dist[ind1] = 0;
	while (front < rear)
	{
		i = queue[front++];
		for (j = 0; j < MAX_PEOPLE; j++)
			if ((*matrix)[i][j] == 1 && visited[j] == 0)
			{
				queue[rear++] = j;
				visited[j] = 1;
				dist[j] = dist[i] + 1;
			}
	}
	free(visited);
	free(queue);
	j = dist[ind2];
	free(dist);
	return j;
}

void common(int ***matrix, int ind1, int ind2)
{
	int i, j = 0;
	for (i = 0; i < MAX_PEOPLE; i++)
		if ((*matrix)[ind1][i] == 1 && (*matrix)[ind2][i] == 1)
			j = 1;
	if (j == 0)
		printf("No common friends for %s and %s\n",
			   get_user_name(ind1), get_user_name(ind2));
	else
	{
		printf("The common friends between %s and %s are:\n",
			   get_user_name(ind1), get_user_name(ind2));
		for (i = 0; i < MAX_PEOPLE; i++)
			if ((*matrix)[ind1][i] == 1 && (*matrix)[ind2][i] == 1)
				printf("%s\n", get_user_name(i));
	}
}

void friends(int ***matrix, int ind1)
{
	int i, nr = 0;
	for (i = 0; i < MAX_PEOPLE; i++)
		if ((*matrix)[ind1][i] == 1)
			nr++;
	printf("%s has %d friends\n", get_user_name(ind1), nr);
}

void popular(int ***matrix, int ind1)
{
	int i, j, nr, maxim = 0, aux = 0, ind2;
		for (i = 0; i < MAX_PEOPLE; i++)
			if ((*matrix)[ind1][i] == 1)
			{
				aux++;
				nr = 0;
				for (j = 0; j < MAX_PEOPLE; j++)
					if ((*matrix)[i][j] == 1)
						nr++;
				if (maxim < nr)
				{
					maxim = nr;
					ind2 = i;
				}
			}
		if (aux >= maxim)
			printf("%s is the most popular\n", get_user_name(ind1));
		else
			printf("%s is the most popular friend of %s\n",
			       get_user_name(ind2), get_user_name(ind1));
}

void handle_input_friends(char *input, int ***matrix)
{
	int ind1 = 0, ind2 = 0, nr;
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	if (!cmd)
		return;
	if (!strcmp(cmd, "add")) {
		cmd = strtok(NULL, "\n ");
		ind1 = get_user_id(cmd);
		cmd = strtok(NULL, "\n ");
		ind2 = get_user_id(cmd);
		add_friend(matrix, ind1, ind2);
	} else if (!strcmp(cmd, "remove")) {
		cmd = strtok(NULL, "\n ");
		ind1 = get_user_id(cmd);
		cmd = strtok(NULL, "\n ");
		ind2 = get_user_id(cmd);
		remove_friend(matrix, ind1, ind2);
	} else if (!strcmp(cmd, "suggestions")) {
		cmd = strtok(NULL, "\n ");
		ind1 = get_user_id(cmd);
		suggestions(matrix, ind1);
	} else if (!strcmp(cmd, "distance")) {
		cmd = strtok(NULL, "\n ");
		ind1 = get_user_id(cmd);
		cmd = strtok(NULL, "\n ");
		ind2 = get_user_id(cmd);
		nr = distance(matrix, ind1, ind2);
		if (nr == -1)
			printf("There is no way to get from %s to %s\n",
				   get_user_name(ind1), get_user_name(ind2));
		else
			printf("The distance between %s - %s is %d\n",
			       get_user_name(ind1), get_user_name(ind2), nr);
	} else if (!strcmp(cmd, "common")) {
		cmd = strtok(NULL, "\n ");
		ind1 = get_user_id(cmd);
		cmd = strtok(NULL, "\n ");
		ind2 = get_user_id(cmd);
		common(matrix, ind1, ind2);
	} else if (!strcmp(cmd, "friends")) {
		cmd = strtok(NULL, "\n ");
		ind1 = get_user_id(cmd);
		friends(matrix, ind1);
	} else if (!strcmp(cmd, "popular")) {
		cmd = strtok(NULL, "\n ");
		ind1 = get_user_id(cmd);
		popular(matrix, ind1);
	}
	free(commands);
}
