#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 1024

/**
 * main - Entry point of the simple shell program
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: 0 on success, or error code
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while (1)
    {
        printf("$ "); /* prompt */

        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            perror("getline");
            break;
        }

        /* Retirer le \n à la fin */
        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        /* Construire argv */
        char *argv[2];
        argv[0] = line;
        argv[1] = NULL;

        /* Exécuter la commande */
        if (execvp(argv[0], argv) == -1)
        {
            perror("execvp");
        }

        /* On sort uniquement si execvp échoue */
    }

    free(line);
    return 0;
}
