#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <cs50.h>


string encrypt(string text, string key);
bool is_alphanum_string(string key);
bool is_repeated_string(string key);

int main(int argc, string argv[])

{
    if (argc != 2)

    {
        printf("Usage: ./substitution key\n");
        return 1;

    }

    else if (strlen(argv[1]) != 26)

    {
        printf("Key must contain 26 characters.\n");
        return 1;

    }

    else if (is_alphanum_string(argv[1]))

    {
        printf("Key must only contain alphabetic characters.\n");
        return 1;
    }

    else if (is_repeated_string(argv[1]))

    {
        printf("Key must not contain repeatead characters.\n");
        return 1;
    }

    string key = argv[1];
    string plaintext = get_string("plaintext: ");

    printf("ciphertext: %s\n", encrypt(plaintext, key));
    return 0;
}


string encrypt(string text, string key)

{
    int length = strlen(text);
    char character;

    for (int i = 0; i < length; i++)

    {

        if (isalpha(text[i]))

        {

            character = key[tolower(text[i]) - 97];

            if (isupper(text[i]))

            {
                text[i] = toupper(character);
            }

            else

            {
                text[i] = tolower(character);
            }
        }
    }

    return text;
}


bool is_alphanum_string(string key)

{
    bool is_alphanum = false;
    int length = strlen(key);

    for (int i = 0; i < length; i++)

    {
        if (isdigit(key[i]))

        {
            is_alphanum = true;
            i = length - 1;
        }
    }

    return is_alphanum;
}

bool is_repeated_string(string key)

{
    int length = strlen(key);
    bool is_repeated = false;

    for (int i = 0; i < length; i++)

    {
        for (int j = 0; j < length; j++)

            if (j == i)

            {
                continue;
            }

            else if (key[i] == key[j])

            {
                is_repeated = true;
                j = length - 1;
                i = length - 1;
            }

    }

    return is_repeated;
}