#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0
int main(int argc,char *argv[])
{
    int fd,check=false,number;
    char sign[10], filename[100];

    printf("number sign filename\n");
    scanf("%d %s %s",&number,sign,filename);

    fd=open(filename,O_WRONLY | O_CREAT | O_TRUNC,0644);

    if(number<0)
    {
        check=true;
        number=abs(number);
    }

    if(check)
    {
        for(int i=number;i>0;i--)
        {
            for(int j=i;j>0;j--)
            {
                write(fd,sign,strlen(sign));
            }
            write(fd,"\n",1);
        }
    }

    else
    {
        for(int i=0;i<number;i++)
        {
            for(int j=0;j<i+1;j++)
            {
                write(fd,sign,strlen(sign));
            }
            write(fd,"\n",1);
        }
    }
    return 0;
}