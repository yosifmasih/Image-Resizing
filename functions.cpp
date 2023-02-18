
// compile command:  g++ -std=c++17 -Wall -Wextra -pedantic -Weffc++ functions.cpp seamcarving.cpp

/*
IMPORTANT NOTES:
- width = # of columns
- height = # of rows
- pixels begin at the upper left corner of the image and traverse down and to the right
- index with [col][row] instead of [row][col]
- Pixel struct holds integer values for red, green, and blue
- we want to remove lower energy pixels
- to calculate the energy of a pixel, we use the dual gradient energy function
- dual gradient energy function: sqrt((deltax)^2(x, y) + (deltay)^2(x, y))
- (deltax)^2 is calculated by finding the difference in the RGB values from the pixels 
  right before and right after the pixel of choice and squaring them individually and then adding them
- (deltay)^2 is found similarly but its the RGB values of the pixel right above and below
  the pixel of choice, squaring each difference and adding them
- to access values inside of Pixel struct, we have to use the built in function: <structurename>.<valuename>() ---> for this problem we use i.e image.red() 

GAMEPLAN:
- we need to go through every pixel, achieving this by starting at the top left and looping through a 2d array in a [col][row] fashion, at pixels not at the border,
  we need to access the RGB values of that pixel through the Pixel struct. Using the RGB values and their difference to neighboring pixels, we can calculate the energy 
  of a pixel and identify which of those are lower and higher, removing the lower energy pixels to reshape the image. The same is done for border pixels, however we have
  to find the difference by using the other side of the image as the neighboring pixel if the end of the image is reached.
*/

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

Pixel** createImage(int width, int height) {
  cout << "Start createImage... " << endl;
  
  // Create a one dimensional array on the heap of pointers to Pixels 
  //    that has width elements (i.e. the number of columns)
  Pixel** image = new Pixel*[width];
  
  bool fail = false;
  
  for (int i=0; i < width; ++i) { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];
    
    if (image[i] == nullptr) { // failed to allocate
      fail = true;
    }
  }
  
  if (fail) { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    for (int i=0; i < width; ++i) {
      delete [] image[i]; // deleting nullptr is not a problem
    }
    delete [] image; // delete array of pointers
    return nullptr;
  }
  
  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = { 0, 0, 0 };
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel** image, int width) {
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i=0; i<width; ++i) {
    delete [] image[i]; // delete each individual array placed on the heap
  }
  delete [] image;
  image = nullptr;
}

// implement for part 1

int* createSeam(int length) {
  //create a seam
  int* seam = new int[length];

  for (int i = 0; i < length; i++) {
    seam[i] = 0;
  }

  return seam;
}

void deleteSeam(int* seam) {
  //delete the seam
  delete[] seam;
}


bool loadImage(string filename, Pixel** image, int width, int height) {

/*
- this function will open a file and load the pixels onto a 2d array
- if loading was successful, return true, otherwise false
- throw errors if:
  1. too many color values (accounted for)
  2. color values < 0 or > 255 (accounted for)
  3. failed to open file (accounted for)
  4. file type isnt P3 (accounted for)
  5. contains non integer value (accounted for)
  6. not enough color values (accounted for)
  7. input value doesnt match value in file ()
*/

  //initialize essential variables
  int i;
  int j;
  
  ifstream fin;
  int x = 0; //arbitrary constant to compare the type of the color with

  //open the file
  ifstream file(filename);
  //file.open(filename, ios::in);
  fin.open(filename);

  //check if the file opens, if it doesnt, throw an error
   if (!file.is_open()) {
    cout << "Error: failed to open input file - " << filename << endl;
    return false;
  }
  //check if the file is of type P3
  char type[2];
  fin >> type;
  if((toupper(type[0]) != 'P') || (type[1] != '3')){
    cout << "Error: type is " <<  type << " instead of P3" << endl;
    return false;
  }

//test if the width and height match the values in the file
  int w, h;
  fin >> w;
  if(w != width){
    cout << "Error: input width (" << width << ") does not match value in file ("  << w << ")" << endl;
    return false;
  }
  fin >> h;
  if(h != height){
    cout << "Error: input height (" << height << ") does not match value in file ("  << h << ")" << endl;
    return false;
  }

  //check if the size is a valid number
  int size;
  fin >> size;
  if (file.fail()) {
    cout << "Error: read non-integer value" << endl;
    return false;
  }
  if (size != 255) {
    cout << "Error invalid color value" << endl;
    return false;
  }

  //loop through all pixels
  if(file.is_open()){
    for(i=0;i<width;i++){
      for(j=0;j<height;j++){

        Pixel pixel_color;

        //load pixels here
        fin >> pixel_color.r >> pixel_color.g >> pixel_color.b;

        //validate pixels
        if(file.fail()){
          cout << "Error: not enough color values" << endl;
          return false;
        }

        //check if all the colors are of type <int>
        if(typeid(pixel_color.r).name() != typeid(x).name()){
          cout << "Error: read non-integer value" << endl;
          return false;
        }
        else if(typeid(pixel_color.g).name() != typeid(x).name()){
          cout << "Error: read non-integer value" << endl;
          return false;
        }
        else if(typeid(pixel_color.b).name() != typeid(x).name()){
          cout << "Error: read non-integer value" << endl;
          return false;
        }

        //check if all colors are within 0-255 range
        if(pixel_color.r > 255 || pixel_color.r < 0){
          cout << "Error: invalid color value " << pixel_color.r << endl;
          return false;
        }
        else if(pixel_color.g > 255 || pixel_color.g < 0){
          cout << "Error: invalid color value " << pixel_color.g << endl;
          return false;
        }
        else if(pixel_color.b > 255 || pixel_color.b < 0){
          cout << "Error: invalid color value " << pixel_color.b << endl;
          return false;
        }

        image[i][j].r = pixel_color.r;
        image[i][j].g = pixel_color.g;
        image[i][j].b = pixel_color.b;

      if(file.eof()){
        cout << "Error: not enough color values" << endl;
      return false;  
    }
    }
    //if the end of the file is hit before the entire loop is finished, this means that there are not enough colors
    if(file.eof()){
      cout << "Error: not enough color values" << endl;
      return false;  
    }
    }

    
  }

  //throw error if the file still doesnt end after the loop terminates
  if(!file.eof()){
    string temp = "yosif";
    fin >> temp;
    if(temp != "yosif"){
      cout << "Error: too many color values" << endl; 
      //cout << "Error: too many color values" << endl;
      return false;
    }
  }

  //throw error if file opening failed
  else{
    cout << "Error: failed to open input file - " << filename << endl;
    return false;
  }

  //check for whitespaces
  if (!(file >> std::ws).eof() ) {
    cout <<"Error: not enough color values" << endl;
    //cout << "Error: too many color values" << endl;
    return false;
  }

  return true;
}

bool outputImage(string filename, const Pixel*const* image, int width, int height) {

  //declare variables
  ofstream file;
  int i, j;

  //open the file in write mode
  file.open(filename);

  //loop through all pixels and write them to a file
  if(file.is_open()){
    for(i=0; i<width; i++){
      for(j=0; j<height; j++){
        file << image[i][j].r  << " " << image[i][j].g  << " " << image[i][j].b << " ";
    }
  }
  }
  else{
    cout << "Error: failed to open output file - " << filename << endl;
    return false;
  }
  
  return true;
}

int energy(const Pixel*const* image, int x, int y, int width, int height) { 
  //this function calculates the energy of a pixel
  //remember: dual gradient energy function = sqrt((deltax^2(x,y)) + (deltay^2(x,y)))
  //image[col][row] == image[width][height] == image[x][y]  ****width is going left to right, height is going top to bottom****

  //declare variables
  int energy, energyRedx, energyRedy, energyGreenx, energyGreeny, energyBluex, energyBluey;
  
  //hidden test case for 2x2 array
  if (width < 3) {
    //energy will be zero
    energyRedx = 0;
    energyGreenx = 0;
    energyBluex = 0;
  } 
  //for left side
  else if (x == 0) {
    energyRedx = pow((image[width - 1][y].r - image[x+1][y].r), 2);
    energyGreenx = pow((image[width - 1][y].g - image[x+1][y].g), 2);
    energyBluex = pow((image[width - 1][y].b - image[x+1][y].b), 2);
  } 
  //for right side
  else if (x == width - 1) {
    energyRedx = pow((image[x-1][y].r - image[0][y].r), 2);
    energyGreenx = pow((image[x-1][y].g - image[0][y].g), 2);
    energyBluex = pow((image[x-1][y].b - image[0][y].b), 2);
  } 
  //for all non edge cases
  else {
    energyRedx = pow((image[x+1][y].r - image[x-1][y].r), 2);
    energyGreenx = pow((image[x+1][y].g - image[x-1][y].g), 2);
    energyBluex = pow((image[x+1][y].b - image[x-1][y].b), 2);
  }

  if (height < 3) {
    //energy will be zero
    energyRedy = 0;
    energyGreeny = 0;
    energyBluey = 0;
  } 
  //for bottom side
  else if (y == 0) {
    energyRedy = pow((image[x][height - 1].r - image[x][y+1].r), 2);
    energyGreeny = pow((image[x][height - 1].g - image[x][y+1].g), 2);
    energyBluey = pow((image[x][height - 1].b - image[x][y+1].b), 2);
  } 
  //for top side
  else if (y == height - 1) {
    energyRedy = pow((image[x][0].r - image[x][y-1].r), 2);
    energyGreeny = pow((image[x][0].g - image[x][y-1].g), 2);
    energyBluey = pow((image[x][0].b - image[x][y-1].b), 2);
  } 
  //for all non edge cases
  else {
    energyRedy = pow((image[x][y+1].r - image[x][y-1].r), 2);
    energyGreeny = pow((image[x][y+1].g - image[x][y-1].g), 2);
    energyBluey = pow((image[x][y+1].b - image[x][y-1].b), 2); 
  }

  
  energy = energyRedx + energyRedy + energyGreenx + energyGreeny + energyBluex + energyBluey;

  return energy;
}

// implement for part 2

// uncomment for part 2
 
//complete this function first
int loadVerticalSeam(const Pixel*const* image, int start_col, int width, int height, int* seam) {

  //image[col][row]
  //seam if a dynamic array

  //declare variables
  int i;
  int totalEnergy = energy(image, start_col, 0, width, height);
  seam[0] = start_col;

  /*
  - traverse through image starting at first row of start_col
  - traverse down the column by choosing the path of lowest energy
  
  PSEUDOCODE:
  1. set seam for the first row to the starting column
  */

  for(i=1;i<height;i++){
    //check if the width == 1
    if(width == 1){
      totalEnergy += energy(image, start_col, i, width, height);
      seam[i] = start_col;
    }
    //for left side column
    if(start_col == 0){
      //check same column
      if(energy(image, start_col, i, width, height) < energy(image, start_col+1, i, width, height)){
        totalEnergy += energy(image, start_col, i, width, height);
        seam[i] = start_col;
      }
      //check column just right of start col
      else if(energy(image, start_col+1, i, width, height) < energy(image, start_col, i, width, height)){
        totalEnergy += energy(image, start_col+1, i, width, height);
        start_col += 1;
        seam[i] = start_col;
      }
      //check if both sides are tied
      else if(energy(image, start_col, i, width, height) == energy(image, start_col+1, i, width, height)){
        totalEnergy += energy(image, start_col, i , width, height);
        seam[i] = start_col;
      }
    }
    //for right side column
    else if(start_col == width-1){
      //check same column
      if(energy(image, start_col, i, width, height) < energy(image, start_col-1, i, width, height)){
        totalEnergy += energy(image, start_col, i, width, height);
        seam[i] = start_col;
      }
      //check column just left of start col
      else if(energy(image, start_col-1, i, width, height) < energy(image, start_col, i, width, height)){
        totalEnergy += energy(image, start_col-1 ,i, width, height);
        start_col -= 1;
        seam[i] = start_col;
      }
      else if(energy(image, start_col, i , width, height) == energy(image, start_col-1, i, width, height)){
        totalEnergy += energy(image, start_col, i , width, height);
        seam[i] = start_col;
      }
    }
    //for middle columns
    else{
      //check if middle pixel has the lowest energy
      if(energy(image, start_col, i, width, height) < energy(image, start_col+1, i, width, height) && energy(image, start_col, i, width, height) < energy(image, start_col-1, i, width, height)){
        totalEnergy += energy(image, start_col, i, width, height);
        seam[i] = start_col;
        //start_col doesnt change
      }
      //check if right pixel has the lowest energy
      else if(energy(image, start_col+1, i, width, height) < energy(image, start_col, i, width, height) && energy(image, start_col+1, i, width, height) < energy(image, start_col-1, i, width, height)){
        totalEnergy += energy(image, start_col+1, i, width, height);
        start_col += 1;
        seam[i] = start_col;
      }
      //check if the left pixel has the lowest energy
      else if(energy(image, start_col-1, i, width, height) < energy(image, start_col, i, width, height) && energy(image, start_col-1, i, width, height) < energy(image, start_col+1, i, width, height)){
          totalEnergy += energy(image, start_col-1, i, width, height);
          start_col -= 1;
          seam[i] = start_col;
      }
      //check if start col ties with right col
      else if(energy(image, start_col, i, width, height) < energy(image, start_col-1, i, width, height) && energy(image, start_col, i, width, height) == energy(image, start_col+1, i, width, height)){
        totalEnergy += energy(image, start_col, i, width, height);
        seam[i] = start_col;
      }
      //check if start_col ties with left col
      else if(energy(image, start_col, i, width, height) < energy(image, start_col+1, i, width, height) && energy(image, start_col, i, width, height) == energy(image, start_col-1, i, width, height)){
        totalEnergy += energy(image, start_col, i, width, height);
        seam[i] = start_col;
      }
      //check if all three cols tie, it goes middle
      else if(energy(image, start_col, i, width, height) == energy(image, start_col-1, i, width, height)  && energy(image, start_col-1, i, width, height) == energy(image, start_col+1, i, width, height)){
        totalEnergy += energy(image, start_col, i, width, height);
        seam[i] = start_col;
      }
      //check if left col ties with right col
      else if(energy(image, start_col-1, i, width, height) < energy(image, start_col, i, width, height) && energy(image, start_col-1, i, width, height) == energy(image, start_col+1, i , width, height)){
        totalEnergy += energy(image, start_col+1, i, width, height);
        start_col += 1;
        seam[i] = start_col;
      }
    }
  }

  return totalEnergy;
}

/*//optional
int loadHorizontalSeam(const Pixel*const* image, int start_row, int width, int height, int* seam) {
  return 0;
}*/

//complete this function second
int* findMinVerticalSeam(const Pixel*const* image, int width, int height) {

  int i;
  int min = INT32_MAX;
  int* seam = new int[height];
  int* temp = new int[height];

  for(i=0;i<width;i++){
    if(loadVerticalSeam(image, i, width, height, seam) < min){
      min = loadVerticalSeam(image, i, width, height, seam);
      temp = seam;
    }
  } 

  //deallocate seam
  delete[] seam;

  return temp;
}

/*//optional
int* findMinHorizontalSeam(const Pixel*const* image, int width, int height) {
  return nullptr;
}*/

//complete this function third
void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) {
  //declare variables
  int i, j;
  
  //loop through rows
  for(i=0;i<height;i++){
  
      for(j=verticalSeam[i];j<width-1;j++){
      image[j][i].r = image[j+1][i].r;
      image[j][i].g = image[j+1][i].g;
      image[j][i].b = image[j+1][i].b;
    
    }
    

  }
  
}

/*//optional
void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam) {
}*/

