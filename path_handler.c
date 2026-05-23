#include "shell.h"

/**
 * _getenv - Custom getenv function to find environment variables
 * @name: Name of the variable to search for (e.g., "PATH")
 * Return: Pointer to the value string, or NULL if not found
 */
char *_getenv(const char *name)
{
	int i = 0;
	size_t len = strlen(name);

	while (environ[i] != NULL)
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
		{
			return (environ[i] + len + 1);
		}
		i++;
	}
	return (NULL);
}

/**
 * find_path - Resolves short commands into their full executable PATH
 * @command: Simple command name (e.g., "ls")
 * Return: Full path string on success, or a copy of command if not found
 */
char *find_path(char *command)
{
	char *path_env, *path_copy, *token, *full_path;
	struct stat st;

	/* If command already contains a slash, check it directly */
	if (strchr(command, '/') != NULL)
	{
		if (stat(command, &st) == 0)
			return (strdup(command));
		return (NULL);
	}
	path_env = _getenv("PATH");
	if (path_env == NULL || strlen(path_env) == 0)
		return (NULL);

	path_copy = strdup(path_env);
	token = strtok(path_copy, ":");
	while (token != NULL)
	{
		/* Allocate memory: folder length + '/' + command length + '\0' */
		full_path = malloc(strlen(token) + strlen(command) + 2);
		if (full_path == NULL)
		{
			free(path_copy);
			return (NULL);
		}
		sprintf(full_path, "%s/%s", token, command);
		if (stat(full_path, &st) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
