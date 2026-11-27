#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

#include "shell.h"
#include "exec.h"

int execute_external(char **args){
  
  if (args[0] == NULL) return 0;
  
  pid_t pid = fork();

  if (pid == -1){
    perror("fork error inside execute_command");
    return 0;
  }
  // Child Process
  if (pid == 0){
    signal(SIGINT, SIG_DFL); // undo the shell's SIGINT handler: Ctrl+C should kill THIS program normally
    char* cmd = args[0];

    execvp(cmd, args);
    // If it returns something then it failed
    perror("execvp returned inside execute_command");
    exit(127); // convention for command not found which is likely the reason for failure
    // exit not return because we need the child to exit not to become a zombie if it failed to execute while return would not kill it
  }
  // Parent Process
  else {
    int status;

    waitpid(pid, &status, WUNTRACED);
  }
  return 1;
}

int run_pipeline(char ***commands, int n) {
    int in_fd = STDIN_FILENO;  // stage 0 reads the real stdin
    pid_t pids[MAX_STAGES];

    for (int i = 0; i < n; i++) {
        int fd[2];
        int is_last = (i == n - 1);
        if (!is_last) pipe(fd);

        pids[i] = fork();
        if (pids[i] == 0) {
            signal(SIGINT, SIG_DFL); // same reason as in execute_external: each stage dies normally on Ctrl+C
            if (in_fd != STDIN_FILENO) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (!is_last) {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
            }
            execvp(commands[i][0], commands[i]);
            perror(commands[i][0]);
            exit(127);
        }

        if (in_fd != STDIN_FILENO) close(in_fd);       // parent's done with the last stage's read end
        if (!is_last) { close(fd[1]); in_fd = fd[0]; }  // hand this stage's read end to the next iteration
    }

    int status;
    for (int i = 0; i < n; i++) waitpid(pids[i], &status, 0);
    return 1;
}
