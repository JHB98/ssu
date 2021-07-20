#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int recordN;
    char *recordF, buf[250] = {0};
    FILE *fp;
    recordN = atoi(argv[1]);
    recordF = argv[2];
    fp = fopen(recordF, "w");

    fwrite(&recordN, 4, 1, fp);

    for (int i = 0; i < recordN; i++)
    {
        scanf("%s", buf);
        fwrite(buf, 250, 1, fp);
    }

    return 0;
}