#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;

#define BUFFER_SIZE 1024

void display_prompt(void)
{
	write(STDOUT_FILENO, "#cisfun$ ", 9);
}

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	pid_t pid;
	int status;

	while (1)
	{
		display_prompt();

		read = getline(&line, &len, stdin);
		if (read == -1)  /* Handles EOF (Ctrl+D) */
		{
			write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* Remove newline character */
		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		if (strlen(line) == 0)
			continue;

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			continue;
		}

		if (pid == 0)  /* Child process */
		{
			char *argv[] = {line, NULL};

			if (execve(line, argv, environ) == -1)
			{
				perror("./shell");
				exit(EXIT_FAILURE);
			}
		}
		else  /* Parent process */
		{
			wait(&status);
		}
	}

	free(line);
	return 0;
}
