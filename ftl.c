/*#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "blockmap.h"
int AMT[BLOCKS_PER_DEVICE][2];
int freeblockNum;
extern FILE *flashfp;
int dd_read(int ppn, char *pagebuf);
int dd_write(int ppn, char *pagebuf);
int dd_erase(int pbn);
void ftl_open()
{
    int lbn;
    char tmpbuf[PAGE_SIZE];
    memset(tmpbuf, 0xff, PAGE_SIZE);
    memset(AMT, -1, sizeof(AMT));
    freeblockNum = -1;
    for (int i = 0; i < BLOCKS_PER_DEVICE; i++)
    {
        AMT[i][0] = i;
    }
    for (int i = 0; i < BLOCKS_PER_DEVICE; i++)
    {
        dd_read(PAGES_PER_BLOCK * i, tmpbuf);
        memcpy(&lbn, tmpbuf + SECTOR_SIZE, 4);
        if ((lbn >= 0) && (lbn <= 15))
        {
            AMT[lbn][1] = i;
        }
        else if (freeblockNum == -1)
        {
            freeblockNum = i;
            AMT[BLOCKS_PER_DEVICE - 1][1] = freeblockNum;
        }
    }
    return;
}

void ftl_read(int lsn, char *sectorbuf)
{
    char pagebuf[PAGE_SIZE];
    memset(pagebuf, 0xff, PAGE_SIZE);
    int offset, lbn;
    lbn = lsn / 4;
    offset = lsn % 4;
    dd_read((AMT[lbn][1] * PAGES_PER_BLOCK) + offset, pagebuf);
    memcpy(sectorbuf, pagebuf, SECTOR_SIZE);
    return;
}

void ftl_write(int lsn, char *sectorbuf)
{
    int offset, lbn, lsn_check, ref, ref_lbn;
    char pagebuf[PAGE_SIZE], tmpbuf[PAGE_SIZE];
    memset(pagebuf, 0xff, PAGE_SIZE);
    memset(tmpbuf, 0xff, PAGE_SIZE);
    lbn = lsn / 4;
    offset = lsn % 4;
    if (AMT[lbn][1] == -1)
    {
        AMT[lbn][1] = freeblockNum;
        memcpy(pagebuf + SECTOR_SIZE, &lbn, 4);
        dd_write((AMT[lbn][1] * PAGES_PER_BLOCK), pagebuf);

        freeblockNum = -1;
        for (int i = 0; i < BLOCKS_PER_DEVICE; i++)
        {
            dd_read(i * PAGES_PER_BLOCK, tmpbuf);
            memcpy(&ref_lbn, tmpbuf + SECTOR_SIZE, 4);
            if (!((ref_lbn >= 0) && (ref_lbn <= 15)))
            {
                freeblockNum = i;
                AMT[BLOCKS_PER_DEVICE - 1][1] = freeblockNum;
                break;
            }
        }
    }
    memcpy(pagebuf, sectorbuf, SECTOR_SIZE);
    memcpy(pagebuf + SECTOR_SIZE, &lbn, 4);
    memcpy(pagebuf + SECTOR_SIZE + 4, &lsn, 4);

    memset(tmpbuf, 0xff, PAGE_SIZE);
    dd_read((AMT[lbn][1] * PAGES_PER_BLOCK) + offset, tmpbuf);

    memcpy(&lsn_check, tmpbuf + SECTOR_SIZE + 4, 4);
    if ((lsn_check >= 0) && (lsn_check <= 59))
    {
        dd_write((freeblockNum * PAGES_PER_BLOCK) + offset, pagebuf);
        for (int i = 0; i < PAGES_PER_BLOCK; i++)
        {
            if (offset != i)
            {
                dd_read((AMT[lbn][1] * PAGES_PER_BLOCK) + i, pagebuf);
                dd_write((freeblockNum * PAGES_PER_BLOCK) + i, pagebuf);
            }
        }
        dd_erase(AMT[lbn][1]);
        ref = AMT[lbn][1];
        AMT[lbn][1] = freeblockNum;
        freeblockNum = ref;
        AMT[BLOCKS_PER_DEVICE - 1][1] = freeblockNum;
    }
    else
    {
        dd_write((AMT[lbn][1] * PAGES_PER_BLOCK) + offset, pagebuf);
    }
    return;
}

void ftl_print()
{
    printf("lbn pbn\n");
    for (int j = 0; j < DATABLKS_PER_DEVICE; j++)
    {
        printf("%d  %d\n", AMT[j][0], AMT[j][1]);
    }
    printf("free block's pbn = %d\n", AMT[BLOCKS_PER_DEVICE - 1][1]);
    return;
}*/