#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc,char *argv[])
{
    int fd1,fd2,len;
    char buf[10]={0};

    fd1=open(argv[1],O_RDONLY);

    for(int i=0;i<strlen(argv[1]);i++)
    {
        if(argv[1][i]==tolower(argv[1][i]))
        {
            argv[1][i]=toupper(argv[1][i]);
        }
        else if(argv[1][i]==toupper(argv[1][i]))
        {
            argv[1][i]=tolower(argv[1][i]);
        }
    }

    fd2=open(argv[1],O_WRONLY | O_CREAT | O_TRUNC,0644);

    while((len=read(fd1,buf,1))>0)
    {
        if(buf[0]==tolower(buf[0]))
        {
            buf[0]=toupper(buf[0]);
        }
        else if(buf[0]==toupper(buf[0]))
        {
            buf[0]=tolower(buf[0]);
        }
        write(fd2,buf,1);
    }
    
    return 0;
}