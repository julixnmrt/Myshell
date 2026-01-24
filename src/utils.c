#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char* next_non_empty(char **line) {
  char *tok;

  /* Consume empty tokens. */
  while ((tok = strsep(line, TOKEN_SEP)) && !*tok);

  return tok;
}


pipeline_struct *parse_pipeline(char *line){

  char* copy = strndup(line, MAX_LEN);
  char* cmd_str;
  int n_cmds = 0;
  int i = 0;

    // compte le nombre de pipeline pour l'allocation du tableau de commandes
    for (char* cur = copy; *cur; cur++) {
        if (*cur == '|') ++n_cmds;
    }

    ++n_cmds; //il y a une commande (cmds) de plus que de pipe

    pipeline_struct *cmds = calloc(sizeof(pipeline_struct) + n_cmds * sizeof(cmd_struct*), 1);
    cmds->n_cmds = n_cmds;

    while((cmd_str = strsep(&copy, "|"))) {
        cmds->cmds[i++] = parse_command(cmd_str);
    }

    return cmds;
}

cmd_struct* parse_command(char* str) {
  /* Copy the input line in case the caller wants it later. */
  char* copy = strndup(str, MAX_LEN);
  char* token;
  int i = 0;

  /*
   * Being lazy (Rule 0) and allocating way too much memory for the args array.
   * Using calloc to ensure it's zero-initialised, which is important because
   * execvp expects a NULL-terminated array of arguments.
   */
  cmd_struct* ret = calloc(sizeof(cmd_struct) + MAX_LEN * sizeof(char*), 1);

  while ((token = next_non_empty(&copy))) {
    ret->args[i++] = token;
  }
  ret->progname = ret->args[0];
  ret->redirect[0] = ret->redirect[1] = -1;
  return ret;
}