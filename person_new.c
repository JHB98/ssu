#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "person.h"
typedef struct
{
    char id[14];
    int pageNum;
    int recordNum;
} Index;
int compare(const void *index1, const void *index2)
{
    if (strlen(((Index *)index1)->id) < strlen(((Index *)index2)->id))
    {
        return -1;
    }
    else if (strlen(((Index *)index1)->id) > strlen(((Index *)index2)->id))
    {
        return 1;
    }
    else
    {
        return strcmp(((Index *)index1)->id, ((Index *)index2)->id);
    }
}
void readPage(FILE *fp, char *pagebuf, int pagenum)
{
    fseek(fp, 16 + (pagenum * PAGE_SIZE), SEEK_SET);
    fread(pagebuf, PAGE_SIZE, 1, fp);
}
void unpack(char *recordbuf)
{
    char *token = strtok(recordbuf, "#");
    printf("id = %s\n", token);
    token = strtok(NULL, "#");
    printf("name = %s\n", token);
    token = strtok(NULL, "#");
    printf("age = %s\n", token);
    token = strtok(NULL, "#");
    printf("addr = %s\n", token);
    token = strtok(NULL, "#");
    printf("phone = %s\n", token);
    token = strtok(NULL, "#");
    printf("email = %s\n", token);
    memset(recordbuf, 0, sizeof(recordbuf));
}
void createIndex(FILE *idxfp, FILE *recordfp)
{
    int pageNum, recordNum, offset, indexNum = 0;
    char pagebuf[PAGE_SIZE], indexbuf[21], check, ref[13];
    Index *array;

    fseek(recordfp, 0, SEEK_SET);
    fread(&pageNum, 4, 1, recordfp);
    fseek(idxfp, 0, SEEK_SET);
    fwrite(&indexNum, sizeof(indexNum), 1, idxfp);
    for (int i = 0; i < pageNum; i++)
    {
        memset(pagebuf, 0, sizeof(pagebuf));
        readPage(recordfp, pagebuf, i);
        memcpy(&recordNum, pagebuf, 4);
        for (int j = 0; j < recordNum; j++)
        {
            memcpy(&offset, pagebuf + 4 + (8 * j), 4);
            memcpy(&check, pagebuf + HEADER_AREA_SIZE + offset, 1);
            if (check == '*')
            {
                continue;
            }
            memset(indexbuf, 0, sizeof(indexbuf));
            memset(ref, 0, sizeof(ref));
            sscanf(pagebuf + HEADER_AREA_SIZE + offset, "%[^#]", ref);
            memcpy(indexbuf, ref, 13);
            memcpy(indexbuf + 13, &i, 4);
            memcpy(indexbuf + 17, &j, 4);
            fwrite(indexbuf, sizeof(indexbuf), 1, idxfp);
            indexNum++;
        }
    }
    fseek(idxfp, 0, SEEK_SET);
    fwrite(&indexNum, sizeof(indexNum), 1, idxfp);
    array = (Index *)malloc(indexNum * sizeof(Index));
    for (int i = 0; i < indexNum; i++)
    {
        fread(&array[i].id, 13, 1, idxfp);
        array[i].id[13] = '\0';
        fread(&array[i].pageNum, 4, 1, idxfp);
        fread(&array[i].recordNum, 4, 1, idxfp);
    }
    qsort(array, indexNum, sizeof(Index), compare);
    fseek(idxfp, 4, SEEK_SET);
    for (int i = 0; i < indexNum; i++)
    {
        memset(indexbuf, 0, sizeof(indexbuf));
        memcpy(indexbuf, array[i].id, 13);
        memcpy(indexbuf + 13, &array[i].pageNum, 4);
        memcpy(indexbuf + 17, &array[i].recordNum, 4);
        fwrite(indexbuf, sizeof(indexbuf), 1, idxfp);
    }
}
void binarysearch(FILE *idxfp, const char *id, int *pageNum, int *recordNum)
{
    char refId[14];
    int min = 0, mid, max, ans = -1, cnt = 0;
    fseek(idxfp, 0, SEEK_SET);
    fread(&max, 4, 1, idxfp);
    max--;
    while (min <= max)
    {
        cnt++;
        mid = (min + max) / 2;
        fseek(idxfp, 4 + (21 * mid), SEEK_SET);
        memset(refId, 0, sizeof(refId));
        fread(refId, 13, 1, idxfp);
        refId[13] = '\0';
        if (strlen(id) > strlen(refId))
        {
            min = mid + 1;
        }
        else if (strlen(id) < strlen(refId))
        {
            max = mid - 1;
        }
        else
        {
            if (strcmp(id, refId) > 0)
            {
                min = mid + 1;
            }
            else if (strcmp(id, refId) < 0)
            {
                max = mid - 1;
            }
            else
            {
                ans = mid;
                break;
            }
        }
    }
    printf("#reads: %d\n", cnt);
    if (ans == -1)
    {
        return;
    }
    else
    {
        fseek(idxfp, 4 + (21 * ans) + 13, SEEK_SET);
        fread(pageNum, 4, 1, idxfp);
        fread(recordNum, 4, 1, idxfp);
        return;
    }
}
int main(int argc, char *argv[])
{
    FILE *fp, *idxfp;
    char id[14] = {0}, pagebuf[PAGE_SIZE], recordBuf[MAX_RECORD_SIZE];
    int pageNum = -1, recordNum = -1, offset, length;
    if (!strcmp(argv[1], "i"))
    {
        fp = fopen(argv[2], "r");
        idxfp = fopen(argv[3], "w+");
        createIndex(idxfp, fp);
    }
    else if (!strcmp(argv[1], "b"))
    {
        fp = fopen(argv[2], "r");
        idxfp = fopen(argv[3], "r");
        strcpy(id, argv[4]);
        binarysearch(idxfp, id, &pageNum, &recordNum);
        if (pageNum == -1)
        {
            printf("no persons\n");
        }
        else
        {
            memset(pagebuf, 0, sizeof(pagebuf));
            memset(recordBuf, 0, sizeof(recordBuf));
            readPage(fp, pagebuf, pageNum);
            memcpy(&offset, pagebuf + 4 + (8 * recordNum), 4);
            memcpy(&length, pagebuf + 8 + (8 * recordNum), 4);
            memcpy(recordBuf, pagebuf + HEADER_AREA_SIZE + offset, length);
            unpack(recordBuf);
        }
    }
    return 0;
}