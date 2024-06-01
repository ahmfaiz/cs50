#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE avgcolour;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avgcolour =
                round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avgcolour;
            image[i][j].rgbtGreen = avgcolour;
            image[i][j].rgbtRed = avgcolour;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int mid = width / 2;
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < mid; j++)
        {
            temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Temp memory to store modified image so as to not affect original image while processing
    RGBTRIPLE filtered[height][width];

    float ravg, gavg, bavg;
    int pixels;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // For each pixel in image
            ravg = gavg = bavg = 0;
            pixels = 0;

            for (int near_i = -1; near_i <= 1; near_i++)
            {
                // Skip if on border
                if (i + near_i < 0 || i + near_i >= height)
                    continue;

                for (int near_j = -1; near_j <= 1; near_j++)
                {
                    // Skip if on border
                    if (j + near_j < 0 || j + near_j >= width)
                        continue;

                    ravg += image[i + near_i][j + near_j].rgbtRed;
                    gavg += image[i + near_i][j + near_j].rgbtGreen;
                    bavg += image[i + near_i][j + near_j].rgbtBlue;
                    pixels++;
                }
            }

            // Rounding off to nearest integer
            ravg = round(ravg / pixels);
            gavg = round(gavg / pixels);
            bavg = round(bavg / pixels);

            filtered[i][j].rgbtRed = ravg;
            filtered[i][j].rgbtGreen = gavg;
            filtered[i][j].rgbtBlue = bavg;
        }
    }

    // Copy the filtered image back to the original image
    memcpy(image, filtered, height * width * sizeof(RGBTRIPLE));
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Temp memory to store modified image so as to not affect original image while processing
    RGBTRIPLE filtered[height][width];

    float ravgx, gavgx, bavgx, ravgy, gavgy, bavgy;
    int r, g, b;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // For each pixel in image
            ravgx = gavgx = bavgx = 0;
            ravgy = gavgy = bavgy = 0;
            for (int near_i = -1; near_i <= 1; near_i++)
            {
                for (int near_j = -1; near_j <= 1; near_j++)
                {
                    // If pixel lies on border, treat as 0x000000
                    if ((j + near_j < 0) || (j + near_j >= width) || (i + near_i < 0) ||
                        (i + near_i >= height))
                    {
                        continue;
                    }
                    // If pixel not on border
                    ravgx += (image[i + near_i][j + near_j].rgbtRed * gx[near_i + 1][near_j + 1]);
                    gavgx += (image[i + near_i][j + near_j].rgbtGreen * gx[near_i + 1][near_j + 1]);
                    bavgx += (image[i + near_i][j + near_j].rgbtBlue * gx[near_i + 1][near_j + 1]);
                    ravgy += (image[i + near_i][j + near_j].rgbtRed * gy[near_i + 1][near_j + 1]);
                    gavgy += (image[i + near_i][j + near_j].rgbtGreen * gy[near_i + 1][near_j + 1]);
                    bavgy += (image[i + near_i][j + near_j].rgbtBlue * gy[near_i + 1][near_j + 1]);
                }
            }

            // Calculating final value of pixel
            r = round(sqrt((ravgx * ravgx) + (ravgy * ravgy)));
            g = round(sqrt((gavgx * gavgx) + (gavgy * gavgy)));
            b = round(sqrt((bavgx * bavgx) + (bavgy * bavgy)));

            // Keeping values less than 255
            filtered[i][j].rgbtRed = r > 255 ? 255 : r;
            filtered[i][j].rgbtGreen = g > 255 ? 255 : g;
            filtered[i][j].rgbtBlue = b > 255 ? 255 : b;
        }
    }

    // Copy the filtered image back to the original image
    memcpy(image, filtered, height * width * sizeof(RGBTRIPLE));
    return;
}

