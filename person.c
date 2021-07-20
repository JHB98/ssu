#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "person.h"
void readPage(FILE *fp, char *pagebuf, int pagenum)
{
    fseek(fp, 16 + (pagenum * PAGE_SIZE), SEEK_SET);
    fread(pagebuf, PAGE_SIZE, 1, fp);
}
void writePage(FILE *fp, const char *pagebuf, int pagenum)
{
    fseek(fp, 16 + (pagenum * PAGE_SIZE), SEEK_SET);
    fwrite(pagebuf, PAGE_SIZE, 1, fp);
}
void pack(char *recordbuf, const Person *p)
{
    strcat(recordbuf, p->id);
    strcat(recordbuf, "#");
    strcat(recordbuf, p->name);
    strcat(recordbuf, "#");
    strcat(recordbuf, p->age);
    strcat(recordbuf, "#");
    strcat(recordbuf, p->addr);
    strcat(recordbuf, "#");
    strcat(recordbuf, p->phone);
    strcat(recordbuf, "#");
    strcat(recordbuf, p->email);
    strcat(recordbuf, "#");
}
void unpack(char *recordbuf, Person *p)
{
    char *token = strtok(recordbuf, "#");
    strcpy(p->id, token);
    token = strtok(NULL, "#");
    strcpy(p->name, token);
    token = strtok(NULL, "#");
    strcpy(p->age, token);
    token = strtok(NULL, "#");
    strcpy(p->addr, token);
    token = strtok(NULL, "#");
    strcpy(p->phone, token);
    token = strtok(NULL, "#");
    strcpy(p->email, token);
    memset(recordbuf, 0, sizeof(recordbuf));
}
void add(FILE *fp, const Person *p)
{
    char recordbuf[MAX_RECORD_SIZE] = {0};
    char pagebuf[PAGE_SIZE] = {0};
    int deletePageNum = 0, deleteRecordNum, dataPageNum, allRecordNum, recordNum, offset, length, sum = 0, save;
    pack(recordbuf, p);
    fseek(fp, 8, SEEK_SET);
    fread(&deletePageNum, 4, 1, fp);
    if (deletePageNum == -1) //삭제 레코드가 없을 때
    {
        fseek(fp, 0, SEEK_SET);
        fread(&dataPageNum, 4, 1, fp);
        if (dataPageNum == 0) //데이터 페이지도 없을때(최초)
        {
            recordNum = 1;
            offset = 0;
            length = strlen(recordbuf);
            memcpy(pagebuf, &recordNum, 4);
            memcpy(pagebuf + 4, &offset, 4);
            memcpy(pagebuf + 8, &length, 4);
            memcpy(pagebuf + HEADER_AREA_SIZE, recordbuf, length);
            writePage(fp, pagebuf, 0);
            dataPageNum = 1;
            allRecordNum = 1;
            fseek(fp, 0, SEEK_SET);
            fwrite(&dataPageNum, 4, 1, fp);
            fwrite(&allRecordNum, 4, 1, fp);
        }
        else // 마지막 데이터 페이지에 append
        {
            fseek(fp, 0, SEEK_SET);
            fread(&dataPageNum, 4, 1, fp);
            readPage(fp, pagebuf, dataPageNum - 1);
            memcpy(&recordNum, pagebuf, 4);
            if ((4 + (recordNum * 8)) <= (HEADER_AREA_SIZE - 8)) //마지막 데이터 페이지에 쓸 수 있을 때
            {
                for (int i = 0; i < recordNum; i++) //데이터 area도 비어 있는지?
                {
                    memcpy(&length, (pagebuf + 8 + (i * 8)), 4);
                    sum += length;
                }
                if ((sum + strlen(recordbuf)) <= DATA_AREA_SIZE) //쓸 수 있다!
                {
                    memcpy(&offset, (pagebuf + 4 + ((recordNum - 1) * 8)), 4);
                    memcpy(&length, (pagebuf + 4 + ((recordNum - 1) * 8)) + 4, 4);
                    offset += length;
                    length = strlen(recordbuf);
                    memcpy(pagebuf + HEADER_AREA_SIZE + offset, recordbuf, length);
                    memcpy(pagebuf + 4 + (recordNum * 8), &offset, 4);
                    memcpy((pagebuf + 4 + (recordNum * 8) + 4), &length, 4);
                    recordNum++;
                    memcpy(pagebuf, &recordNum, 4);
                    writePage(fp, pagebuf, dataPageNum - 1);
                    fseek(fp, 4, SEEK_SET);
                    fread(&allRecordNum, 4, 1, fp);
                    allRecordNum++;
                    fseek(fp, 4, SEEK_SET);
                    fwrite(&allRecordNum, 4, 1, fp);
                }
            }
            else //마지막 데이터 페이지가 꽉차서 쓸 수 없을 때
            {
                memset(pagebuf, 0, sizeof(pagebuf));
                fseek(fp, 0, SEEK_SET);
                fread(&dataPageNum, 4, 1, fp);
                recordNum = 1;
                offset = 0;
                length = strlen(recordbuf);
                memcpy(pagebuf, &recordNum, 4);
                memcpy(pagebuf + 4, &offset, 4);
                memcpy(pagebuf + 8, &length, 4);
                memcpy(pagebuf + HEADER_AREA_SIZE, recordbuf, length);
                writePage(fp, pagebuf, dataPageNum);
                fseek(fp, 4, SEEK_SET);
                fread(&allRecordNum, 4, 1, fp);
                allRecordNum++;
                dataPageNum++;
                fseek(fp, 0, SEEK_SET);
                fwrite(&dataPageNum, 4, 1, fp);
                fwrite(&allRecordNum, 4, 1, fp);
            }
        }
    }
    else //삭제 레코드가 있을 때
    {
        fread(&deleteRecordNum, 4, 1, fp);
        while (1)
        {
            if (deletePageNum == -1) // 삭제 리스트에서 못찾아서 append
            {
                fseek(fp, 0, SEEK_SET);
                fread(&dataPageNum, 4, 1, fp);
                readPage(fp, pagebuf, dataPageNum - 1);
                memcpy(&recordNum, pagebuf, 4);
                if ((4 + (recordNum * 8)) <= (HEADER_AREA_SIZE - 8)) //마지막 데이터 페이지에 쓸 수 있을 때
                {
                    for (int i = 0; i < recordNum; i++) //데이터 area도 비어 있는지?
                    {
                        memcpy(&length, (pagebuf + 8 + (i * 8)), 4);
                        sum += length;
                    }
                    if ((sum + strlen(recordbuf)) <= DATA_AREA_SIZE) //쓸 수 있다!
                    {
                        memcpy(&offset, (pagebuf + 4 + ((recordNum - 1) * 8)), 4);
                        memcpy(&length, (pagebuf + 4 + ((recordNum - 1) * 8)) + 4, 4);
                        offset += length;
                        length = strlen(recordbuf);
                        memcpy(pagebuf + HEADER_AREA_SIZE + offset, recordbuf, length);
                        memcpy(pagebuf + 4 + (recordNum * 8), &offset, 4);
                        memcpy((pagebuf + 4 + (recordNum * 8) + 4), &length, 4);
                        recordNum++;
                        memcpy(pagebuf, &recordNum, 4);
                        writePage(fp, pagebuf, dataPageNum - 1);
                        fseek(fp, 4, SEEK_SET);
                        fread(&allRecordNum, 4, 1, fp);
                        allRecordNum++;
                        fseek(fp, 4, SEEK_SET);
                        fwrite(&allRecordNum, 4, 1, fp);
                    }
                }
                else //마지막 데이터 페이지가 꽉차서 쓸 수 없을 때
                {
                    memset(pagebuf, 0, sizeof(pagebuf));
                    fseek(fp, 0, SEEK_SET);
                    fread(&dataPageNum, 4, 1, fp);
                    recordNum = 1;
                    offset = 0;
                    length = strlen(recordbuf);
                    memcpy(pagebuf, &recordNum, 4);
                    memcpy(pagebuf + 4, &offset, 4);
                    memcpy(pagebuf + 8, &length, 4);
                    memcpy(pagebuf + HEADER_AREA_SIZE, recordbuf, length);
                    writePage(fp, pagebuf, dataPageNum);
                    fseek(fp, 4, SEEK_SET);
                    fread(&allRecordNum, 4, 1, fp);
                    allRecordNum++;
                    dataPageNum++;
                    fseek(fp, 0, SEEK_SET);
                    fwrite(&dataPageNum, 4, 1, fp);
                    fwrite(&allRecordNum, 4, 1, fp);
                }
                break;
            }
            readPage(fp, pagebuf, deletePageNum);
            save = deletePageNum;
            memcpy(&offset, pagebuf + 4 + (8 * deleteRecordNum), 4);
            memcpy(&length, pagebuf + 8 + (8 * deleteRecordNum), 4);
            memcpy(&deletePageNum, pagebuf + HEADER_AREA_SIZE + offset + 1, 4);
            memcpy(&deleteRecordNum, pagebuf + HEADER_AREA_SIZE + offset + 5, 4);
            if (length >= strlen(recordbuf)) //들어 갈 수 있다면
            {
                fseek(fp, 8, SEEK_SET);
                fwrite(&deletePageNum, 4, 1, fp);
                fwrite(&deleteRecordNum, 4, 1, fp);
                memcpy(pagebuf + HEADER_AREA_SIZE + offset, recordbuf, strlen(recordbuf));
                writePage(fp, pagebuf, save);
                break;
            }
            memset(pagebuf, 0, sizeof(pagebuf));
        }
    }
}
void delete (FILE *fp, const char *id)
{
    Person p = {0};
    char recordbuf[MAX_RECORD_SIZE] = {0};
    char pagebuf[PAGE_SIZE] = {0};
    int deletePageNum = 0, deleteRecordNum, dataPageNum, allRecordNum, recordNum, offset, length;
    char check;
    fseek(fp, 0, SEEK_SET);
    fread(&dataPageNum, 4, 1, fp);
    for (int i = 0; i < dataPageNum; i++)
    {
        readPage(fp, pagebuf, i);
        memcpy(&recordNum, pagebuf, 4);
        for (int j = 0; j < recordNum; j++)
        {
            memcpy(&offset, pagebuf + 4 + (j * 8), 4);
            memcpy(&length, pagebuf + 4 + (j * 8) + 4, 4);
            memcpy(recordbuf, pagebuf + HEADER_AREA_SIZE + offset, length);
            memcpy(&check, recordbuf, 1);
            if (check == '*')
            {
                continue;
            }
            unpack(recordbuf, &p);
            if (!strcmp(id, p.id)) //찾으면
            {
                memcpy(recordbuf, pagebuf + HEADER_AREA_SIZE + offset, length);
                fseek(fp, 8, SEEK_SET);
                fread(&deletePageNum, 4, 1, fp);
                fread(&deleteRecordNum, 4, 1, fp);
                memcpy(recordbuf, "*", 1);
                memcpy(recordbuf + 1, &deletePageNum, 4);
                memcpy(recordbuf + 5, &deleteRecordNum, 4);
                memcpy(pagebuf + HEADER_AREA_SIZE + offset, recordbuf, length);
                writePage(fp, pagebuf, i);
                fseek(fp, 8, SEEK_SET);
                fwrite(&i, 4, 1, fp);
                fwrite(&j, 4, 1, fp);
                return;
            }
        }
    }
}
int main(int argc, char *argv[])
{
    FILE *fp; // 레코드 파일의 파일 포인터
    Person p = {0};
    int reset[4] = {0, 0, -1, -1};
    char isId[14] = {0};
    char recordbuf[100];
    if (!access(argv[2], F_OK))
    {
        fp = fopen(argv[2], "r+");
    }
    else
    {
        fp = fopen(argv[2], "w+");
        fwrite(reset, sizeof(reset), 1, fp);
    }
    if (!strcmp(argv[1], "a"))
    {
        strcpy(p.id, argv[3]);
        strcpy(p.name, argv[4]);
        strcpy(p.age, argv[5]);
        strcpy(p.addr, argv[6]);
        strcpy(p.phone, argv[7]);
        strcpy(p.email, argv[8]);
        add(fp, &p);
    }
    else if (!strcmp(argv[1], "d"))
    {
        strcpy(isId, argv[3]);
        delete (fp, isId);
    }
    return 0;
}