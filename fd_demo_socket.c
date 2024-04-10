#define _GNU_SOURCE
#include <stdio.h>
#include <sys/socket.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "utils.h"
#include <grp.h>

int main(int argc, char **argv)
{
    int fds[2];
    int pid;
    int n = 1;
    char *childargs[10] = {0};
    // create socket pair to send file descriptor
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds))
        err(1, "socketpair");
    childargs[0] = "./demo2";

    switch (pid = fork())
    {
    case -1:
        err(1, "fork");
    case 0: // child
        close(fds[0]);
        asprintf(&childargs[1], "%d", fds[1]); // clear supplementary groups
        setgroups(0, NULL);
        // drop privileges
        if (setresgid(1000, 1000, 1000) != 0)
            err(1, "set GID");
        if (setresuid(1000, 1000, 1000) != 0)
            err(1, "set UID");
        // execute child
        execv(childargs[0], childargs);
        break;
    default: // parent
        close(fds[1]);
        // open then send this descriptor over
        int file = open("/etc/shadow", O_RDONLY);
        if (sendfd(fds[0], &n, sizeof(n), (int)file) < 0)
            err(1, "sendfd");
        close(file);
    }

    return 0;
}
