#!/bin/bash

echo '#include <signal.h>' |\
    g++ -E -dD -xc - |\
    awk '$2~/^SIG[A-Z]/ && $3~/[0-9]+/ && !seen[$3]++ { print "case "$2": return \""$2"\";" }' \
    > signal_to_str_cases.inc
