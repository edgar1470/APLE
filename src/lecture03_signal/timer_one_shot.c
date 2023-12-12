#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

void
timer_handler(int signum)
{
    printf("signum = %d timer alarm\n", signum);
    fflush(stdout);
}

int
main(int argc, char *argv[])
{
    int interval_seconds = 3; // defalut 10seconds
    int remain           = 0;

    if (argc == 2) {
        interval_seconds = atoi(argv[1]);
    }
    struct sigaction sa;
    memset(&sa, 0x0, sizeof(struct sigaction));
    sa.sa_handler = &timer_handler;

    if (sigaction(SIGALRM, &sa, NULL) != 0) {
        printf("register signal process failed\n");
        return -1;
    }

    struct itimerval new;
    memset(&new, 0x0, sizeof(struct itimerval));
    // ont-shot tiemr
    new.it_interval.tv_sec  = 0;
    new.it_interval.tv_usec = 0;
    // timer is armed to interval_seconds
    new.it_value.tv_sec  = interval_seconds;
    new.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &new, NULL);

    for (int i = 0; i < 10; i++) {
        remain = sleep(1); // expired or a signal arrived
        printf("reamin=%d\n", remain);
        fflush(stdout);
    }
}
