#ifndef SIGNAME_H
#define SIGNAME_H

#include <stdio.h>
#include <signal.h>

// Gets signal name by its number, returns NULL for invalid one
char const *get_signame(int signo);

#endif //SIGNAME_H
