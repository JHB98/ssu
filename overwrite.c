#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
    int fd, offset;

    if (argc != 4)
    {
        printf("Error\n");
        return 0;
    }

    fd = open(argv[1], O_WRONLY | O_CREAT, 0644);
    offset = atoi(argv[2]);
    lseek(fd, offset, SEEK_SET);
    write(fd, argv[3], strlen(argv[3]));

    return 0;
}