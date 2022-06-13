#include "shell.h"

/**
 * push_arg - push an argument to the list
 * @start: pointer to start of arg string
 * @end: pointer to char after end of arg
 * @arglist: arglist
 * @quote: if inside quote
 * @argi: pointer to arg index
 * @var_list: variables
 * @num: exit code
 * Return: if comment encountered
 */
int push_arg(char *start, char *end, char **arglist, int quote, int *argi,
						 VarList *var_list, int num)
{
	static char num_buffer[10];
	int bufp = 10;

	if (!quote && *start == '#')
		return (1);
	*end = '\0';
	if (*start == '$' && start[1])
	{
		if (start[1] == '?' && !start[2])
		{
			num_buffer[--bufp] = '\0';
			while (num || bufp == 9)
			{
				num_buffer[--bufp] = num % 10 + '0';
				num /= 10;
			}
			start = num_buffer + bufp;
		}
		else
		{
			var_list = get_variable(var_list, start + 1);
			if (var_list)
				start = var_list->value;
			else
				start = NULL;
		}
	}
	if (start)
	{
		arglist[*argi] = start;
		(*argi)++;
	}
	return (0);
}

/**
 * parse_input - parse input command
 * @input: input
 * @var_list: env vars
 * @code: exit code
 * Return: array of argument strings
 */
char **parse_input(char *input, VarList *var_list, int code)
{
	static char *arglist[10000]; /* realy big lol */
	int argi = 0; /* number of args */
	int quote = 0; /* if inside quoted section */
	char *start = NULL; /* start of arg being parsed */
	char c;

	(void)var_list;
	for (; *input; input++)
	{
		c = *input;
		if (((c == ' ' || c == '\n') && !quote) || (c == '"' && quote))
		{
			if (start)
			{
				if (push_arg(start, input, arglist, quote, &argi, var_list, code))
					break;
				start = NULL;
			}
			quote = 0;
		}
		else if (c == '"')
		{
			quote = 1;
			if (!start)
				start = input + 1;
		}
		else
			if (!start)
				start = input;
	}
	if (start)
		push_arg(start, input, arglist, quote, &argi, var_list, code);
	arglist[argi] = NULL;
	return (arglist);
}

/**
 * print_args - print arg list (for debug)
 * @args: arg list
 */
void print_args(char **args)
{
	int i;

	for (i = 0; args[i]; i++)
		printf("Arg %d: `%s`\n", i, args[i]);
}
