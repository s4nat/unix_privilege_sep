#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include "utils.h"

int privsep(char **argv)
{

    int fd, n;
    fprintf(stderr, "demo2 - I'm the child, uid: %d, gid: %d\n", getuid(), getgid());

    if (recvfd(atoi(argv[1]), &n, sizeof(n), &fd) <= 0 || fd < 0)
        err(1, "recvfd");

    // read only first 128 byte
    char buf[128];
    read(fd, buf, 128);
    printf("%s\n", buf);
    close(fd);
    char buf[128];

    // try to read another file not passed through socket
    const char *sd = "/etc/sudoers";
    int file = open(sd, O_RDONLY);
    if (file < 0)
    {
        fprintf(stderr, "demo2 - Error opening file %s\n", sd);
        exit(1);
    }
    else
    {
        fprintf(stderr, "demo2 - File %s opened\n", sd);
        read(file, buf, 128);
        printf("%s\n", buf);
        close(file);
    }
    return 0;
}

int main(int argc, char **argv)
{
    const char *fn = "/etc/shadow";

    if (argc > 1)
    {
        return privsep(argv);
    }

    // try to open a privilege file
    int fd = open(fn, O_RDONLY);
    if (fd < 0)
    {
        fprintf(stderr, "demo2 - Error opening file %s\n", fn);
        exit(1);
    }
    fprintf(stderr, "demo2 File %s opened\n", fn);
    return 0;
}
