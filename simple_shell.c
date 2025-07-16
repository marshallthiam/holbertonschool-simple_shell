#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern char **environ;

#define BUFFER_SIZE 1024

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *args[2];

    while (1)
    {
        /* Display prompt */
        write(STDOUT_FILENO, "#cisfun$ ", 9);

        /* Read input */
        read = getline(&line, &len, stdin);
        if (read == -1)
        {
            /* Handle Ctrl+D (EOF) */
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        /* Remove newline character */
        if (line[read - 1] == '\n')
            line[read - 1] = '\0';

        /* Skip empty input */
        if (line[0] == '\0')
            continue;

        /* Prepare args for execve */
        args[0] = line;
        args[1] = NULL;

        /* Fork and execve */
        pid_t pid = fork();

        if (pid == -1)
        {
            perror("fork");
            continue;
        }
        else if (pid == 0)
        {
            /* Child process */
            if (execve(args[0], args, environ) == -1)
            {
                perror("./shell");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            /* Parent process waits */
            wait(NULL);
        }
    }

    free(line);
    return 0;
}
