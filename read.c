#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc,char* argv[])
{
    int fd,cnt=0,offset,byte;
    char c;
    
     if(argc!=4)
    {
        printf("Error\n");
        return 0;
    }
    
    offset=atoi(argv[2]);
    byte=atoi(argv[3]);

    fd=open(argv[1],O_RDONLY);
    lseek(fd,offset,SEEK_SET);
    while(cnt < byte)
    {
        if (read(fd,&c,1)>0)
        {
            putchar(c);
            cnt++;
        }
        else
        {
            break;
        }
    }

    return 0;
}