#include "shell.h"

/**
 * _getenv - Safely retrieves the value of an environment variable
 * @name: Name of the environment variable
 * Return: Pointer to the value string, or NULL if not found
 */
char *_getenv(const char *name)
{
	int i = 0;
	size_t len = strlen(name);

	if (name == NULL || environ == NULL)
		return (NULL);

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
 * find_path - Resolves a command name into its full executable path
 * @command: The command to resolve
 * Return: Allocated full path string on success, or NULL if not found
 */
char *find_path(char *command)
{
	char *path_env, *path_copy, *token, *full_path;
	struct stat st;

	if (command == NULL)
		return (NULL);

	if (strchr(command, '/') != NULL)
	{
		if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
			return (strdup(command));
		return (NULL);
	}

	path_env = _getenv("PATH");
	if (path_env == NULL || strlen(path_env) == 0)
	{
		if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
			return (strdup(command));
		return (NULL);
	}

	path_copy = strdup(path_env);
	if (path_copy == NULL)
		return (NULL);

	token = strtok(path_copy, ":");
	while (token != NULL)
	{
		full_path = malloc(strlen(token) + strlen(command) + 2);
		if (full_path == NULL)
		{
			free(path_copy);
			return (NULL);
		}

		sprintf(full_path, "%s/%s", token, command);
		if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		token = strtok(NULL, ":");
	}

	free(path_copy);

	if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
		return (strdup(command));

	return (NULL);
}
