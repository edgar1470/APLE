// Firstly, Pseudo Terminal (PTY) is an interface for communicating with a
// program running in a virtual terminal. It's essentially a pair of
// bidirectional pipes that are used to interact with an 'end-point' in the
// communication.
//
// [The TTY demystified](http://www.linusakesson.net/programming/tty/index.php)
//
// clang-format off
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pty.h>
// clang-format on 

int
main(void)
{
    int   ptym, ptys;
    char  buf[1024];
    int   ret;

    /* Open a PTY */
    ret = openpty(&ptym, &ptys, NULL, NULL, NULL);
    if (ret != 0) {
        perror("openpty() failed");
        exit(EXIT_FAILURE);
    }

    /* Read the PTY Slave name */
    ret = ptsname_r(ptym, buf, 1024);
    if (ret != 0) {
        perror("ptsname_r failed");
        exit(EXIT_FAILURE);
    }
    printf("The PTY slave is: %s\n", buf);

    pid_t pid = fork();
    if (pid == -1) {
        fprintf(stderr, "Failed on fork\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) { // 子进程
        dup2(ptys, 0); // 子进程的 stdin/stdou/stderr 都指向 ptys
        dup2(ptys, 1);
        dup2(ptys, 2);
        execlp("bc", "bc", NULL); // 调用 bc 
    } else { // 父进程
        execlp("bc", "bc", NULL); // 调用 bc 
    }
    wait(NULL);
    close(ptym);
    close(ptys);
    return EXIT_SUCCESS;
}

// In this code,
// we open a PTY and then fork to a new process.We then replace the `stdin`,
// `stdout`, and `stderr` of the child with the slave `pty` file descriptor,
// so the standard input / output of the `bc` calculator are now in the
// `pty`.The child then runs `bc` as the calculator.
//
// For the parent, we now open   the `masterpty` file descriptor(the master
// side of our PTY) and write out the calculation expressions, then print
// the results from the child process.
//
// You must link the program with ` - lutil` to make use of `openpty()` and
// `ptsname()`.
//
// This program will print the results of the expressions, which the bc
// calculator reads from the `pty`.
