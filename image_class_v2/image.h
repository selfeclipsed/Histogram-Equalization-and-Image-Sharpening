#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;
class img
{
private:
	int width;
	int height;
	uint8_t*** image;
	
public:
	bool readBmp(char* bmpName);
	bool saveBmp(char* bmpName);
	void grey_image1();
	void grey_image2();
	void grey_pixel_chart();
	void grey_equalize();
	void grey_sharpening();
	void grey_smooth();
	void RGB_pixel_chart();
	void RGB_equalize();
	void RGB_sharpening();
	void RGB_smooth();
	void delete_image();
	void setpath(char readPath[]);
	void menu(img& image);
};