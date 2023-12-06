#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(void)
{
    int   fd[2];
    pid_t pid;

    if (pipe(fd) < 0) {
        fprintf(stderr, "Failed on pipe\n");
        return -1;
    }

    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Failed on fork\n");
        return -1;
    }

    if (pid == 0) {                 // 子进程
        dup2(fd[1], STDOUT_FILENO); // STDOUT -> pipe 写端点
        close(fd[0]);               // 关闭 STDIN

        char *args[] = { "bc", NULL };
        execvp(args[0], args); // 调用 bc , 接受用户输入，输出给管道
        fprintf(stderr, "child failed on execvp bc\n");
        return -1;
    } else {                       // 父进程
        close(fd[1]);              // 关闭 STDOUT
        dup2(fd[0], STDIN_FILENO); // STDIN -> pipe 读端点

        // char *args[] = { "/data/playground/APLE/APLE_github_repo/src/"
        //                  "lecture02_filesystem/myquit", NULL };
        char *args[] = { "bc", NULL };
        execvp(args[0], args); // 调用 bc , 接受管道输入
        fprintf(stderr, "parant failed on execvp bc\n");
        return -1;
    }

    wait(NULL);

    return 0;
}

// This C program creates a pipe `fd[2]` and a fork.The child process
//     runs `bc` command with its standard output duplicated to the write
// - end of the pipe.The parent then closes the              read
// - end of the pipe and reassigns its standard input to the read
// - end of the                                              pipe.
//
//   Do note that the error checking in this example code is pretty minimal,
// and doesn't handle all error possibilities, making a robust program would
// require much more care.
//
// Also,
// remember to compile this code with - Wall
//     - g flags if want debuging info and all the libraries should be
//           installed at required directories
//               .Also need to have bc installed in the linux
//                   system.This program can perform arithmetic operations.
