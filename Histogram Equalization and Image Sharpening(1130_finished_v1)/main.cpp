#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "imgproc.h"
using namespace std;
int main()
{
	int bmpWidth;
	int bmpHeight;
	uint8_t*** image;
	int i, j, k;
	char readPath[] = "C:/cpp_files/  .bmp";
	set_path(readPath);
	readBmp(readPath, image, bmpWidth, bmpHeight);
	menu(image, bmpWidth, bmpHeight);
	char writePath[] = "C:/cpp_files/output.bmp";
	saveBmp(writePath, image, bmpWidth, bmpHeight);
	delete_image(image, bmpWidth, bmpHeight);
	return 0;
}