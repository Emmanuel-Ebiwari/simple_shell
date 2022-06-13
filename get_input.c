#include "shell.h"

/**
 * myhandle - signal handler
 * @mysignal: signal
 */
void myhandle(int mysignal)
{
	if (mysignal == SIGINT)
		er_puts("\n$ ");
}

/**
 * get_input - read a line of input
 * Return: pointer to input data (nul-terminated)
 * returns NULL when end of input is reached
 */
char *get_input(void)
{
	char *b = NULL;
	size_t num = 0;
	ssize_t length;
	int i = 0;
	static int exit_next_time;

	if (isatty(STDIN_FILENO))
		er_puts("$ ");

	if (exit_next_time)
	{
		free(b);
		if (isatty(STDIN_FILENO))
			er_puts("\n");
		return (NULL);
	}

	length = getline(&b, &num, stdin);

	/* end of input (or allocation failed)*/
	if (length == -1)
	{
		free(b);
		fflush(stdout);
		if (isatty(STDIN_FILENO))
			er_puts("\n");
		return (NULL);
	}

	/* Check if input didn't end in \n */
	for (i = 0; b[i]; i++)
		;
	if (i > 0 && b[i - 1] != '\n')
		exit_next_time = 1;

	return (b);
}
