#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;
bool readBmp(char* bmpName, uint8_t***& image, int& bmpWidth, int& bmpHeight)
{
	FILE* fp = fopen(bmpName, "rb");
	if (fp == 0) return 0;
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);
	BITMAPINFOHEADER head;
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);
	bmpWidth = head.biWidth;
	bmpHeight = head.biHeight;
	int biBitCount = head.biBitCount;
	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
	if (biBitCount != 24)
	{
		cout << "所读图像必须为24位彩色图" << endl;
		return 0;
	}
	unsigned char* pBmpBuf = new unsigned char[lineByte * bmpHeight];
	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);
	int bmpWidthBytes = bmpWidth * 3;
	if (bmpWidth % 4 > 0)
	{
		bmpWidthBytes = (bmpWidthBytes / 4 + 1) * 4;
	}
	image = new uint8_t * *[bmpHeight];
	int i, j, k;
	for (i = 0; i < bmpHeight; ++i)
	{
		image[i] = new uint8_t * [bmpWidth];
		for (j = 0; j < bmpWidth; ++j)
		{
			image[i][j] = new uint8_t[3];
			for (k = 0; k < 3; ++k)
			{
				image[i][j][k] = (uint8_t)pBmpBuf[(bmpHeight - 1 - i) * bmpWidthBytes + j * 3 + k];
			}
		}
	}
	delete pBmpBuf;
	fclose(fp);
	return 1;
}
bool saveBmp(char* bmpName, uint8_t*** image, int width, int height)
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
void grey_image1(uint8_t*** image, int width, int height)
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
void grey_image2(uint8_t*** image, int width, int height)
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
void grey_pixel_chart(uint8_t*** image, int width, int height)
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
	dataFile.open("C:/opencv/files/bar_chart.txt", ofstream::app);
	fstream file("C:/opencv/files/bar_chart.txt", ios::out);
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
void grey_equalize(uint8_t*** image, int width, int height)
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
void grey_sharpening(uint8_t*** image, int width, int height)
{
	int sum = 0, sharp_para = 5;
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
void grey_smooth(uint8_t*** image, int width, int height)
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
void RGB_pixel_chart(uint8_t*** image, int width, int height)
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
	dataFile.open("C:/opencv/files/bar_chart.txt", ofstream::app);
	fstream file("C:/opencv/files/bar_chart.txt", ios::out);
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
void RGB_equalize(uint8_t*** image, int width, int height)
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
int main()
{
	int bmpWidth;
	int bmpHeight;
	uint8_t*** image;
	int i, j, k;
	char readPath[] = "C:/opencv/pictures/8.bmp";
	readBmp(readPath, image, bmpWidth, bmpHeight);
	//grey_image2(image, bmpWidth, bmpHeight);
	//grey_equalize(image, bmpWidth, bmpHeight);
	RGB_equalize(image, bmpWidth, bmpHeight);
	//grey_sharpening(image, bmpWidth, bmpHeight);
	//grey_smooth(image, bmpWidth, bmpHeight);
	//grey_pixel_chart(image, bmpWidth, bmpHeight);
	RGB_pixel_chart(image, bmpWidth, bmpHeight);
	char writePath2[] = "C:/opencv/pictures/output.bmp";
	saveBmp(writePath2, image, bmpWidth, bmpHeight);
	for (i = 0; i < bmpHeight; ++i)
	{
		for (j = 0; j < bmpWidth; ++j)
		{
			delete image[i][j];
		}
		delete image[i];
	}
	delete image;
}