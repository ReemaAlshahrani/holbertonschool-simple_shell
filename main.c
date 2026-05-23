#include "shell.h"

/**
 * main - Entry point for simple shell 0.1 with space handling
 * @ac: Argument count (unused)
 * @av: Argument vector containing program name
 * Return: Always 0 on success
 */
int main(int ac, char **av)
{
	char *buffer = NULL;
	size_t bufsize = 0;
	ssize_t characters_read;
	char *clean_command;
	(void)ac;

	while (1)
	{
		prompt_display();

		/* Handle end of file (Ctrl+D) condition */
		characters_read = getline(&buffer, &bufsize, stdin);
		if (characters_read == -1)
		{
			free(buffer);
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			exit(EXIT_SUCCESS);
		}

		/* Remove trailing newline character */
		if (buffer[characters_read - 1] == '\n')
			buffer[characters_read - 1] = '\0';

		/* Use strtok to strip away spaces and tabs */
		clean_command = strtok(buffer, " \t");

		/* Execute only if there is a valid command after trimming */
		if (clean_command != NULL && strlen(clean_command) > 0)
		{
			handle_command(clean_command, av[0]);
		}
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
		write(STDOUT_FILENO, "#cisfun$ ", 9);
		fflush(stdout);
	}
}

/**
 * handle_command - Executes simple single-word command
 * @command: The clean string containing the executable command
 * @prog_name: Name of the shell program for error tracking
 */
void handle_command(char *command, char *prog_name)
{
	pid_t child_pid;
	int status;
	char *args[2];

	args[0] = command;
	args[1] = NULL;

	/* Check if the file exists and is executable */
	if (access(args[0], X_OK) == -1)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", prog_name, args[0]);
		return;
	}

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error");
		return;
	}

	if (child_pid == 0)
	{
		/* Core execution sending system environment */
		if (execve(args[0], args, environ) == -1)
		{
			perror(prog_name);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* Wait until the execution finishes */
		wait(&status);
	}
}
