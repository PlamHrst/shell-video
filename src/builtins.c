#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "exec.h"

int builtin_cd(char** args){
  if (args[1] == NULL){
    fprintf(stderr, "Provide a location to change directory to");
    return 1;
  }
  if (chdir(args[1]) == -1) perror("chdir failed in builtin_cd");
  return 1;
}

int builtin_exit(char **args) { return 0; } 

static const char *builtin_names[] = {"cd", "exit"};
static int (*builtin_funcs[])(char**) = { builtin_cd, builtin_exit };

int execute(char **args){
  if (args[0] == NULL) return 1;
  for (size_t i = 0; i < sizeof(builtin_funcs) / sizeof(*builtin_funcs); i++){
    if (strcmp(builtin_names[i], args[0]) == 0){
      return builtin_funcs[i](args);
    }
  }
  return execute_external(args);
}
