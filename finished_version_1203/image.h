#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#include "assert.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;
class mode_exception
{
public:
	mode_exception()
		: message("wrong operation:the operation does not exist,please enter number 0~7.") { }
	const char* what() const { return message; }
private:
	const char* message;
};
class input_exception
{
public:
	input_exception()
		: message("wrong input:the picture does not exist,please enter number 01~12.") { }
	const char* what() const { return message; }
private:
	const char* message;
};
class img
{
private:
	int width;
	int height;
	uint8_t*** image;
	bool error = false;
public:
	img(char readPath[])
	{
		int width;
		int height;
		uint8_t*** image;
		char a, b;
		cout << "please enter the number of the picture(from 01 to 12): ";
		cin >> a >> b;
		try
		{
			if (a != '0' && a != '1')
			{
				throw input_exception();
				error = true;
			}
			if(b < '0' || b > '9')
			{
				throw input_exception();
				error = true;
			}
			if (a == '1' && (b > '2' || b < '0'))
			{
				throw input_exception();
				error = true;
			}
			if (a == '0' && (b > '9' || b <= '0'))
			{
				throw input_exception();
				error = true;
			}
		}
		catch (input_exception ex)
		{
			cout << "Exception occurred: " << ex.what() << '\n';
			cout << endl;
			cout << "program terminated.";
			assert(error);
		}
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