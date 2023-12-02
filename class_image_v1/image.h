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
	bool readBmp(char* bmpName)
	{
		FILE* fp = fopen(bmpName, "rb");
		if (fp == 0) return 0;
		fseek(fp, sizeof(BITMAPFILEHEADER), 0);
		BITMAPINFOHEADER head;
		fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);
		width = head.biWidth;
		height = head.biHeight;
		int biBitCount = head.biBitCount;
		int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;
		if (biBitCount != 24)
		{
			cout << "read error (24-digit)" << endl;
			return 0;
		}
		unsigned char* pBmpBuf = new unsigned char[lineByte * height];
		fread(pBmpBuf, 1, lineByte * height, fp);
		int bmpWidthBytes = width * 3;
		if (width % 4 > 0)
		{
			bmpWidthBytes = (bmpWidthBytes / 4 + 1) * 4;
		}
		image = new uint8_t * *[height];
		int i, j, k;
		for (i = 0; i < height; ++i)
		{
			image[i] = new uint8_t * [width];
			for (j = 0; j < width; ++j)
			{
				image[i][j] = new uint8_t[3];
				for (k = 0; k < 3; ++k)
				{
					image[i][j][k] = (uint8_t)pBmpBuf[(height - 1 - i) * bmpWidthBytes + j * 3 + k];
				}
			}
		}
		delete pBmpBuf;
		fclose(fp);
		return 1;
	}
	bool saveBmp(char* bmpName)
	{
		if (!image)
			return 0;
		int biBitCount = 24;
		int colorTablesize = 0;
		int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;
		FILE* fp = fopen(bmpName, "wb");
		if (fp == 0) return 0;
		BITMAPFILEHEADER fileHead;
		fileHead.bfType = 0x4D42;
		fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
			+ colorTablesize + lineByte * height;
		fileHead.bfReserved1 = 0;
		fileHead.bfReserved2 = 0;
		fileHead.bfOffBits = 54 + colorTablesize;
		fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);
		BITMAPINFOHEADER head;
		head.biBitCount = biBitCount;
		head.biClrImportant = 0;
		head.biClrUsed = 0;
		head.biCompression = 0;
		head.biHeight = height;
		head.biPlanes = 1;
		head.biSize = 40;
		head.biSizeImage = lineByte * height;
		head.biWidth = width;
		head.biXPelsPerMeter = 0;
		head.biYPelsPerMeter = 0;
		fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);
		unsigned char* imgBuf = new unsigned char[height * lineByte]();
		int i, j, k;
		for (i = 0; i < height; ++i)
		{
			for (j = 0; j < width; ++j)
			{
				for (k = 0; k < 3; ++k)
				{
					imgBuf[(height - 1 - i) * lineByte + j * 3 + k] = image[i][j][k];
				}
			}
		}
		fwrite(imgBuf, height * lineByte, 1, fp);
		delete imgBuf;
		fclose(fp);
		return 1;
	}
	void grey_image1()
	{
		int i, j, k;
		for (i = 0; i < height; ++i)
		{
			for (j = 0; j < width; ++j)
			{
				for (k = 0; k < 3; ++k)
				{
					image[i][j][k] = (image[i][j][0] * 0.299 + image[i][j][1] * 0.587 + image[i][j][2] * 0.114);
				}
			}
		}
	}
	void grey_image2()
	{
		int i, j, k;
		for (i = 0; i < height; ++i)
		{
			for (j = 0; j < width; ++j)
			{
				for (k = 0; k < 3; ++k)
				{
					image[i][j][k] = (image[i][j][0] + image[i][j][1] + image[i][j][2]) / 3;
				}
			}
		}
	}
	void grey_pixel_chart()
	{
		int pcnt[256] = { 0 };
		for (int p = 0; p <= 255; p++)
		{
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					if (p == image[i][j][0]) pcnt[p] += 1;
				}
			}
		}
		ofstream dataFile;
		dataFile.open("C:/cpp_files/bar_chart.txt", ofstream::app);
		fstream file("C:/cpp_files/bar_chart.txt", ios::out);
		for (int i = 0; i <= 255; i++)
		{
			if (i < 10) dataFile << '0' << '0' << i << '|';
			else if (i >= 10 && i < 100) dataFile << '0' << i << '|';
			else dataFile << i << '|';
			for (int j = 0; j <= pcnt[i]; j++)
			{
				dataFile << '.';
			}
			dataFile << endl;
		}
		dataFile.close();
	}
	void grey_equalize()
	{
		int pcnt[256] = { 0 };
		for (int p = 0; p <= 255; p++)
		{
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					if (p == image[i][j][0]) pcnt[p] += 1;
				}
			}
		}
		int psum = 0;
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				psum = 0;
				for (int p = 0; p <= image[i][j][0]; p++)
				{
					psum += pcnt[p];
				}
				image[i][j][0] = 256 * psum / height / width;
			}
		}
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				for (int k = 1; k < 3; k++)
				{
					image[i][j][k] = image[i][j][0];
				}
			}
		}
	}
	void grey_sharpening()
	{
		int sum = 0, sharp_para = 4;
		for (int i = 1; i < height - 1; i++)
		{
			for (int j = 1; j < width - 1; j++)
			{
				sum = abs(image[i][j][0] - image[i + 1][j + 1][0]) + abs(image[i + 1][j][0] - image[i][j + 1][0]);
				image[i][j][0] -= sum * sharp_para;
				image[i][j][1] -= sum * sharp_para;
				image[i][j][2] -= sum * sharp_para;
			}
		}
	}
	void grey_smooth()
	{
		uint8_t*** new_image = image;
		int conv_sum;
		for (int i = 1; i < height - 1; i++)
		{
			for (int j = 1; j < width - 1; j++)
			{
				conv_sum = (image[i][j][0] + image[i - 1][j - 1][0] + image[i][j - 1][0] + image[i + 1][j - 1][0] + image[i - 1][j][0] + image[i + 1][j][0] + image[i - 1][j + 1][0] + image[i][j + 1][0] + image[i + 1][j + 1][0]) / 9;
				new_image[i][j][0] = conv_sum;
			}
		}
		for (int i = 0; i < height; i++)
		{
			for (int j = 1; j < width; j++)
			{
				image[i][j][0] = new_image[i][j][0];
				image[i][j][1] = new_image[i][j][0];
				image[i][j][2] = new_image[i][j][0];
			}
		}
	}
	void RGB_pixel_chart()
	{
		int pcnt1[256] = { 0 };
		for (int p = 0; p <= 255; p++)
		{
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					if (p == image[i][j][0]) pcnt1[p] += 1;
				}
			}
		}
		int pcnt2[256] = { 0 };
		for (int p = 0; p <= 255; p++)
		{
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					if (p == image[i][j][1]) pcnt2[p] += 1;
				}
			}
		}
		int pcnt3[256] = { 0 };
		for (int p = 0; p <= 255; p++)
		{
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					if (p == image[i][j][2]) pcnt3[p] += 1;
				}
			}
		}
		ofstream dataFile;
		dataFile.open("C:/cpp_files/bar_chart.txt", ofstream::app);
		fstream file("C:/cpp_files/bar_chart.txt", ios::out);
		dataFile << "red part:" << endl;
		for (int i = 0; i <= 255; i++)
		{
			if (i < 10) dataFile << '0' << '0' << i << '|';
			else if (i >= 10 && i < 100) dataFile << '0' << i << '|';
			else dataFile << i << '|';
			for (int j = 0; j <= pcnt1[i]; j++)
			{
				dataFile << '.';
			}
			dataFile << endl;
		}
		dataFile << "green part:" << endl;
		for (int i = 0; i <= 255; i++)
		{
			if (i < 10) dataFile << '0' << '0' << i << '|';
			else if (i >= 10 && i < 100) dataFile << '0' << i << '|';
			else dataFile << i << '|';
			for (int j = 0; j <= pcnt2[i]; j++)
			{
				dataFile << '.';
			}
			dataFile << endl;
		}
		dataFile << "blue part:" << endl;
		for (int i = 0; i <= 255; i++)
		{
			if (i < 10) dataFile << '0' << '0' << i << '|';
			else if (i >= 10 && i < 100) dataFile << '0' << i << '|';
			else dataFile << i << '|';
			for (int j = 0; j <= pcnt3[i]; j++)
			{
				dataFile << '.';
			}
			dataFile << endl;
		}
		dataFile.close();
	}
	void RGB_equalize()
	{
		int pcnt1[256] = { 0 };
		for (int p = 0; p <= 255; p++)
		{
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					if (p == image[i][j][0]) pcnt1[p] += 1;
				}
			}
		}
		int psum1 = 0;
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				psum1 = 0;
				for (int p = 0; p <= image[i][j][0]; p++)
				{
					psum1 += pcnt1[p];
				}
				image[i][j][0] = 256 * psum1 / height / width;
			}
		}
		int pcnt2[256] = { 0 };
		for (int p = 0; p <= 255; p++)
		{
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					if (p == image[i][j][1]) pcnt2[p] += 1;
				}
			}
		}
		int psum2 = 0;
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				psum2 = 0;
				for (int p = 0; p <= image[i][j][1]; p++)
				{
					psum2 += pcnt2[p];
				}
				image[i][j][1] = 256 * psum2 / height / width;
			}
		}
		int pcnt3[256] = { 0 };
		for (int p = 0; p <= 255; p++)
		{
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					if (p == image[i][j][2]) pcnt3[p] += 1;
				}
			}
		}
		int psum3 = 0;
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				psum3 = 0;
				for (int p = 0; p <= image[i][j][2]; p++)
				{
					psum3 += pcnt3[p];
				}
				image[i][j][2] = 256 * psum3 / height / width;
			}
		}
	}
	void RGB_sharpening()
	{
		int sum1 = 0, sum2 = 0, sum3 = 0, sharp_para = 4;
		for (int i = 1; i < height - 1; i++)
		{
			for (int j = 1; j < width - 1; j++)
			{
				sum1 = abs(image[i][j][0] - image[i + 1][j + 1][0]) + abs(image[i + 1][j][0] - image[i][j + 1][0]);
				sum2 = abs(image[i][j][1] - image[i + 1][j + 1][1]) + abs(image[i + 1][j][1] - image[i][j + 1][1]);
				sum3 = abs(image[i][j][2] - image[i + 1][j + 1][2]) + abs(image[i + 1][j][2] - image[i][j + 1][2]);
				image[i][j][0] -= sum1 * sharp_para;
				image[i][j][1] -= sum2 * sharp_para;
				image[i][j][2] -= sum3 * sharp_para;
			}
		}
	}
	void RGB_smooth()
	{
		uint8_t*** new_image = image;
		int conv_sum;
		for (int k = 0; k < 3; k++)
		{
			for (int i = 1; i < height - 1; i++)
			{
				for (int j = 1; j < width - 1; j++)
				{
					conv_sum = (image[i][j][k] + image[i - 1][j - 1][k] + image[i][j - 1][k] + image[i + 1][j - 1][k] + image[i - 1][j][k] + image[i + 1][j][k] + image[i - 1][j + 1][k] + image[i][j + 1][k] + image[i + 1][j + 1][k]) / 9;
					new_image[i][j][k] = conv_sum;
				}
			}
			for (int i = 0; i < height; i++)
			{
				for (int j = 1; j < width; j++)
				{
					image[i][j][k] = new_image[i][j][k];
				}
			}
		}
	}
	void delete_image()
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
	void setpath(char readPath[])
	{
		char a, b;
		cout << "please enter the number of the picture(from 01 to 12): ";
		cin >> a >> b;
		readPath[13] = a;
		readPath[14] = b;
	}
	void menu(img &image)
	{
		cout << "please select the image processing pattern: " << endl;
		cout << "enter 0: read and save the image without modifying its infomation" << endl;
		cout << "enter 1: switch the image into grey image(method1) and count its pixels" << endl;
		cout << "enter 2: switch the image into grey image(method2) and count its pixels" << endl;
		cout << "enter 3: switch the image into grey image(method1) and equalize the grey image and count its pixels" << endl;
		cout << "enter 4: switch the image into grey image(method1) and sharpen the grey image" << endl;
		cout << "enter 5: count the pixels of the RGB image" << endl;
		cout << "enter 6: equalize the RGB image and count the pixels" << endl;
		cout << "enter 7: sharpen the RGB image" << endl;
		int num;
		cin >> num;
		switch (num)
		{
		case(0): break;
		case(1): image.grey_image1(); image.grey_pixel_chart(); break;
		case(2): image.grey_image2(); image.grey_pixel_chart(); break;
		case(3): image.grey_image1(); image.grey_equalize(); image.grey_pixel_chart(); break;
		case(4): image.grey_image1(); image.grey_sharpening(); image.grey_image1(); image.grey_smooth(); break;
		case(5): image.RGB_pixel_chart(); break;
		case(6): image.RGB_equalize(); image.RGB_pixel_chart(); break;
		case(7): image.RGB_sharpening(); image.RGB_smooth(); break;
		}
	}
};