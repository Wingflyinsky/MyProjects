#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <gdiplus.h> //֧��png�ȶ���ͼ���ȡ
#pragma pack(2)//�����д������sizeof�ò�����ȷ�Ľ��
#pragma comment(lib, "gdiplus.lib") //����Ҫ���ӿ�
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef long    LONG;
typedef struct {   //λͼ�ļ�ͷ��Ϣ
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BITMAPFILEHEADER2;

typedef struct {   //λͼ��Ϣͷ��Ϣ
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

void saveBitmap(int h, int w, unsigned char *pData, int nDatasize, const char *outputName)
{

	// ����λͼ��С
	const int height = h;
	const int width = w;
	const int size = nDatasize;
	double x, y;
	int index;

	// ����λͼ�ļ�ͷ
	BITMAPFILEHEADER2 fileHeader;

	fileHeader.bfType = 0x4D42;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfSize = sizeof(BITMAPFILEHEADER2) + sizeof(BITMAPINFOHEADER2) + size;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER2) + sizeof(BITMAPINFOHEADER2);

	// ����λͼ��Ϣͷ
	BITMAPINFOHEADER2 bitmapHeader = { 0 };

	bitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapHeader.biHeight = -height;
	bitmapHeader.biWidth = width;
	bitmapHeader.biPlanes = 1;
	bitmapHeader.biBitCount = 24;
	bitmapHeader.biSizeImage = size;
	bitmapHeader.biCompression = 0; //BI_RGB


	// д���ļ���
	FILE *output = fopen(outputName, "wb");

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
