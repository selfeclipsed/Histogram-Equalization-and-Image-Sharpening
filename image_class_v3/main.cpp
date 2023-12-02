#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "image.h"
using namespace std;
int main()
{
	char readPath[] = "C:/cpp_files/  .bmp";//two space is saved for the numbers users input.
	char writePath[] = "C:/cpp_files/class_img_output.bmp";
	img image(readPath);
	image.readBmp(readPath);
	image.operations(image);
	image.saveBmp(writePath);
	return 0;
}