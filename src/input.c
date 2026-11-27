#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "input.h"

int split_pipeline(char *line, char **stages) {
    int n = 0;
    char *stage = strtok(line, "|");
    while (stage != NULL && n < MAX_STAGES) {
        stages[n++] = stage;
        stage = strtok(NULL, "|");
    }
    return n;
}

char **tokenize(char* line){
  char** tokens = malloc(MAX_TOKENS * sizeof(char *));
  int i = 0;
  char* token = strtok(line, DELIMS);
  while (token != NULL && i < MAX_TOKENS - 1){
    tokens[i++] = token;
    token = strtok(NULL, DELIMS);
  }
  tokens[i] = NULL; // for some reason it seems to work without this so test what happens
  return tokens;
}
