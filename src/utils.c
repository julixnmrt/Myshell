#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char **parse_command(char *line)
{
    char **args = malloc(sizeof(char*) * (MAX_ARGS + 1));
    if (!args) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int i = 0;

    // Supprimer le '\n' final
    line[strcspn(line, "\n")] = '\0';

    // Découper la ligne
    char *token = strtok(line, " ");
    while (token != NULL && i < MAX_ARGS) {
        args[i] = strdup(token);  // alloue une copie
        if (!args[i]) {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
        i++;
        token = strtok(NULL, " ");
    }

    args[i] = NULL;  // argv doit finir par NULL
    return args;
}
