#include "shell.h"

/**
 * main - Entry point for simple shell 1.0 with exit and env built-ins
 * @ac: Argument count (unused)
 * @av: Argument vector containing program name
 * Return: Last recorded exit status
 */
int main(int ac, char **av)
{
	char *buffer = NULL;
	size_t bufsize = 0;
	ssize_t characters_read;
	char *args[1024];
	char *token;
	int i, env_idx, exit_status = 0;
	(void)ac;

	while (1)
	{
		prompt_display();

		characters_read = getline(&buffer, &bufsize, stdin);
		if (characters_read == -1)
		{
			free(buffer);
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			exit(exit_status);
		}

		if (buffer[characters_read - 1] == '\n')
			buffer[characters_read - 1] = '\0';

		i = 0;
		token = strtok(buffer, " \t");
		while (token != NULL)
		{
			args[i] = token;
			i++;
			token = strtok(NULL, " \t");
		}
		args[i] = NULL;

		if (args[0] != NULL)
		{
			/* 1. Built-in exit handling */
			if (strcmp(args[0], "exit") == 0)
			{
				free(buffer);
				exit(exit_status);
			}
			/* 2. Built-in env handling */
			else if (strcmp(args[0], "env") == 0)
			{
				env_idx = 0;
				while (environ[env_idx] != NULL)
				{
					write(STDOUT_FILENO, environ[env_idx], strlen(environ[env_idx]));
					write(STDOUT_FILENO, "\n", 1);
					env_idx++;
				}
				exit_status = 0;
			}
			/* 3. External commands handling */
			else
			{
				exit_status = handle_command(args, av[0]);
			}
		}
	}
	free(buffer);
	return (exit_status);
}

/**
 * prompt_display - Prints the prompt symbol if in interactive mode
 */
void prompt_display(void)
{
	if (isatty(STDIN_FILENO))
	{
		write(STDOUT_FILENO, "$ ", 2);
		fflush(stdout);
	}
}

/**
 * handle_command - Validates path and forks only if command exists
 * @args: Array of command strings and parameters
 * @prog_name: Name of the shell program
 * Return: 0 on success, 127 if command not found, or child exit status
 */
int handle_command(char **args, char *prog_name)
{
	pid_t child_pid;
	int status, exit_code = 0;
	char *executable_path;

	executable_path = find_path(args[0]);
	if (executable_path == NULL)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", prog_name, args[0]);
		return (127);
	}

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error");
		free(executable_path);
		return (1);
	}

	if (child_pid == 0)
	{
		if (execve(executable_path, args, environ) == -1)
		{
			perror(prog_name);
			free(executable_path);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&status);
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		free(executable_path);
	}

	return (exit_code);
}
