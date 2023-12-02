#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "image.h"
using namespace std;
int main()
{
	char readPath[] = "C:/cpp_files/  .bmp";
	char writePath[] = "C:/cpp_files/class_img_output.bmp";
	img image;
	image.setpath(readPath);
	image.readBmp(readPath);
	image.menu(image);
	image.saveBmp(writePath);
	image.delete_image();
	return 0;
}