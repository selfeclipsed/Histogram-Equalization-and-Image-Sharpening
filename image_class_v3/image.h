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
	img(char readPath[])
	{
		int width;
		int height;
		uint8_t*** image;
		char a, b;
		cout << "please enter the number of the picture(from 01 to 12): ";
		cin >> a >> b;
		for (int i = 0; readPath[i] != '\0'; i++)
		{
			if (readPath[i] == ' ')
			{
				readPath[i] = a;
				readPath[i + 1] = b;
				return;
			}
		}
	};
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
	void operations(img& image);
	~img()
	{
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				delete image[i][j];
			}
			delete image[i];
		}
		delete image;
		cout << "processing finished!";
	}
};