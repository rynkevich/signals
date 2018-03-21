#ifndef SIGNALS_SIGUTILS_H
#define SIGNALS_SIGUTILS_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>

struct sigcounter
{
    int signal;
    int quantity;
};

long long get_time();
void printerr(const char *module, const char *errmsg, const char *comment);
void confirm_signal(int table_id, pid_t pid, pid_t ppid, bool has_received, int sigusrno);
void print_time();
void confirm_termination(int table_id, pid_t pid, pid_t ppid, int sigusr1_sent, int sigusr2_sent);
void print_signumber(struct sigcounter *siginfo);

#endif //SIGNALS_SIGUTILS_H