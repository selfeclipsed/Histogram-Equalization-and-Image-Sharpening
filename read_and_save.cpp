#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#include<iostream>
using namespace std;

/***********************************************************************
* 函数名称：
* readBmp()
*
*函数参数：
*  char *bmpName -文件名字及路径
*  uint8_t *** &image -要将像素值存储到的内存空间的指针
*  int& bmpWidth 图像宽度
*  int& bmpHeight 图像高度
*返回值：
*   bool类型 0为失败,1为成功
*   uint8_t *** &image，三维数组,大小为bmpHeight*bmpWidth*3
*   int& bmpWidth 图像宽度
*   int& bmpHeight 图像高度
*说明：给定一个图像文件名及其路径，读图像的像素数据、宽、高等数据进内存,存放在相应的变量中
***********************************************************************/
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
	if (biBitCount != 24) {
		cout << "所读图像必须为24位彩色图" << endl;
		return 0;
	}
	unsigned char* pBmpBuf = new unsigned char[lineByte * bmpHeight];
	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);
	int bmpWidthBytes = bmpWidth * 3;
	if (bmpWidth % 4 > 0) {
		bmpWidthBytes = (bmpWidthBytes / 4 + 1) * 4;
	}
	image = new uint8_t * *[bmpHeight];
	int i, j, k;
	for (i = 0; i < bmpHeight; ++i) {
		image[i] = new uint8_t * [bmpWidth];
		for (j = 0; j < bmpWidth; ++j) {
			image[i][j] = new uint8_t[3];
			for (k = 0; k < 3; ++k) {
				image[i][j][k] = (uint8_t)pBmpBuf[(bmpHeight - 1 - i) * bmpWidthBytes + j * 3 + k];
			}
		}
	}
	delete pBmpBuf;
	fclose(fp);
	return 1;
}

/***********************************************************************
* 函数名称：
* saveBmp()
*
*函数参数：
*  char *bmpName -文件名字及路径
*  uint8_t *** image  -待存盘的像素数据 大小为height*width*3的三维数组
*  int width   -像素为单位待存盘位图的宽
*  int  height  -像素为单位待存盘位图高
*返回值：
*   0为失败,1为成功
*
*说明：给定一个图像像素数据、宽、高信息，将其写到指定文件中
***********************************************************************/
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
	for (i = 0; i < height; ++i) {
		for (j = 0; j < width; ++j) {
			for (k = 0; k < 3; ++k) {
				imgBuf[(height - 1 - i) * lineByte + j * 3 + k] = image[i][j][k];
			}
		}
	}
	fwrite(imgBuf, height * lineByte, 1, fp);
	delete imgBuf;
	fclose(fp);
	return 1;
}


//文件存取函数调用的例子
int main()
{
	int bmpWidth;//图像的宽
	int bmpHeight;//图像的高
	uint8_t*** image;
	int i, j;

	//读取图像, 将像素值存储到image中
	char readPath[] = "C:/opencv/pictures/11.bmp";
	readBmp(readPath, image, bmpWidth, bmpHeight);
	cout << "width=" << bmpWidth << " height=" << bmpHeight << endl;

	//存储image
	char writePath[] = "C:/opencv/pictures/my_image.bmp";
	saveBmp(writePath, image, bmpWidth, bmpHeight);

	//清除image以及grey image空间
	for (i = 0; i < bmpHeight; ++i) {
		for (j = 0; j < bmpWidth; ++j) {
			delete image[i][j];
		}
		delete image[i];
	}
	delete image;
}
