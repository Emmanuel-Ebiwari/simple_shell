#include "shell.h"

/**
 * er_num - print number to stderr
 * @num: number to print. Should not be INT_MIN
 */
void er_num(int num)
{
	char buffer[10];
	int bufp = 10;

	while (num || bufp == 10)
	{
		buffer[--bufp] = num % 10 + '0';
		num /= 10;
	}
	write(STDERR_FILENO, buffer + bufp, 10 - bufp);
}

/**
 * er_puts - print string to stderr
 * @a: string
 */
void er_puts(char *a)
{
	int len = 0;

	while (a[len])
		len++;
	write(STDERR_FILENO, a, len);
}

/**
 * erro - print error message
 * @linenum: line number
 * @argv: name of shell executable
 * @com: command
 * @exstr: argv 1
 * @er_id: error id
 * Return: exit code
 */
int erro(int linenum, char *argv, char *com, char *exstr, int er_id)
{
	er_puts(argv);
	er_puts(": ");
	er_num(linenum);
	er_puts(": ");
	er_puts(com);
	er_puts(": ");
	if (er_id == 0)
	{
		er_puts("not found\n");
		return (127);
	}
	else if (er_id == 1)
	{
		er_puts("Permission denied\n");
		return (126);
	}
	else if (er_id == 2)
	{
		er_puts("Illegal number: ");
		er_puts(exstr);
		er_puts("\n");
		return (2);
	}
	else if (er_id == 3)
	{
		er_puts("Usage: ");
		er_puts(com);
		er_puts(" ");
		er_puts(exstr);
		er_puts("\n");
		return (1);
	}
	else if (er_id == 4)
	{
		er_puts("can't cd to ");
		er_puts(exstr);
		er_puts("\n");
		return (2);
	}
	return (0);
}
