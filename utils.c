#include "utils.h"

long long get_time();

long long get_time()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_usec / 1000;
}

void printerr(const char *module, const char *errmsg, const char *comment)
{
    fprintf(stderr, "%s: %s ", module, errmsg);
    if (comment) {
        fprintf(stderr, "(%s)", comment);
    }
    fprintf(stderr, "\n");
}

void confirm_signal(int table_id, pid_t pid, pid_t ppid, bool has_received, int sigusrno)
{
    printf("%d %d %d has %s SIGUSR%d at %lld ms\n", table_id, pid, ppid,
        has_received ? "received" : "sent", sigusrno, get_time());
    fflush(stdout);
}

void confirm_termination(int table_id, pid_t pid, pid_t ppid, int sigusr1_sent, int sigusr2_sent)
{
    printf("%d %d %d has terminated; SIGUSR1 sent: %d, SIGUSR2 sent: %d\n",
        table_id, pid, ppid, sigusr1_sent, sigusr2_sent);
}
