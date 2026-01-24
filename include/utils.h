#ifndef UTILS_H
#define UTILS_H

#define MAX_ARGS 100  // max 100 arguments
#define MAX_LEN 50
#define TOKEN_SEP " \t\n\r"

typedef struct {
  char* progname;
  int redirect[2];
  char* args[];
} cmd_struct;

typedef struct {
  int n_cmds;
  cmd_struct* cmds[];
} pipeline_struct;

pipeline_struct *parse_pipeline(char *line);

// Retourne un tableau d'arguments à partir d'une ligne
cmd_struct* parse_command(char* str);

#endif