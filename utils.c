int rcvfd(int child_fd, int *n, unsigned long size, int *file)
{
    //
    read(child_fd, file);
}

int sendfd(int parent_fd, int *n, unsigned long size, int file)
{
    write()
    // send write stringified int file to file at parent_fd
}
