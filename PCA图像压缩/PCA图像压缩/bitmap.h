#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <gdiplus.h> //支持png等多种图像读取
#pragma pack(2)//必须得写，否则sizeof得不到正确的结果
#pragma comment(lib, "gdiplus.lib") //还需要连接库

using namespace std;
using namespace Gdiplus;

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef long    LONG;
const int TRAINING_TIME = 500;

typedef struct {
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BITMAPFILEHEADER2;

typedef struct {
	DWORD      biSize;
	DWORD       biWidth;
	DWORD       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;
	DWORD       biXPelsPerMeter;
	DWORD       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
} BITMAPINFOHEADER2;

void saveBitmap(int h, int w, unsigned char *pData, int nDatasize)
{

	// 定义位图大小
	const int height = h;
	const int width = w;
	const int size = nDatasize;
	double x, y;
	int index;

	// 第一部分，生成位图的文件头信息
	BITMAPFILEHEADER2 fileHeader;

	fileHeader.bfType = 0x4D42;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfSize = sizeof(BITMAPFILEHEADER2) + sizeof(BITMAPINFOHEADER2) + size;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER2) + sizeof(BITMAPINFOHEADER2);

	// 第二部分，生成位图信息头
	BITMAPINFOHEADER2 bitmapHeader = { 0 };

	bitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapHeader.biHeight = -height;
	bitmapHeader.biWidth = width;
	bitmapHeader.biPlanes = 1;
	bitmapHeader.biBitCount = 24;
	bitmapHeader.biSizeImage = size;
	bitmapHeader.biCompression = 0; //BI_RGB


	// 第三部分，输出图片
	string outputName =  "output";
	string training_time = std::to_string(TRAINING_TIME);
	outputName.append(training_time);
	outputName.append(".bmp");
	const char* file_name = outputName.c_str();

	FILE *output = fopen(file_name, "wb");

	if (output == NULL)
	{
		printf("Cannot open file!\n");
	}
	else
	{
		fwrite(&fileHeader, sizeof(BITMAPFILEHEADER2), 1, output);
		fwrite(&bitmapHeader, sizeof(BITMAPINFOHEADER2), 1, output);
		fwrite(pData, size, 1, output);
		fclose(output);
	}
}

