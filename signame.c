#include "signame.h"

char const *get_signame(int signo)
{
    switch(signo) {
        #include "signal_to_str_cases.inc"
        default: return NULL;
    }
}


