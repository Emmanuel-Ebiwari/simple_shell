#include "shell.h"

/**
 * _strlen - get length of string
 * @s: string
 * Return: length
 */
size_t _strlen(char *s)
{
	size_t len = 0;

	if (s)
		while (*s++)
			len++;
	return (len);
}

/**
 * _memcpy - copy memory
 * @dest: destination
 * @source: source
 * @length: number of bytes
 */
void _memcpy(char *dest, char *source, size_t length)
{
	while (length-- > 0)
		*dest++ = *source++;
}

/**
 * read_envp - read envp into a VarList
 * @var_list: head of list (should have no items)
 * @envp: envp
 *  This reads envp (the list passed into the main function)
 *  and copies its variables into a varlist
 */
void read_envp(VarList *var_list, char **envp)
{
	char *item;
	VarList *tail = var_list;
	size_t name_len, value_len;

	var_list->length = 0;
	while (*envp)
	{
		item = *envp;
		/* create node */
		tail = tail->next = safe_malloc(sizeof(VarList));
		/* separate "NAME=VALUE" string into name and value */
		for (name_len = 0; item[name_len] != '='; name_len++)
			if (item[name_len] == '\0') /* found invalid item */
				continue;
		for (value_len = 0; item[name_len + 1 + value_len]; value_len++)
			;
		/* copy name/value into varlist */
		tail->name = safe_malloc(name_len + 1);
		_memcpy(tail->name, item, name_len);
		tail->name[name_len] = '\0';
		tail->value = safe_malloc(value_len + 1);
		_memcpy(tail->value, item + name_len + 1, value_len + 1);
		/* update length and move to next item */
		var_list->length++;
		envp++;
	}
	tail->next = NULL;
}

/**
 * make_envp - generates an array of strings, from a var list
 * @var_list: head of var list
 * Return: envp
 *  Each value in envp is a string, of the form "NAME=VALUE",
 *  except the final item, which is NULL.
 *  The same as the 3rd argument to main, and execve.
 */
char **make_envp(VarList *var_list)
{
	char **envp = safe_malloc((var_list->length + 1) * sizeof(char *));
	char **curr = envp;
	size_t name_len, value_len;
	char *env_item;

	while ((var_list = var_list->next))
	{
		/* get length of name/value */
		name_len = _strlen(var_list->name);
		value_len = _strlen(var_list->value);
		/* allocate item and copy data into it */
		env_item = safe_malloc(name_len + 1 + value_len + 1);
		_memcpy(env_item, var_list->name, name_len);
		env_item[name_len] = '=';
		_memcpy(env_item + name_len + 1, var_list->value, value_len);
		env_item[name_len + 1 + value_len] = '\0';
		/* put item in list */
		*curr = env_item;
		curr++;
	}
	*curr = NULL;
	return (envp);
}

/**
 * free_envp - free the memory allocated by make_envp
 * @envp: envp
 *  THIS MUST BE CALLED AFTER EVERY make_envp
 */
void free_envp(char **envp)
{
	char **curr;

	for (curr = envp; *curr; curr++)
	{
		free(*curr);
	}
	free(envp);
}
