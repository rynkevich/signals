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
    fprintf(stderr, "%s: %s (%s)\n", module, errmsg, comment ? comment : "");
}

void confirm_signal(int table_id, pid_t pid, pid_t ppid, bool has_received, int signal)
{
    printf("table_id: %d pid: %d ppid: %d has %s %s at %lld\n", table_id, pid, ppid,
           has_received ? "received" : "sent", get_signame(signal), get_time());
    fflush(stdout);
}

void confirm_termination(pid_t pid, pid_t ppid, int sigcount, ...)
{
    va_list counters;
    va_start(counters, sigcount);

    printf("%d %d has terminated; sent signals: ", pid, ppid);
    for (int i = 0; i < sigcount; i++) {
        struct sigcounter *siginfo = va_arg(counters, struct sigcounter *);
        print_signumber(siginfo);
        if (i != sigcount - 1) {
            printf(", ");
        }
    }
    printf("\n");

    va_end(counters);
}

void print_signumber(struct sigcounter *siginfo)
{
    printf("%d of %s", siginfo->quantity, get_signame(siginfo->signal));
}