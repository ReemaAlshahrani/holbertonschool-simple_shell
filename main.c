#include "shell.h"

/**
 * main - Entry point for simple shell with full argument support
 * @ac: Argument count (unused)
 * @av: Argument vector containing program name
 * Return: Always 0 on success
 */
int main(int ac, char **av)
{
	char *buffer = NULL;
	size_t bufsize = 0;
	ssize_t characters_read;
	char *args[1024]; /* Array to store the command and its arguments */
	char *token;
	int i;
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

		/* Tokenize the input string into arguments */
		i = 0;
		token = strtok(buffer, " \t");
		while (token != NULL)
		{
			args[i] = token;
			i++;
			token = strtok(NULL, " \t");
		}
		args[i] = NULL; /* Executive array must end with NULL */

		/* Execute only if at least one argument/command is typed */
		if (args[0] != NULL)
		{
			handle_command(args, av[0]);
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
 * handle_command - Executes commands with full argument vectors
 * @args: Array of strings containing the command and arguments
 * @prog_name: Name of the shell program for error tracking
 */
void handle_command(char **args, char *prog_name)
{
	pid_t child_pid;
	int status;

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
		/* Send the entire matrix of arguments into execve */
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
