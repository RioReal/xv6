
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    char *exec_argv[MAXARG];
    char arg[MAXPATH];
    int cnt = 0;
    int arg1, exec_argc;
    char p;

    memmove(exec_argv, argv + 1, (argc - 1) * sizeof(char *));

    while (read(0, &p, 1))
    {
        exec_argc = argc - 1;
        arg1 = 0;

        if (p == ' ' || p == '\n')
        {
            arg[cnt++] = 0;
            exec_argv[exec_argc++] = &arg[arg1];
            arg1 = cnt;
            if (p == '\n')
            {
                exec_argv[exec_argc] = 0;
                int pid = fork();
                if (pid < 0)
                {
                    exit(0);
                }
                else if (pid == 0)
                {
                    exec(exec_argv[0], exec_argv);
                }
                else
                {
                    wait(0);
                }
                cnt = 0;
            }
        }
        else
        {
            arg[cnt++] = p;
        }
    }

    exit(0);
}