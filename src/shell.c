#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(void)
{
    char *line = NULL;
    size_t len = 0;

    while (1)
    {
        printf("myshell> ");
        fflush(stdout);

        if (getline(&line, &len, stdin) == -1)
        {
            printf("\n");
            break;
        }

        
        char **argv = parse_command(line);
        int i = 0;
        while (argv[i]!=NULL){
            printf("%s\n", argv[i]);
            i++;
        }
    }

    free(line);
    return 0;
}