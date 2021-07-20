#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int compare(const void *a, const void *b);

int main(int argc,char *argv[])
{
    int num,fd1,fd2,*randnum,cnt=0,index=0;
    char buf[10]={0};
    printf("number : ");
    scanf("%d",&num);
    srand(time(NULL));
    fd1=open("random.txt",O_RDWR | O_CREAT | O_TRUNC,0644);
    fd2=open("sort.txt",O_WRONLY | O_CREAT | O_TRUNC,0644);

    randnum=(int *)malloc(sizeof(int)*num);

    for(int i=0;i<num;i++)
    {
        randnum[i]=(rand()%100)+1;
        sprintf(buf,"%d",randnum[i]);
        write(fd1,buf,strlen(buf));
        write(fd1," ",1);
        if(((i%10)==0)&&(i!=0))
        {
            write(fd1,"\n",1);
        }
    }

    while(read(fd1,buf,1)>0)
    {
        if(buf[cnt]==' ')
        {
            randnum[index]=atoi(buf);
            index++;
            memset(buf,0,10*sizeof(char));
            cnt=0;
            continue;
        }
        cnt++;
    }

    qsort(randnum,num,sizeof(int),compare);

    for(int i=0;i<num;i++)
    {
        sprintf(buf,"%d",randnum[i]);
        write(fd2,buf,strlen(buf));
        write(fd2," ",1);
        if(((i%10)==0)&&(i!=0))
        {
            write(fd2,"\n",1);
        }
    }

    free(randnum);
    return 0;
}

int compare(const void *a, const void *b)
{
    int num1 = *(int *)a;
    int num2 = *(int *)b;   

    if (num1 < num2)
    {
        return -1;      
    }
    
    if (num1 > num2) 
    {
        return 1; 
    }
    return 0;
}