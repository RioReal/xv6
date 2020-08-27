#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int *fd);

int main(int argc, char *argv[])
{
    int i = 2;
    int fd[2];

    if (pipe(fd) < 0)
    {
        exit(0);
    }

    int pid = fork();

    if (pid < 0)
    {
        exit(0);
    }
    else if (pid == 0)
    {
        // child read, print, send to right
        sieve(fd);
    }
    else
    {
        // write numbers to right, then wait.
        close(fd[0]);
        for (; i < 35; i++)
        {
            write(fd[1], &i, 4);
        }
        close(fd[1]);
        wait(0);
        exit(0);
    }
    exit(0);
}

void sieve(int *fd)
{
    int p, n;
    int fd2[2];

    // read the prime
    close(fd[1]);
    if (read(fd[0], &p, 4) != 0)
    {
        fprintf(2, "prime: %d\n", p);
    }
    else
    {
        exit(0);
    }

    if (pipe(fd2) < 0)
    {
        exit(0);
    }

    // recurse
    int pid = fork();

    if (pid < 0)
    {
        exit(0);
    }
    else if (pid == 0)
    {
        sieve(fd2);
    }
    else
    {
        close(fd2[0]);
        while (read(fd[0], &n, 4) != 0)
        {
            if (n % p != 0)
            {
                write(fd2[1], &n, 4);
            }
        }
        close(fd2[1]);
        wait(0);
        exit(0);
    }
}