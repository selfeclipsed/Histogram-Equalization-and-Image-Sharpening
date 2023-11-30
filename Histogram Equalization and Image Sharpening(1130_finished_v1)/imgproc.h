#ifndef IMGPROC_H_INCLUDED
#define IMGPROC_H_INCLUDED
#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "imgproc.h"
using namespace std;
bool readBmp(char* bmpName, uint8_t***& image, int& bmpWidth, int& bmpHeight);
bool saveBmp(char* bmpName, uint8_t*** image, int width, int height);
void grey_image1(uint8_t***& image, int width, int height);
void grey_image2(uint8_t***& image, int width, int height);
void grey_pixel_chart(uint8_t*** image, int width, int height);
void grey_equalize(uint8_t*** image, int width, int height);
void grey_sharpening(uint8_t*** image, int width, int height);
void grey_smooth(uint8_t*** image, int width, int height);
void RGB_pixel_chart(uint8_t*** image, int width, int height);
void RGB_equalize(uint8_t*** image, int width, int height);
void RGB_sharpening(uint8_t*** image, int width, int height);
void RGB_smooth(uint8_t*** image, int width, int height);
void delete_image(uint8_t*** image, int width, int height);
void set_path(char readPath[]);
void menu(uint8_t***& image, int width, int height);
#endif // MODULE1_H_INCLUDED