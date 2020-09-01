#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void find(char *path, char *name)
{
    char buf[512], sub_path[DIRSIZ + 1], *p;
    int fd, fd2;
    struct dirent de;
    struct stat st, st2;

    //printf("%s\n", name);
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        printf("find: path too long\n");
        return;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if ((fd2 = open(buf, 0)) < 0)
        {
            fprintf(2, "find: cannot open %s\n", buf);
            continue;
        }

        if (fstat(fd2, &st2) < 0)
        {
            fprintf(2, "find: cannot stat %s\n", buf);
            close(fd2);
            continue;
        }

        char a[DIRSIZ + 1];
        memmove(a, fmtname(buf), DIRSIZ + 1);

        char b[DIRSIZ + 1];
        memmove(b, fmtname(name), DIRSIZ + 1);

        if (st2.type == T_DIR)
        {
            strcpy(sub_path, path);
            char *p2 = sub_path + strlen(sub_path);
            *p2++ = '/';
            memmove(p2, de.name, DIRSIZ);
            find(sub_path, name);
        }

        if (strcmp(a, b) == 0)
        {
            printf("%s\n", buf);
        }
        close(fd2);
    }

    close(fd);
}

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        printf("find: wrong input");
        exit(0);
    }
    else
    {
        find(argv[1], argv[2]);
    }
    exit(0);
}
