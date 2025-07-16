#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_ARGS 64

char *find_command_path(char *command);
void run_command(char **args);
/**
 * main - Entry point for the shell program
 * Return: 0 on success
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *args[MAX_ARGS];
    char *token;
    int i;

    while (1)
    {
        printf(":) ");
        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            perror("getline");
            break;
        }

        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        i = 0;
        token = strtok(line, " ");
        while (token && i < MAX_ARGS - 1)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] == NULL)
            continue;

        run_command(args);
    }

    free(line);
    return 0;
}
/**
 * run_command - Executes a command entered by the user
 * @input: the full input line from the user
 * Return: 1 if the shell should continue running, 0 otherwise
 */
void run_command(char **args)
{
    pid_t pid;
    char *cmd_path;

    /* Si commande contient /, essayer directement */
    if (strchr(args[0], '/') != NULL)
    {
        if (access(args[0], X_OK) == 0)
            cmd_path = args[0];
        else
        {
            perror("Command not found");
            return;
        }
    }
    else
    {
        cmd_path = find_command_path(args[0]);
        if (cmd_path == NULL)
        {
            fprintf(stderr, "%s: command not found\n", args[0]);
            return;
        }
    }

    pid = fork();
    if (pid == 0)
    {
        /* processus fils */
        execve(cmd_path, args, environ);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        /* processus parent */
        wait(NULL);
        if (cmd_path != args[0])
            free(cmd_path);
    }
    else
    {
        perror("fork");
    }
}
/**
 * find_command_path - Finds the full path of a command in the PATH
 * @command: The command name
 * Return: Pointer to the full path string or NULL if not found
 */
char *find_command_path(char *command)
{
    char *path = getenv("PATH");
    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");
    char full_path[1024];

    while (dir)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return strdup(full_path);
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}
