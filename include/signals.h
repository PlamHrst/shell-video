#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>

extern volatile sig_atomic_t got_sigint;

void install_signal_handlers(void);

#endif
