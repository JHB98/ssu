#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
    int fd1, fd2, fd3, len;
    char buf[100];

    if (argc != 4)
    {
        printf("ERROR\n");
        return 0;
    }

    fd1 = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fd2 = open(argv[2], O_RDONLY);
    fd3 = open(argv[3], O_RDONLY);

    while ((len = read(fd2, buf, 100)) > 0)
    {
        write(fd1, buf, len);
    }

    while ((len = read(fd3, buf, 100)) > 0)
    {
        write(fd1, buf, len);
    }

    return 0;
}