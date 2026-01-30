#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "utils.h"
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t got_sigint = 0;

void sigint_handler(int sig) {
    (void)sig; 
    got_sigint = 1;
    write(STDOUT_FILENO, "\n", 1); // saute une ligne
}

int main(void)
{
    signal(SIGINT, sigint_handler);

    char *line = NULL;
    size_t len = 0;

    while (1)
    {
        char cwd[1024];
        int interactive = isatty(STDIN_FILENO); // vrai si mode interactif (pour tests)

        if (interactive) {
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                char *home = getenv("HOME");
                if (home && strncmp(cwd, home, strlen(home)) == 0) {
                    // Affiche "~" suivi du reste du chemin
                    printf("\033[32mMyshell:\033[0m\033[34m~%s\033[0m$\033[0m ",
                        cwd + strlen(home));
                } else {
                    // Affiche le chemin complet
                    printf("\033[32mMyshell:\033[0m\033[34m%s\033[0m$\033[0m ",
                        cwd);
                }
            } else {
                perror("getcwd");
                printf("\033[32mMyshell@\033[0m\033[32m$\033[0m ");
            }
            fflush(stdout);
        }
        
        ssize_t r = getline(&line, &len, stdin);
        if (r == -1) {
            if (got_sigint) {
                got_sigint = 0;
                continue; // ← retour au prompt
            }
            printf("\n");
            break;
        }


        pipeline_struct *pipeline = parse_pipeline(line);
        int n_pipes = pipeline->n_cmds - 1;
        
        if (pipeline == NULL ||
            pipeline->n_cmds == 0 ||
            pipeline->cmds[0]->args[0] == NULL ||
            pipeline->cmds[0]->args[0][0] == '\0') {
            continue;
        }

        if(n_pipes == 0){
            char *commande =pipeline->cmds[0]->args[0];

            if (strcmp(commande, "cd") == 0) {
                char *path;

                if (pipeline->cmds[0]->args[1] != NULL) {
                    path = pipeline->cmds[0]->args[1];
                } else {
                    path = getenv("HOME");
                    if (path == NULL) {
                        fprintf(stderr, "cd: HOME not set\n");
                        continue;
                    }
                }

                if (chdir(path) == -1) {
                    perror("cd");
                }
            }

            else if(strcmp(commande, "exit") == 0|| strcmp(commande, "quit") == 0){
                break;
            }
            else{
                pid_t p = fork();
                if(p<0){
                    perror("fork fail");
                    exit(1);
                }
                else if(p==0){
                    signal(SIGINT, SIG_DFL);
                    execvp(commande, pipeline->cmds[0]->args);
                    perror("execvp");
                    exit(1);
                }
                else{
                    wait(NULL);
                }  
            }
        }
        else{
            /* 1. Créer tous les pipes */
            int pipes[n_pipes][2];
            for (int i = 0; i < n_pipes; i++) {
                if (pipe(pipes[i]) == -1) {
                    perror("pipe erro");
                    exit(1);
                }
            }

            /* 2. Fork pour chaque commande */
            for (int i = 0; i < pipeline->n_cmds; i++) {
                pid_t pid = fork();
                if (pid == -1) {
                    perror("fork");
                    exit(1);
                }

                if (pid == 0) {
                    /* stdin */
                    if (i > 0) {
                        dup2(pipes[i - 1][0], STDIN_FILENO);
                    }

                    /* stdout */
                    if (i < pipeline-> n_cmds - 1) {
                        dup2(pipes[i][1], STDOUT_FILENO);
                    }

                    /* fermer TOUS les pipes */
                    for (int j = 0; j < n_pipes; j++) {
                        close(pipes[j][0]);
                        close(pipes[j][1]);
                    }

                    signal(SIGINT, SIG_DFL);
                    execvp(pipeline->cmds[i]->args[0], pipeline->cmds[i]->args);

                    perror("execvp");
                    exit(1);
                }
            }

            /* 3. Fermer tous les pipes dans le parent */
            for (int i = 0; i < n_pipes; i++) {
                close(pipes[i][0]);
                close(pipes[i][1]);
            }

            /* 4. Attendre tous les enfants */
            for (int i = 0; i < pipeline->n_cmds; i++) {
                wait(NULL);
            }
        }
    }

    free(line);
    return 0;
}
