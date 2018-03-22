# signals
OS and System Programming, lab #4

Build a process tree according to the listed table. Entry "1->(2,3,4,5)" in table #1 means that initial process 0 creates child process 1, which in turn creates child processes 2, 3, 4, 5. Entry "1->(2,3,4) SIGUSR1" in table #2 means that process 1 sends SIGUSR1 to his children (2, 3, 4) simultaneously (i.e. using single kill() call). After receiving or sending a signal every process outputs the following info:
```
N pid ppid has received/sent SIGUSR1/SIGUSR2 at (current time) ms
```
N is the child index in table #1.
After receiving 101st SIGUSR signal, process 1 sends SIGTERM to his children, then awaits their termination and terminates itself. Process 0 waits for process 1 termination and terminates itself as well. When receiving SIGTERM, children terminate themselves with the following output:
```
pid ppid has terminated; SIGUSR1 sent: X, SIGUSR2 sent: Y
```
X and Y are total number of received SIGUSR1 and SIGUSR2 respectively.

Tables #1/#2 entries for the implemented case:
```
1->(2,3)   2->(4,5)   5->6    6->(7,8)
```
```
1->(2,3) SIGUSR1   3->4 SIGUSR2   4->(5,6,7) SIGUSR1   
7->8 SIGUSR1   8->1 SIGUSR2
```
Every other case of this assignment could be implemented by modifying sigconfig.h
