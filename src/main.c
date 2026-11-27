#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>

#include "shell.h"
#include "input.h"
#include "builtins.h"
#include "exec.h"
#include "signals.h"

int main(void){
  install_signal_handlers();

  char *line = NULL;
  char **args = NULL;
  size_t lenOfLine = 0;
  int keep_running = 1;
  char *stage_strs[MAX_STAGES];
  char **commands[MAX_STAGES];

  while (keep_running){
    printf("mysh>");
    
    if (getline(&line, &lenOfLine, stdin) == -1){
      if (got_sigint) {
        got_sigint = 0;
        clearerr(stdin);
        printf("\n");
        continue; // Ctrl+C interrupted the read
      }
      printf("\n");
      break; // EOF (Ctrl D) apparantly
    }
    int n = split_pipeline(line, stage_strs);
    for (int i = 0; i < n; i++) {
        commands[i] = tokenize(stage_strs[i]); // same tokenize() from Part 1, unchanged
    }

    keep_running = (n == 1) ? execute(commands[0])        // single command: builtin-or-external
                           : run_pipeline(commands, n);  // 2+ stages: the pipe path

    for (int i = 0; i < n; i++) free(commands[i]);
  }
  free(line);
  line = NULL;
  return EXIT_SUCCESS;
}
