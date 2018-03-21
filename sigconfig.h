#ifndef SIGNALS_SIGCONFIG_H
#define SIGNALS_SIGCONFIG_H

#include <signal.h>

#define FORKS_NUM 8
#define RECEIVED_SIGS_CRITICAL 101

const int PARENTS[FORKS_NUM] = {0, 1, 1, 2, 2, 5, 6, 6};
const int RECEIVERS[FORKS_NUM] = {2, -1, 4, 5, -1, -1, 8, 1};
const int PROCESS_GROUPS[FORKS_NUM] = {1, 2, 2, 4, 5, 5, 5, 8};
const int GROUP_SIGNALS[FORKS_NUM] = {SIGUSR2, SIGUSR1, -1, SIGUSR2, SIGUSR1, -1, -1, SIGUSR1};

#endif //SIGNALS_SIGCONFIG_H
