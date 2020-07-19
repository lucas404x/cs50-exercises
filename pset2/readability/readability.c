#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <cs50.h>


float coleman_liau_index(float L, float S);
float S(int setences, int words);
float L(int letters, int words);
int count_sentences(string text);
int count_letters(string text);
int count_words(string text);


int main()

{
    string text = get_string("Text: ");

    // text analyze

    int sentences = count_sentences(text);
    int letters = count_letters(text);
    int words = count_words(text);

    // calc index

    int index = round(coleman_liau_index(L(letters, words), S(sentences, words)));

    if (index < 1)

    {
        printf("Before Grade 1\n");

    }

    else if (index >= 16)

    {
        printf("Grade 16+\n");
    }

    else

    {
        printf("Grade %d\n", index);
    }

    return 0;
}

float coleman_liau_index(float L, float S)

{
    return 0.0588 * L - 0.296 * S - 15.8;
}

int count_sentences(string text)

{
    bool is_sentence = false;
    int sentences = 0;
    bool expression;

    for (int i = 0, length = strlen(text); i < length; i++)

    {
        expression = (text[i] == '!' || text[i] == '.' || text[i] == '?');

        if (!is_sentence && expression)

        {
            is_sentence = true;
            sentences++;
        }

        else if (is_sentence && !expression)

        {
            is_sentence = false;
        }

        else if (i == length - 1)

        {
             sentences++;
         }
    }

    return sentences;
}


int count_letters(string text)

{
    int letters = 0;
    int character;

    for (int i = 0, length = strlen(text); i < length; i++)

    {
        character = (int) tolower(text[i]);
        if (character >= 97 && character <= 122)

        {
            letters++;
        }
    }

    return letters;
}

int count_words(string text)

{
    bool is_space = false;
    int words = 0;

    for (int i = 0, length = strlen(text); i < length; i++)

    {
        if (!is_space && isspace(text[i]))

        {
            is_space = true;
            words++;
        }

        else if (!is_space && i == length - 1)

        {
            words++;
        }

        else if (is_space && isalpha(text[i]))

        {
            is_space = false;
        }



    }

    return words;
}

float L(int letters, int words)

{
    return ((float) letters / (float) words) * 100;

}

float S(int sentences, int words)

{
    return ((float) sentences / (float) words) * 100;

}
