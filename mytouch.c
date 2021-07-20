#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <utime.h>
int main(int argc,char **argv)
{
    int fd;
    fd=open(argv[1],O_RDONLY);
    if(fd==-1)
    {
        fd=open(argv[1],O_RDONLY|O_CREAT,0644);
    }
    else
    {
        utime(argv[1],NULL);
    }
    
    return 0;
}