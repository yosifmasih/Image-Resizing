#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>

struct Pixel {
  int r; // red
  int g; // green
  int b; // blue
};

// Implemented for you

Pixel** createImage(int width, int height);
void deleteImage(Pixel** image, int width);

// Implement for part 1

int* createSeam(int length);
void deleteSeam(int* seam);
bool loadImage(std::string filename, Pixel** image, int width, int height);
bool outputImage(std::string filename, const Pixel*const* image, int width, int height);
int energy(const Pixel*const* image, int x, int y, int width, int height);

// Implement for part 2

// uncomment for part 2


int loadVerticalSeam(const Pixel*const* image, int start_col, int width, int height, int* seam);
int loadHorizontalSeam(const Pixel*const* image, int start_row, int width, int height, int* seam);
int* findMinVerticalSeam(const Pixel*const* image, int width, int height);
int* findMinHorizontalSeam(const Pixel*const* image, int width, int height);
void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam);
void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam);


#endif