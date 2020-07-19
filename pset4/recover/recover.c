#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *f, *outf = NULL;
    char filename[8];
    int index = 0;

    unsigned char buffer[512];

    f = fopen(argv[1], "r");

    if (f == NULL)
    {
        return 1;
    }


    while (fread(&buffer, 512, 1, f) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (index > 0)
            {
                fclose(outf);
            }


            sprintf(filename, "%03i.jpg", index);
            printf("%i\n", index);
            outf = fopen(filename, "w");
            index++;
        }


        if (index > 0)
        {
            fwrite(&buffer, 512, 1, outf);
        }

    }

    fclose(outf);
    fclose(f);

    return 0;

}
