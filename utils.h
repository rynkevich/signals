#ifndef SIGNALS_SIGUTILS_H
#define SIGNALS_SIGUTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/types.h>

void printerr(const char *module, const char *errmsg, const char *comment);
void confirm_signal(int table_id, pid_t pid, pid_t ppid, bool has_received, int sigusrno);
void confirm_termination(int table_id, pid_t pid, pid_t ppid, int sigusr1_sent, int sigusr2_sent);

#endif //SIGNALS_SIGUTILS_H
