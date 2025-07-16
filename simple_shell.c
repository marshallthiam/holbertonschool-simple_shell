#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * main - Entry point of the simple shell
 *
 * Return: Always 0 on success
 */
char *argv[2];  /* Déclare le tableau avec deux éléments */
argv[0] = line;
argv[1] = NULL;

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	pid_t pid;
	int status;

	while (1)
	{
		/* Print the prompt */
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		/* Read input */
		nread = getline(&line, &len, stdin);

		if (nread == -1)
		{
			free(line);
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* Remove trailing newline */
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		/* Exit command */
		if (strcmp(line, "exit") == 0)
			break;

		/* Fork to execute */
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			continue;
		}
		if (pid == 0)
		{

			if (execvp(argv[0], argv) == -1)
			{
				perror("execvp");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			waitpid(pid, &status, 0);
		}
	}

	free(line);
	return (0);
}
