#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
    int fd1, fd2, len;
    char buf[10];

    if (argc != 3)
    {
        printf("Wrong number of files\n");
        return 0;
    }
    fd1 = open(argv[1], O_RDONLY);
    fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    while ((len = read(fd1, buf, 10)) > 0)
    {
        write(fd2, buf, len);
    }

    return 0;
}