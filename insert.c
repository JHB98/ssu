#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
int main(int argc,char* argv[])
{
    int fd,offset,Fsize;
    char* buff;
    
     if(argc!=4)
    {
        printf("Error\n");
        return 0;
    }

    fd=open(argv[1],O_RDWR|O_CREAT,0644);
    offset=atoi(argv[2]);
    Fsize = lseek(fd,0,SEEK_END);
    buff=(char*)malloc(sizeof(char)*(Fsize-offset));
    lseek(fd,offset,SEEK_SET);
    read(fd,buff,(Fsize-offset));
    lseek(fd,offset,SEEK_SET);
    write(fd,argv[3],strlen(argv[3]));
    write(fd,buff,(Fsize-offset));
    
    free(buff);

    return 0;
}