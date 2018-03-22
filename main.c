#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include "utils.h"
#include "sigconfig.h"

#define SIGUSR_HANDLER_SLEEP_TIME 100

void start_exchange();
void signal_handler(int signo);
int send_signal_to_children(int signo, int table_id);
bool has_children_to_fork(int table_id, int *last_forked_child);
bool is_forking_done(pid_t *pids);

char *module;
pid_t *pids;
int current_table_id;
int sigusr1_sent = 0;
int sigusr2_sent = 0;
int received_total = 0;

int main(int argc, char *argv[])
{
    module = basename(argv[0]);

    pids = mmap(NULL, FORKS_NUM * sizeof(pid_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (pids == MAP_FAILED) {
        printerr(module, strerror(errno), "mmap");
        return 1;
    }

    start_exchange();

    if (munmap(pids, FORKS_NUM * sizeof(pid_t)) == -1) {
        printerr(module, strerror(errno), "munmap");
        return 1;
    }

    return 0;
}

void start_exchange()
{
    pid_t pid;
    if (pid = fork(), pid == -1) {
        printerr(module, strerror(errno), "fork");
        exit(1);
    }
    if (pid > 0) {
        if (setpgid(pid, pid) == -1) {
            printerr(module, strerror(errno), "setpgid");
            exit(1);
        }
        while (wait(NULL) != -1) {
            /* block the process */
        }
        return;
    }

    current_table_id = 1;
    pids[0] = getpid();

    struct sigaction handler;
    handler.sa_handler = &signal_handler;
    sigfillset(&handler.sa_mask);
    handler.sa_flags = 0;

    int last_forked_child = 0;
    while (has_children_to_fork(current_table_id, &last_forked_child)) {
        pid_t child_pid = fork();
        if (child_pid == -1) {
            printerr(module, strerror(errno), "fork");
            exit(1);
        }

        if (!child_pid) {
            current_table_id = last_forked_child;
            pids[current_table_id - 1] = getpid();
            last_forked_child = 0;
        } else {
            int gleader_pid;
            while (!(gleader_pid = pids[PROCESS_GROUPS[last_forked_child - 1] - 1])) {
                /* block the process */
            }
            if (setpgid(child_pid, gleader_pid) == -1) {
                printerr(module, strerror(errno), "setpgid");
                exit(1);
            }
        }
    }

    if (sigaction(SIGUSR1, &handler, 0) == -1) {
        printerr(module, strerror(errno), "sigaction");
        exit(1);
    }
    if (sigaction(SIGUSR2, &handler, 0) == -1) {
        printerr(module, strerror(errno), "sigaction");
        exit(1);
    }
    if (sigaction(SIGTERM, &handler, 0) == -1) {
        printerr(module, strerror(errno), "sigaction");
        exit(1);
    }

    if (current_table_id == 1) {
        while (!is_forking_done(pids)) {
            /* block the process */
        }
        int signal_to_send = GROUP_SIGNALS[RECEIVERS[current_table_id - 1] - 1];
        int receiver_pgid = pids[RECEIVERS[current_table_id - 1] - 1];
        if (kill(-receiver_pgid, signal_to_send) == -1) {
            printerr(module, strerror(errno), "kill");
            exit(1);
        }
        confirm_signal(current_table_id, pids[current_table_id - 1],
            getppid(), false, signal_to_send == SIGUSR1 ? 1 : 2);
    }

    while (true) {
        sleep(1);
    }
}

void signal_handler(int signo)
{
    if (signo == SIGTERM) {
        if (send_signal_to_children(SIGTERM, current_table_id) == -1) {
            exit(1);
        }
        while (wait(NULL) != -1) {
            /* block the process */
        }
        if (current_table_id != 1) {
            confirm_termination(current_table_id, getpid(), getppid(),
                sigusr1_sent, sigusr2_sent);
        }
        exit(0);
    } else {
        received_total++;
        confirm_signal(current_table_id, pids[current_table_id - 1], getppid(), true, signo == SIGUSR1 ? 1 : 2);

        if (current_table_id == 1 && received_total == RECEIVED_SIGS_CRITICAL) {
            usleep((SIGUSR_HANDLER_SLEEP_TIME + 10) * FORKS_NUM);
            printf("\n");
            if (send_signal_to_children(SIGTERM, current_table_id) == -1) {
                exit(1);
            }
            while (wait(NULL) != -1) {
                /* block the process */
            }
            exit(0);
        }

        int receiver_table_id = RECEIVERS[current_table_id - 1];
        if (receiver_table_id != -1) {
            int receiver_pgid = pids[receiver_table_id - 1];
            int signal_to_send = GROUP_SIGNALS[receiver_table_id - 1];

            usleep(SIGUSR_HANDLER_SLEEP_TIME);

            if (kill(-receiver_pgid, signal_to_send) == -1) {
                printerr(module, strerror(errno), "kill");
                exit(1);
            }
            confirm_signal(current_table_id, pids[current_table_id - 1],
                getppid(), false, signal_to_send == SIGUSR1 ? 1 : 2);
            if (signal_to_send == SIGUSR1) {
                sigusr1_sent++;
            } else {
                sigusr2_sent++;
            }
        }
    }
}

int send_signal_to_children(int signo, int table_id)
{
    for (int i = 0; i < FORKS_NUM; i++) {
        if (PARENTS[i] == table_id) {
            if (kill(pids[i], signo) == -1) {
                printerr(module, strerror(errno), "kill");
                return -1;
            }
        }
    }
    return 0;
}

bool has_children_to_fork(int table_id, int *last_forked_child)
{
    for (int i = *last_forked_child; i < FORKS_NUM; i++) {
        if (PARENTS[i] == table_id) {
            *last_forked_child = i + 1;
            return true;
        }
    }
    *last_forked_child = 0;
    return false;
}

bool is_forking_done(pid_t *pids)
{
    for (int i = 0; i < FORKS_NUM; i++) {
        if (!pids[i]) {
            return false;
        }
    }
    return true;
}
