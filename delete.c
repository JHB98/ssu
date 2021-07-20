#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
    int fd, offset, Fsize, byte;
    char *buff1, *buff2;

    if (argc != 4)
    {
        printf("Error\n");
        return 0;
    }

    fd = open(argv[1], O_RDONLY);
    offset = atoi(argv[2]);
    byte = atoi(argv[3]);
    Fsize = lseek(fd, 0, SEEK_END);
    buff1 = (char *)malloc(sizeof(char) * offset);
    buff2 = (char *)malloc(sizeof(char) * (Fsize - (offset + byte)));
    lseek(fd, 0, SEEK_SET);
    read(fd, buff1, offset);
    lseek(fd, (offset + byte), SEEK_SET);
    read(fd, buff2, (Fsize - (offset + byte)));
    close(fd);

    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, buff1, offset);
    write(fd, buff2, (Fsize - (offset + byte)));

    free(buff1);
    free(buff2);

    return 0;
}