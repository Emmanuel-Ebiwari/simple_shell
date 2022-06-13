#include "shell.h"

/**
 * printenv - prints the enviroment
 * @var_list: variables
 */
void printenv(VarList *var_list)
{
	VarList *item = var_list;

	while ((item = item->next))
	{
		_puts(item->name);
		pchar('=');
		_puts(item->value);
		pchar('\n');
	}
}

/**
 * executive - runs the programs after the search_path
 * @args: the tokenized arguments from getline
 * @file_path: the file path of the program
 * @var_list: the VarList
 * Return: the status of the fork
 */
int executive(char **args, char *file_path, VarList *var_list)
{
	pid_t parchild;
	int status;
	char **envp;

	envp = make_envp(var_list);

	parchild = fork();
	if (parchild < 0)
		return (-1);

	if (parchild == 0)
	{
		status = execve(file_path, args, envp);
		_exit(status);
	}

	wait(&status);
	free_envp(envp);

	return (WEXITSTATUS(status));
}

/**
 * main - the main shell
 * @argc: the number of arguments
 * @argv: the arguments
 * @envp: the enviroment
 * Return: 0
 */
int main(int argc, char **argv, char **envp)
{
	char *input, **args, *file_path;
	VarList variables;
	int line_num = 0, status = 0, b;
	VarList *path_var;

	read_envp(&variables, envp);
	(void)argc;
	while (1)
	{
		line_num++;
		signal(SIGINT, myhandle);
		signal(SIGTSTP, myhandle);
		input = get_input();
		if (input == NULL) /* end of input reached */
		{
			free_list(&variables);
			return (status);
		}
		args = parse_input(input, &variables, status);
		if (args[0] == NULL)
			continue;
		b = run_builtins(args, argv[0], &variables, line_num, &status);
		if (!b)
		{ /*do_alias(args);*/
			path_var = get_variable(&variables, "PATH");
			b = search_path(args[0], path_var ? path_var->value : "", &file_path);
			if (b == 0)
				status = executive(args, file_path, &variables);
			else
				status = erro(line_num, argv[0], args[0], NULL, b - 1);
		}
		else if (b == -1)
		{
			free(input);
			free_list(&variables);
			return (status);
		}
		free(input);
	}
	return (0);
}
