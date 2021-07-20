#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>
#include <stdlib.h>
typedef struct entry
{
    char *filename;
    struct stat st;
}Entry;
void normalPrint(DIR *dp,char *name);
int compare(const void *ent1,const void *ent2);
int compare2(const void *str1,const void *str2);
int compare3(const void *ent1,const void *ent2);
void iPrint(DIR *dp,char *name,int fd);
void lPrint(DIR *dp,char *name,int fd);
void tPrint(DIR *dp,char *name,int fd);
void type(struct stat st);
void perm(struct stat st);
int main(int argc,char *argv[])
{
    int fd;
    char *order;
    char od[1000]={'.'};
    DIR *dp;
    if(argc==1)
    {
        dp=opendir(".");
        normalPrint(dp,argv[0]);
    }
    else if(argc==2)
    {
        dp=opendir(argv[1]);
        if(dp==NULL)
        {
            if(!strcmp(argv[1],"-i"))
            {
                dp=opendir(".");
                iPrint(dp,od,fd);
                return 0;
            }
            
            if(!strcmp(argv[1],"-l"))
            {
                dp=opendir(".");
                lPrint(dp,od,fd);
                return 0;
            }

            if(!strcmp(argv[1],"-t"))
            {
                dp=opendir(".");
                tPrint(dp,od,fd);
                return 0;
            }

            fd=open(argv[1],O_RDONLY);
            if(fd==-1)
            {
                printf("ERROR\n");
                return 0;
            }
        }
        normalPrint(dp,argv[1]);
    }
    else//argc==3
    {
        order=argv[1];
        dp=opendir(argv[2]);
        if(dp==NULL)
        {
            fd=open(argv[2],O_RDONLY);
            if(fd==-1)
            {
                printf("ERROR\n");
                return 0;
            }
        }
        if(!strcmp(order,"-i"))
        {
            iPrint(dp,argv[2],fd);
        }
        if(!strcmp(order,"-l"))
        {
            lPrint(dp,argv[2],fd);
        }
        if(!strcmp(order,"-t"))
        {
            tPrint(dp,argv[2],fd);
        }
    }
    return 0;
}
void normalPrint(DIR *dp,char *name)
{
    struct dirent* d;
    char * filename[10000]={0};
    int index=0;
    if(dp!=NULL)
    {
        while(1)
        {
            d=readdir(dp);
            if(d==NULL)
            {
                break;
            }

            if(!strcmp(d->d_name,".")||!strcmp(d->d_name,".."))
            {
                continue;
            }

            filename[index++]=d->d_name;
        }
        qsort(filename,index,sizeof(char*),compare2);

        for(int i=0;i<index;i++)
        {
            printf("%-20s",filename[i]);
            if((!(i%4))&&(i!=0))
            {
                printf("\n");
            }
        }
        printf("\n");
    }

    else
    {
        printf("%-20s\n",name);
    }
}

void iPrint(DIR *dp,char *name,int fd)
{
    struct dirent* d;
    struct stat refst;
    Entry entry[10000];
    int index=0;
    char save_name[1000];
    strcpy(save_name,name);
    if(dp!=NULL)
    {
        while(1)
        {
            strcpy(name,save_name);
            d=readdir(dp);
            if(d==NULL)
            {
                break;
            }
            if(!strcmp(d->d_name,".")||!strcmp(d->d_name,".."))
            {
                continue;
            }
            entry[index].filename=d->d_name;
            strcat(name,"/");
            strcat(name,d->d_name);
            fd=open(name,O_RDONLY);
            fstat(fd,&refst);
            entry[index++].st=refst;
        }
        qsort(entry,index,sizeof(Entry),compare3);

        for(int i=0;i<index;i++)
        {
            printf("%ld %-20s",entry[i].st.st_ino,entry[i].filename);
            if((!(i%4))&&(i!=0))
            {
                printf("\n");
            }
        }
        printf("\n");
    }
    else
    {
        fstat(fd,&refst);
        printf("%ld %-20s\n",refst.st_ino,name);
    }
}

void lPrint(DIR *dp,char *name,int fd)
{
    Entry entry[10000];
    int index=0;
    struct dirent *d;
    struct stat refst;
    char save_name[1000];
    strcpy(save_name,name);
    if(dp!=NULL)
    {
        while(1)
        {
            strcpy(name,save_name);
            d=readdir(dp);
            if(d==NULL)
            {
                break;
            }
            if(!strcmp(d->d_name,".")||!strcmp(d->d_name,".."))
            {
                continue;
            }
            entry[index].filename=d->d_name;
            strcat(name,"/");
            strcat(name,d->d_name);
            fd=open(name,O_RDONLY);
            fstat(fd,&refst);
            entry[index++].st=refst;
        }

        qsort(entry,index,sizeof(Entry),compare3);

        for(int i=0;i<index;i++)
        {
            type(entry[i].st);
            perm(entry[i].st);
            printf("% ld ",entry[i].st.st_nlink);
            printf("%s %s ",getpwuid(entry[i].st.st_uid)->pw_name,getgrgid(entry[i].st.st_uid)->gr_name);
            printf("%9ld ",entry[i].st.st_size);
            printf("%.12s ",ctime(&entry[i].st.st_mtime)+4);
            printf("%s\n",entry[i].filename);
        }
    }
    else
    {
        fd=open(name,O_RDONLY);
        fstat(fd,&refst);
        type(refst);
        perm(refst);
        printf("% ld ",refst.st_nlink);
        printf("%s %s ",getpwuid(refst.st_uid)->pw_name,getgrgid(refst.st_uid)->gr_name);
        printf("%9ld ",refst.st_size);
        printf("%.12s ",ctime(&refst.st_mtime)+4);
        printf("%s\n",name);
    }
}

void tPrint(DIR *dp,char *name,int fd)
{
    struct dirent *d;
    struct stat refst;
    char save_name[1000];
    strcpy(save_name,name);
    Entry entry[10000];
    int index=0;
    if(dp!=NULL)
    {
        while(1)
        {
            strcpy(name,save_name);
            d=readdir(dp);
            if(d==NULL)
            {
                break;
            }
            if(!strcmp(d->d_name,".")||!strcmp(d->d_name,".."))
            {
                continue;
            }
            entry[index].filename=d->d_name;
            strcat(name,"/");
            strcat(name,d->d_name);
            fd=open(name,O_RDONLY);
            fstat(fd,&refst);
            entry[index++].st=refst;
        }
        qsort(entry,index,sizeof(Entry),compare);

        for(int i=0;i<index;i++)
        {
            printf("%-20s",entry[i].filename);
            if((!(i%4))&&(i!=0))
            {
                printf("\n");
            }
        }
        printf("\n");
    }

    else
    {
        printf("%-20s\n",name);
    }
}

int compare(const void *ent1,const void *ent2)
{
    if(((Entry *)ent1)->st.st_mtime > ((Entry *)ent2)->st.st_mtime)
    {
        return -1;
    }
    else if(((Entry *)ent1)->st.st_mtime < ((Entry *)ent2)->st.st_mtime)
    {
        return 1;
    }
    else
    {
        return strcmp(((Entry *)ent1)->filename,((Entry *)ent2)->filename);
    }
}

int compare2(const void *str1,const void *str2)
{
    return strcmp(*(char**)str1,*(char**)str2);
}

int compare3(const void *ent1,const void *ent2)
{
    return strcmp(((Entry*)ent1)->filename,((Entry*)ent2)->filename);
}

void type(struct stat st)
{
   if(S_ISREG(st.st_mode))
   {
       printf("-");
   }
   else if(S_ISDIR(st.st_mode))
   {
       printf("d");
   }
   else if(S_ISCHR(st.st_mode))
   {
       printf("c");
   }
   else if(S_ISBLK(st.st_mode))
   {
       printf("b");
   }
   else if(S_ISFIFO(st.st_mode))
   {
       printf("p");
   }
   else if(S_ISSOCK(st.st_mode))
   {
       printf("s");
   }
   else if(S_ISLNK(st.st_mode))
   {
       printf("|");
   }
}

void perm(struct stat st)
{
    char perms[10]="---------";
    for(int i=0;i<3;i++)
    {
        if(st.st_mode & (S_IRUSR>>i*3))
        {
            perms[i*3]='r';
        }
        if(st.st_mode & (S_IWUSR>>i*3))
        {
            perms[i*3+1]='w';
        }
        if(st.st_mode & (S_IXUSR>>i*3))
        {
            perms[i*3+2]='x';
        }
    }
    if(st.st_mode & S_ISUID)
    {
        if(perms[2]=='x')
        {
            perms[2]='s';
        }
        else
        {
            perms[2]='S';
        }
    }
     if(st.st_mode & S_ISGID)
    {
        if(perms[5]=='x')
        {
            perms[5]='s';
        }
        else
        {
            perms[5]='S';
        }
    }
     if(st.st_mode & S_ISVTX)
    {
        if(perms[8]=='x')
        {
            perms[8]='t';
        }
        else
        {
            perms[8]='T';
        }
    }
    printf("%s",perms);
}