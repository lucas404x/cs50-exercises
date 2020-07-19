#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <cs50.h>


void card_name(long card_number);
bool checksum(long card_number);
int ascii_to_int(char c);

int main(void)

{

    long card_number = get_long("NUMBER: ");

    if (checksum(card_number))

    {
        card_name(card_number);
    }

    else

    {
        printf("INVALID\n");
    }
}




void card_name(long card_number)

{
    char number[255], digits[3];

    int lenght = sprintf(number, "%li", card_number);

    // join two digits

    digits[0] = number[0];
    digits[1] = number[1];
    digits[2] = '\0';

    if (lenght == 15 && (strncmp(digits, "34", 2) == 0 || strncmp(digits, "37", 2) == 0))

    {
        printf("AMEX\n");
    }

    else if (lenght == 16 && (strtol(digits, NULL, 0) >= 51 && strtol(digits, NULL, 0) <= 55))

    {
        printf("MASTERCARD\n");
    }

    else if ((lenght >= 13 && lenght <= 16) && strncmp(digits, "4", 1) == 0)

    {
        printf("VISA\n");
    }

    else

    {
        printf("INVALID\n");
    }
}


bool checksum(long card_number)

{
    int first_sum = 0, second_sum = 0;
    char number[255], n[5];

    int lenght = sprintf(number, "%li", card_number);

    for (int i = lenght - 2; i >= 0;)

    {

        sprintf(n, "%i", ascii_to_int(number[i])*2);
        if (strlen(n) > 1)

            {
                for (int j = 0; j < strlen(n); j++)

            {
                first_sum += ascii_to_int(n[j]);

            }

            }

            else
            {
                first_sum += ascii_to_int(number[i]) * 2;
            }

        second_sum +=  ascii_to_int(number[i + 1]);


        if (i == 1)
        {
            i--;
        }

        else

        {
            i -= 2;
        }

    }

    return (first_sum + second_sum) % 10 == 0;
}

int ascii_to_int(char c)

 {
     return c - 48;

}