#include <inttypes.h> /* Definition of PRIu64 */
#include <stdint.h>   /* Definition of uint64_t */
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>

#define handle_error(msg)                                                     \
    do {                                                                      \
        perror(msg);                                                          \
        exit(EXIT_FAILURE);                                                   \
    } while (0)

#define MAX_REPEAT 10
static void
print_elapsed_time(void)
{
    static struct timespec start;
    struct timespec        curr;
    static int             first_call = 1;
    int                    secs, nsecs;

    if (first_call) {
        first_call = 0;
        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
            handle_error("clock_gettime");
    }

    if (clock_gettime(CLOCK_MONOTONIC, &curr) == -1)
        handle_error("clock_gettime");

    secs  = curr.tv_sec - start.tv_sec;
    nsecs = curr.tv_nsec - start.tv_nsec;
    if (nsecs < 0) {
        secs--;
        nsecs += 1000000000;
    }
    printf("%d.%03d: ", secs, (nsecs + 500000) / 1000000);
    // printf("%d.%d: ", curr.tv_sec, curr.tv_nsec);
}

int
set_timerfd_value(int begin_secs, int interval_secs)
{
    struct timespec   now;
    struct itimerspec new_value;
    int               fd;

    if (clock_gettime(CLOCK_REALTIME, &now) == -1)
        handle_error("clock_gettime");

    // ont-shot tiemr
    // new.it_interval.tv_sec  = 0;
    // new.it_interval.tv_usec = 0;
    // tiemr start at 'start' seconds later
    new_value.it_value.tv_sec     = now.tv_sec + begin_secs;
    new_value.it_value.tv_nsec    = now.tv_nsec;
    new_value.it_interval.tv_sec  = interval_secs;
    new_value.it_interval.tv_nsec = 0;

    fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd == -1) handle_error("timerfd_create");

    printf("fd=%d start=%d interval = %d\n", fd, new_value.it_value.tv_sec,
           new_value.it_interval.tv_sec);

    if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL) == -1)
        handle_error("timerfd_settime");

    return fd;
}

int
main(void)
{
    uint64_t       exp;
    int            fd1, fd2, fd3;
    fd_set         rfds;
    struct timeval tv;
    int            nready = 0;

    printf("timer1 start 1 seconds later , interval = 1, repeat\n");
    printf("timer2 start 5 seconds later , interval = 3, repeat\n");
    printf("timer3 start 5 seconds later , interval = 0, one-shot\n");
    fd1 = set_timerfd_value(1, 1);
    fd2 = set_timerfd_value(5, 3);
    fd3 = set_timerfd_value(5, 0);

    FD_ZERO(&rfds);

    print_elapsed_time();
    printf("timer started %d %d %d\n", fd1, fd2, fd3);

    // wait timeout 5 seconds.
    tv.tv_sec  = 5;
    tv.tv_usec = 0;

    for (int i = 0; i < MAX_REPEAT; i++) {
        FD_SET(fd1, &rfds);
        FD_SET(fd2, &rfds);
        FD_SET(fd3, &rfds);
        // ret = select(fd3 + 1, &rfds, NULL, NULL, &tv);
        nready = select(fd3 + 1, &rfds, NULL, NULL, NULL); // wait forever
        if (nready == -1) handle_error("select error");
        if (nready == 0) continue;

        while (nready) {
            // printf("nready = %d\n", nready);
            // printf("which timer is expired\n");
            print_elapsed_time();
            printf("\n");
            if (FD_ISSET(fd1, &rfds)) {
                read(fd1, &exp, sizeof(uint64_t));
                printf("\ttimer1 read: %" PRIu64 "\n", exp);
                nready--;
            }
            if (FD_ISSET(fd2, &rfds)) {
                read(fd2, &exp, sizeof(uint64_t));
                printf("\ttimer2 read: %" PRIu64 "\n", exp);
                nready--;
            }
            if (FD_ISSET(fd3, &rfds)) {
                read(fd3, &exp, sizeof(uint64_t));
                printf("\ttimer3 read: %" PRIu64 "\n", exp);
                nready--;
            }
        }
    }
    exit(EXIT_SUCCESS);
}
