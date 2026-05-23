#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* Global environment variable */
extern char **environ;

/* Function prototypes */
void prompt_display(void);
void handle_command(char **args, char *prog_name);
char *find_path(char *command);
char *_getenv(const char *name);

#endif /* SHELL_H */
