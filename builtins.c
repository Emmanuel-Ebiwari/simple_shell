#include "shell.h"

/**
 * parse_number - read a string containing a non-negative number
 * @a: string
 * Return: number, or -1 if @a is not a valid number
 */
int parse_number(char *a)
{
	int i, num = 0;

	if (a == NULL)
		return (-1);
	for (i = 0; a[i]; i++)
	{
		if (a[i] >= '0' && a[i] <= '9')
			num = num * 10 + (a[i] - '0');
		else
			return (-1);
	}
	return (num);
}

/**
 * builtin_setenv - setenv builtin command
 * @args: arguments (command is args[0])
 * @shell: name of shell executable
 * @var_list: environment variables
 * @lnum: line number
 * Return: exit code
 */
int builtin_setenv(char **args, char *shell, VarList *var_list, int lnum)
{
	if (!args[1] || !args[2] || args[3])
	{
		return (erro(lnum, shell, args[0], "VARIABLE VALUE", 3));
	}
	set_variable(var_list, args[1], args[2]);
	return (0);
}

/**
 * builtin_unsetenv - unsetenv builtin command
 * @args: arguments (command is args[0])
 * @shell: name of shell executable
 * @var_list: environment variables
 * @lnum: line number
 * Return: exit code
 */
int builtin_unsetenv(char **args, char *shell, VarList *var_list, int lnum)
{
	if (!args[1] || args[2])
	{
		return (erro(lnum, shell, args[0], "VARIABLE", 3));
	}
	remove_variable(var_list, args[1]);
	return (0);
}

/**
 * run_builtins - try to run command as a builtin
 * @args: arguments (command is args[0])
 * @shell: name of shell executable
 * @var_list: environment variables
 * @lnum: line number
 * @ec: exit code output
 * Return: 0 (no builtin), 1 (builtin ran), -1 (exit)
 */
int run_builtins(char **args,
								 char *shell, VarList *var_list, int lnum, int *ec)
{
	int i = 0;

	if (!_strcmp(args[0], "setenv"))
	{
		*ec = builtin_setenv(args, shell, var_list, lnum);
		return (1);
	}
	if (!_strcmp(args[0], "unsetenv"))
	{
		*ec = builtin_unsetenv(args, shell, var_list, lnum);
		return (1);
	}
	if (!_strcmp(args[0], "env"))
	{
		*ec = 0;
		printenv(var_list);
		return (1);
	}
	if (!_strcmp(args[0], "exit"))
	{
		if (!args[1])
			return (-1);
		i = parse_number(args[1]);
		if (i >= 0)
		{
			*ec = i;
			return (-1);
		}
		*ec = erro(lnum, shell, args[0], args[1], 2);
		return (1);
	}
	if (!_strcmp(args[0], "cd"))
	{
		*ec = cdfunction(args, var_list);
		if (*ec < 0)
			erro(lnum, shell, args[0], args[1], 4);
		return (1);
	}
	return (0);
}
/**
 * cdfunction - cd builtin
 * @args: the arguments
 * @var_list: env variables
 * Return: cdstatus
 */
int cdfunction(char **args, VarList *var_list)
{
	int cdstat;
	char *file_path, path[PATH_MAX];
	VarList *var;

	var = get_variable(var_list, "PWD");
	if (var)
		set_variable(var_list, "OLDPWD", var->value);
	else
	{
		getcwd(path, PATH_MAX);
		set_variable(var_list, "OLDPWD", path);
	}
	if (args[1] == NULL)
	{
		var = get_variable(var_list, "HOME");
		if (var)
			cdstat = chdir(var->value);
	}
	else if (!_strcmp(args[1], "-"))
	{
		file_path = get_variable(var_list, "OLDPWD")->value;
		er_puts(file_path);
		er_puts("\n");
		cdstat = chdir(file_path);
	}
	else
		cdstat = chdir(args[1]);
	getcwd(path, PATH_MAX);
	set_variable(var_list, "PWD", path);
	return (cdstat);
}
