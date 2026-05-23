#include "shell.h"

/**
 * main - Entry point for simple shell 0.2 with full argument handling
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

		/* Tokenize the input string into arguments array */
		i = 0;
		token = strtok(buffer, " \t");
		while (token != NULL)
		{
			args[i] = token;
			i++;
			token = strtok(NULL, " \t");
		}
		args[i] = NULL;

		/* Execute only if the arguments array is not empty */
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
 * handle_command - Forks a child process and runs command with arguments
 * @args: Double pointer array containing command and its parameters
 * @prog_name: Name of the shell program used for standard error outputs
 */
void handle_command(char **args, char *prog_name)
{
	pid_t child_pid;
	int status;

	/* Check if the specific command file exists and is executable */
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
		/* Execute the whole arguments array through execve */
		if (execve(args[0], args, environ) == -1)
		{
			perror(prog_name);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* Wait until the execution finishes before prompting again */
		wait(&status);
	}
}
