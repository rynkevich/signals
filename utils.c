#include "utils.h"

//long long get_time();

long long get_time()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_usec / 1000;
}

void printerr(const char *module, const char *errmsg, const char *comment)
{
    fprintf(stderr, "%s: %s (%s)\n", module, errmsg, comment ? comment : "");
}

void confirm_signal(int table_id, pid_t pid, pid_t ppid, bool has_received, int sigusrno)
{
    printf("table_id: %d pid: %d ppid: %d has %s SIGUSR%d at %lld\n", table_id, pid, ppid,
           has_received ? "received" : "sent", sigusrno, get_time());
    fflush(stdout);
}

void confirm_termination(pid_t pid, pid_t ppid, int sigusr1_sent, int sigusr2_sent)
{
    printf("%d %d has terminated; SIGUSR1 sent: %d, SIGUSR2 sent: %d\n",
           pid, ppid, sigusr1_sent, sigusr2_sent);
    fflush(stdout);
}