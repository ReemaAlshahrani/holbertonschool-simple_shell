#include "shell.h"

/**
 * main - Entry point for simple shell 0.3 with PATH handling
 * @ac: Argument count (unused)
 * @av: Argument vector containing program name
 * Return: Always 0 on success
 */
int main(int ac, char **av)
{
	char *buffer = NULL;
	size_t bufsize = 0;
	ssize_t characters_read;
	char *args[1024];
	char *token;
	int i;
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
			exit(EXIT_SUCCESS);
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
			handle_command(args, av[0]);
	}
	free(buffer);
	return (0);
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
 */
void handle_command(char **args, char *prog_name)
{
	pid_t child_pid;
	int status;
	char *executable_path;

	/* Look for the command inside the PATH before forking */
	executable_path = find_path(args[0]);
	if (executable_path == NULL)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", prog_name, args[0]);
		return;
	}

	/* Fork is called only after ensuring the file exists! */
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error");
		free(executable_path);
		return;
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
		free(executable_path);
	}
}
