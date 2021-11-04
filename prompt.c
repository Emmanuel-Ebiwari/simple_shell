#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * main - shell program
 *
 * @ac: argument count
 * @av: argument vector
 *
 * Return: 0
 */

int main(int ac, char *av[])
{
	(void)ac, (void)av;
	char *line = NULL;
	size_t len;

	do {
		printf("my_bash$ ");
		if (getline(&line, &len, stdin) == -1)
		{
			if (feof(stdin))
			{
				printf("\n");
				printf("EXIT_SUCCESS");
				printf("\n");
				exit(EXIT_SUCCESS);
			}
			else
			{
				perror("readline");
				exit(EXIT_FAILURE);
			}
		}
	}
	while (1)

	free(line);
	return (0);
}
