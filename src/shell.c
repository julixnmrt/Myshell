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
    int *nb_cmds = 0;
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

        pipeline_struct *pipeline = parse_pipeline(line);

        for (int i = 0; i < pipeline->n_cmds; i++) {
            cmd_struct *cmd = pipeline->cmds[i];

            printf("{");
            for (int j = 0; cmd->args[j] != NULL; j++) {
                printf("\"%s\"", cmd->args[j]);
                if (cmd->args[j + 1] != NULL)
                    printf(", ");
            }
            printf("} ");
        }

        printf("\n");
    }

    free(line);
    return 0;
}
