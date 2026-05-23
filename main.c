#include "shell.h"

/**
 * main - Core REPL loop for the simple shell
 * Return: Always 0 on success
 */
int main(void)
{
	char *buffer = NULL;
	size_t bufsize = 0;
	ssize_t characters_read;

	while (1)
	{
		prompt_display();

		/* Read input and handle EOF / Ctrl+D */
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

		/* Execute command if input is not empty */
		if (strlen(buffer) > 0)
		{
			handle_command(buffer);
		}
	}

	free(buffer);
	return (0);
}

/**
 * prompt_display - Prints the shell prompt in interactive mode
 */
void prompt_display(void)
{
	if (isatty(STDIN_FILENO))
	{
		write(STDOUT_FILENO, "($) ", 4);
		fflush(stdout);
	}
}

/**
 * handle_command - Forks a child process and executes a basic command
 * @command: The string containing the command to execute
 */
void handle_command(char *command)
{
	pid_t child_pid;
	int status;
	char *args[2];

	args[0] = command;
	args[1] = NULL;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error");
		return;
	}

	if (child_pid == 0)
	{
		/* Execute the program using system environment variables */
		if (execve(args[0], args, environ) == -1)
		{
			perror("./hsh");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* Parent process waits for child to finish execution */
		wait(&status);
	}
}
