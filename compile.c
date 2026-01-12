#include <stdlib.h>
#include "sys/wait.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void main()
{
    printf("yusato> ");
    char *line = NULL;
    size_t len = 0;
    int capacity = 5;

    while (getline(&line, &len, stdin) != -1)
    {
        line[strcspn(line, "\n")] = '\0';
        char *token = strtok(line, " ");
        char **args = malloc(capacity * sizeof(char *));
        int i = 0;
        while (token != NULL)
        {
            args[i++] = token;
            printf("%s %i \n", args[i - 1], i);
            token = strtok(NULL, " ");
            if (i >= capacity)
            {
                printf("realoc %i\n", capacity);
                args = realloc(args, (capacity * 2) * sizeof(char *));
            }
            if (token == NULL)
            {
                args[i] = NULL;
            }
        }
        if (args[0] == NULL)
        {
            continue;
        }
        if (strcmp(args[0], "exit") == 0)
        {
            printf("exit shell\n");
            system("clear");
            break;
        }
        pid_t pid = fork();
        if (pid < 0)
        {
            printf("fork failed");
        }
        else if (pid == 0)
        {
            char *command = args[0];
            execvp(command, args);
            perror("exec failed");
        }
        else if (pid > 0)
        {
            int status;
            pid_t child_id = wait(&status);
            if (WIFEXITED(status))
            {
                printf("Command end with code %i in process %i\n", status, child_id);
            }
            printf("yusato> ");
        }
        free(args);
    }
    free(line);
}