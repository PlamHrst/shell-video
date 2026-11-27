#define _POSIX_C_SOURCE 200809L
#include <stddef.h>
#include "signals.h"

volatile sig_atomic_t got_sigint = 0; // not static anymore: main.c reads this after an interrupted getline()

static void sigint_handler(int signo) {
    (void)signo;
    got_sigint = 1;
}

void install_signal_handlers(void) {
    struct sigaction sa = {0};
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; // no SA_RESTART: we WANT getline() to return early so main's loop can redraw the prompt
    sigaction(SIGINT, &sa, NULL);
}
