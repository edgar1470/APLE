#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

int
tobeDaemon(void)
{
    int   maxfd, fd;
    pid_t pid;

    // generate a child and let parent exit
    pid = fork();
    switch (pid) {
    case -1:
        syslog(LOG_ERR, "fork failed");
        return -1;
    case 0: // child
        break;
    default: // parent
        exit(EXIT_SUCCESS);
    }
    // create a session
    if (setsid() == -1) {
        syslog(LOG_ERR, "set session id failed");
        return -1;
    }

    // avoid to be the leader of the session group
    pid = fork();
    switch (pid) {
    case -1:
        syslog(LOG_ERR, "fork failed");
        return -1;
    case 0: // child
        break;
    default: // parent
        exit(EXIT_SUCCESS);
    }

    umask(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    chdir("/");
    maxfd = sysconf(_SC_OPEN_MAX);
    if (maxfd == -1) maxfd = 8192;
    for (fd = 0; fd < maxfd; fd++) {
        close(fd); // close all opend files
    }
    // reopen stdin,stdout,stderr to /dev/null
    fd = open("/dev/null", O_RDWR);
    if (fd != STDIN_FILENO) {
        syslog(LOG_ERR, "reopen stdin failed");
        return -1;
    }
    if (dup2(fd, STDOUT_FILENO) != STDOUT_FILENO) {
        syslog(LOG_ERR, "dup stdout failed");
        return -1;
    }
    if (dup2(fd, STDERR_FILENO) != STDERR_FILENO) {
        syslog(LOG_ERR, "dup stderr failed");
        return -1;
    }
    syslog(LOG_INFO, "new daemon[%d] start", getpid());
    return 0; // sucess to be a daemon
}

int
main()
{
    tobeDaemon();
    for (int i = 0; i < 10; i++) {
        sleep(1);
        syslog(LOG_INFO, "daemon [%d] running", getpid());
    }
    syslog(LOG_INFO, "daemon exit");

    return 0;
}
