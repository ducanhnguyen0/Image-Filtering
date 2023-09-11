#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtGreen = image[i][j].rgbtBlue;
            image[i][j].rgbtRed = image[i][j].rgbtGreen;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        int l = 0; // set pointer from the left
        int r = width - 1; // set pointer from the right
        while (l < r)
        {
            // swap pixel with temporary variable
            RGBTRIPLE temp = image[i][l];
            image[i][l] = image[i][r];
            image[i][r] = temp;
            l++; // move left pointer up
            r--; // move right pointer down
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image pixel-by-pixel
    RGBTRIPLE copy[height][width];
    for (int q = 0; q < height; q++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[q][w] = image[q][w];
        }
    }
    // Loop through each pixel in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int totalBlue = 0;
            int totalGreen = 0;
            int totalRed = 0;
            int count = 0;
            // Loop thorugh all neighbor pixel around within 1 and set boundaries by fmin, fmax function
            for (int n = fmax(i - 1, 0); n <= fmin(i + 1, height - 1); n++)
            {
                for (int m = fmax(j - 1, 0); m <= fmin(j + 1, width - 1); m++)
                {
                    // Get color from copy to not affect neighbor pixel color in image when they are not chnanged yet
                    totalBlue += copy[n][m].rgbtBlue;
                    totalGreen += copy[n][m].rgbtGreen;
                    totalRed += copy[n][m].rgbtRed;
                    count++;
                }
            }
            image[i][j].rgbtBlue = round(totalBlue / (float) count);
            image[i][j].rgbtGreen = round(totalGreen / (float) count);
            image[i][j].rgbtRed = round(totalRed / (float) count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create 2-D array to store changed pixel coresspond to image
    RGBTRIPLE copy[height][width];

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    // Loop through each pixel in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Create Gx and Gy value variable for each color channel
            long Gx_blue = 0;
            long Gx_green = 0;
            long Gx_red = 0;
            long Gy_blue = 0;
            long Gy_green = 0;
            long Gy_red = 0;
            // Loop through the box around the pixel
            for (int n = -1; n < 2; n++)
            {
                for (int m = -1; m < 2; m++)
                {
                    // Check the boudaries
                    if (i + n < 0 || i + n > height - 1 || j + m < 0 || j + m > width - 1)
                    {
                        // Add zero to value if out of bound
                        Gx_blue += 0;
                        Gx_green += 0;
                        Gx_red += 0;
                        Gy_blue += 0;
                        Gy_green += 0;
                        Gy_red += 0;
                    }
                    else
                    {
                        // Add correspond value with Gx, Gy plus pixel color channel
                        Gx_blue += (image[i + n][j + m].rgbtBlue * Gx[n + 1][m + 1]);
                        Gx_green += (image[i + n][j + m].rgbtGreen * Gx[n + 1][m + 1]);
                        Gx_red += (image[i + n][j + m].rgbtRed * Gx[n + 1][m + 1]);
                        Gy_blue += (image[i + n][j + m].rgbtBlue * Gy[n + 1][m + 1]);
                        Gy_green += (image[i + n][j + m].rgbtGreen * Gy[n + 1][m + 1]);
                        Gy_red += (image[i + n][j + m].rgbtRed * Gy[n + 1][m + 1]);
                    }
                }
            }
            // Check if the value is over 255 or not and add to copy
            copy[i][j].rgbtBlue = fmin(round(sqrt(pow(Gx_blue, 2) + pow(Gy_blue, 2))), 255);
            copy[i][j].rgbtGreen = fmin(round(sqrt(pow(Gx_green, 2) + pow(Gy_green, 2))), 255);
            copy[i][j].rgbtRed = fmin(round(sqrt(pow(Gx_red, 2) + pow(Gy_red, 2))), 255);
        }
    }
    // Apply pixel from copy to image
    for (int q = 0; q < height; q++)
    {
        for (int w = 0; w < width; w++)
        {
            image[q][w].rgbtBlue = copy[q][w].rgbtBlue;
            image[q][w].rgbtGreen = copy[q][w].rgbtGreen;
            image[q][w].rgbtRed = copy[q][w].rgbtRed;
        }
    }
    return;
}
