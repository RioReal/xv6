#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    char buffer[1024];

    // create two pipe
    int fd1[2];
    int fd2[2];
    if (pipe(fd1) < 0 || pipe(fd2) < 0)
    {
        exit(1);
    }

    // create a child
    int pid = fork();

    if (pid < 0)
    {
        exit(-1);
    }
    else if (pid == 0)
    {
        // child read, print and write
        close(fd1[1]);
        read(fd1[0], buffer, sizeof(buffer));
        fprintf(2, "%d: received %s\n", getpid(), buffer);

        close(fd2[0]);
        write(fd2[1], "pong", sizeof(buffer));
    }
    else
    {
        // parent write, read and print
        close(fd1[0]);
        write(fd1[1], "ping", sizeof(buffer));

        close(fd2[1]);
        read(fd2[0], buffer, sizeof(buffer));
        fprintf(2, "%d: received %s\n", getpid(), buffer);
    }

    exit(0);
}
