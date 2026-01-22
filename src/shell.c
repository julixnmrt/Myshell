#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "utils.h"

/*
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
        if(strcmp(argv[0], "cd") == 0){
            if (argv[1] != NULL) {
                chdir(argv[1]);
            } else {
                fprintf(stderr, "cd: missing argument\n");
            }
        }
        else{
            pid_t p = fork();
            if(p<0){
                perror("fork fail");
                exit(1);
            }
            else if(p==0){
                execvp(argv[0], argv);
            }
            else{
                wait(NULL);
            }  
        }
      
    }

    free(line);
    return 0;
}
*/

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
        printf("%s", line);
    }
}