
#include <stdio.h>
#include <cs50.h>

int get_height(void);
void make_pyramid(int height);

int main(void)

{
    make_pyramid(get_height());
}

int get_height(void)

{

    int height;

    do
    {

        height = get_int("Height: ");


    }
    while (height < 1 || height > 8);

    return height;

}

void make_pyramid(int height)

{

    for (int i = 1; i <= height; i++)
    {

        for (int j = 0; j <= i * 2; j++ )
        {

            if (j == 0)
            {
              for (int z = 0; z < height - i; z++)

              {
                  printf(" ");
              }


            }

            if (j == (i*2)/2)

            {
                printf(" ");
                printf(" ");

            }

            else

            {
                printf("#");
            }
        }

        printf("\n");

    }


}