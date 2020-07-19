#include "helpers.h"
#include <math.h>


void swap(RGBTRIPLE *left_side, RGBTRIPLE *right_side);
int verify_num(int n, int limit);

void sum_color_channels(
RGBTRIPLE *pixel, int i, int j,
int *sumRedX, int *sumRedY,
int *sumGreenX, int *sumGreenY,
int *sumBlueX, int *sumBlueY
);


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int average;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average = round(((float) image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen) / 3);
            image[i][j].rgbtRed = average;
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < round((float)(width / 2)); j++)
        {
            swap(&image[i][j], &image[i][width - (j + 1)]);
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int sumRed, sumGreen, sumBlue, count;
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            sumRed = copy[i][j].rgbtRed;
            sumGreen = copy[i][j].rgbtGreen;
            sumBlue = copy[i][j].rgbtBlue;
            count = 1;

            if (j - 1 >= 0)
            {
                sumRed += copy[i][j - 1].rgbtRed;
                sumGreen += copy[i][j - 1].rgbtGreen;
                sumBlue += copy[i][j - 1].rgbtBlue;
                count++;
            }

            if (j + 1 < width)
            {
                sumRed += copy[i][j + 1].rgbtRed;
                sumGreen += copy[i][j + 1].rgbtGreen;
                sumBlue += copy[i][j + 1].rgbtBlue;
                count++;
            }

            if (i - 1 >= 0)
            {
                sumRed += copy[i - 1][j].rgbtRed;
                sumGreen += copy[i - 1][j].rgbtGreen;
                sumBlue += copy[i - 1][j].rgbtBlue;
                count++;
            }

            if (i + 1 < height)
            {
                sumRed += copy[i + 1][j].rgbtRed;
                sumGreen += copy[i + 1][j].rgbtGreen;
                sumBlue += copy[i + 1][j].rgbtBlue;
                count++;
            }

            if (i + 1 < height && j + 1 < width)
            {
                sumRed += copy[i + 1][j + 1].rgbtRed;
                sumGreen += copy[i + 1][j + 1].rgbtGreen;
                sumBlue += copy[i + 1][j + 1].rgbtBlue;
                count++;
            }

            if (i + 1 < height && j - 1 >= 0)
            {
                sumRed += copy[i + 1][j - 1].rgbtRed;
                sumGreen += copy[i + 1][j - 1].rgbtGreen;
                sumBlue += copy[i + 1][j - 1].rgbtBlue;
                count++;
            }

            if (i - 1 >= 0 && j + 1 < width)
            {
                sumRed += copy[i - 1][j + 1].rgbtRed;
                sumGreen += copy[i - 1][j + 1].rgbtGreen;
                sumBlue += copy[i - 1][j + 1].rgbtBlue;
                count++;
            }

            if (i - 1 >= 0 && j - 1 >= 0)
            {
                sumRed += copy[i - 1][j - 1].rgbtRed;
                sumGreen += copy[i - 1][j - 1].rgbtGreen;
                sumBlue += copy[i - 1][j - 1].rgbtBlue;
                count++;
            }

            image[i][j].rgbtRed = round(sumRed / (count * 1.0));
            image[i][j].rgbtGreen = round(sumGreen / (count * 1.0));
            image[i][j].rgbtBlue = round(sumBlue / (count * 1.0));
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    int sumRedX, sumGreenX, sumBlueX;
    int sumRedY, sumGreenY, sumBlueY;


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sumRedX = sumGreenX = sumBlueX = 0;
            sumRedY = sumGreenY = sumBlueY = 0;

            if (j - 1 >= 0)
            {
                sum_color_channels(&copy[i][j - 1], 1, 0, &sumRedX, &sumRedY, &sumGreenX, &sumGreenY, &sumBlueX, &sumBlueY);
            }

            if (j + 1 < width)
            {
                sum_color_channels(&copy[i][j + 1], 1, 2, &sumRedX, &sumRedY, &sumGreenX, &sumGreenY, &sumBlueX, &sumBlueY);
            }

            if (i - 1 >= 0)
            {
                sum_color_channels(&copy[i - 1][j], 0, 1, &sumRedX, &sumRedY, &sumGreenX, &sumGreenY, &sumBlueX, &sumBlueY);
            }

            if (i + 1 < height)
            {
                sum_color_channels(&copy[i + 1][j], 2, 1, &sumRedX, &sumRedY, &sumGreenX, &sumGreenY, &sumBlueX, &sumBlueY);
            }

            if (i - 1 >= 0 && j + 1 < width)
            {
                sum_color_channels(&copy[i - 1][j + 1], 0, 2, &sumRedX, &sumRedY, &sumGreenX, &sumGreenY, &sumBlueX, &sumBlueY);
            }

            if (i - 1 >= 0 && j - 1 >= 0)
            {
                sum_color_channels(&copy[i - 1][j - 1], 0, 0, &sumRedX, &sumRedY, &sumGreenX, &sumGreenY, &sumBlueX, &sumBlueY);
            }

            if (i + 1 < height && j + 1 < width)
            {
                sum_color_channels(&copy[i + 1][j + 1], 2, 2, &sumRedX, &sumRedY, &sumGreenX, &sumGreenY, &sumBlueX, &sumBlueY);
            }

            if (i + 1 < height && j - 1 >= 0)
            {
                sum_color_channels(&copy[i + 1][j - 1], 2, 0, &sumRedX, &sumRedY, &sumGreenX, &sumGreenY, &sumBlueX, &sumBlueY);
            }

            image[i][j].rgbtRed = (int) verify_num(round(sqrt((sumRedX * sumRedX) + (sumRedY * sumRedY))), 255);
            image[i][j].rgbtGreen = (int) verify_num(round(sqrt((sumGreenX * sumGreenX) + (sumGreenY * sumGreenY))), 255);
            image[i][j].rgbtBlue = (int) verify_num(round(sqrt((sumBlueX * sumBlueX) + (sumBlueY * sumBlueY))), 255);

        }
    }

}


void swap(RGBTRIPLE *left_side, RGBTRIPLE *right_side)

{
    RGBTRIPLE tmp = *left_side;
    *left_side = *right_side;
    *right_side = tmp;

}


void sum_color_channels(
RGBTRIPLE *pixel, int i, int j,
int *sumRedX, int *sumRedY,
int *sumGreenX, int *sumGreenY,
int *sumBlueX, int *sumBlueY
)

{
    int gx[3][3] = {

        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int gy[3][3] = {

        {-1, -2, -1},
        {0, 0, 0},
        {1, 2 ,1}
    };

    *sumRedX += (gx[i][j] * pixel->rgbtRed);
    *sumRedY += (gy[i][j] * pixel->rgbtRed);
    *sumGreenX += (gx[i][j] * pixel->rgbtGreen);
    *sumGreenY += (gy[i][j] * pixel->rgbtGreen);
    *sumBlueX += (gx[i][j] * pixel->rgbtBlue);
    *sumBlueY += (gy[i][j] * pixel->rgbtBlue);
}


int verify_num(int n, int limit)
{
    if (n > limit)
    {
        return n - (n - limit);
    }

    return n;
}