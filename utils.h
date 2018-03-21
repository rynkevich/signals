#ifndef SIGNALS_SIGUTILS_H
#define SIGNALS_SIGUTILS_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include "signame.h"

struct sigcounter
{
    int signal;
    int quantity;
};

void printerr(const char *module, const char *errmsg, const char *comment);
void confirm_signal(int table_id, pid_t pid, pid_t ppid, bool has_received, int signal);
void print_time();
void confirm_termination(pid_t pid, pid_t ppid, int sigcount, ...);
void print_signumber(struct sigcounter *siginfo);

#endif //SIGNALS_SIGUTILS_H
