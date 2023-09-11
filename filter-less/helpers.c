#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int temp = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = temp;
            image[i][j].rgbtGreen = temp;
            image[i][j].rgbtRed = temp;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepiaRed = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue);
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            int sepiaGreen = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue);
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            int sepiaBlue = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue);
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
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
