#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc,char **argv)
{
    int fd,num;

    fd=open(argv[2],O_RDONLY);
    sscanf(argv[1],"%o",&num);
    fchmod(fd,num);

  return 0;
}