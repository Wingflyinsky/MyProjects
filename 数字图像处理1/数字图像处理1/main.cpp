#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <gdiplus.h> //֧��png�ȶ���ͼ���ȡ

#pragma comment(lib, "gdiplus.lib") //����Ҫ���ӿ�

using namespace std;
using namespace Gdiplus;

int main()
{
	GdiplusStartupInput gdiplusstartupinput;
	ULONG_PTR gdiplustoken;
	GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL);
	//������ı�Ҫ�ֶ�
	wstring infilename(L"����ͼƬ.png"); //���ַ�������
	Bitmap* bmp = new Bitmap(infilename.c_str()); //��λͼ��ʽ��png
	UINT height = bmp->GetHeight(); //32λ�޷���������
	UINT width = bmp->GetWidth();

	cout << "width " << width << ", height " << height << endl;
	Color color;

	struct pixel_color {
		int R;
		int G;
		int B;
	};

	int yellow_pixel = 0;//��ɫ������
	pixel_color** all_color = new pixel_color*[height]; //�������RGB
	for (int i = 0; i < height; i++) {
		all_color[i] = new pixel_color[width];
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			bmp->GetPixel(x, y, &color);

			all_color[y][x].R = (int)color.GetRed();
			all_color[y][x].G = (int)color.GetGreen();
			all_color[y][x].B = (int)color.GetBlue();
			if (all_color[y][x].R > 220 && all_color[y][x].G > 220 && all_color[y][x].B < 250) {
				yellow_pixel++;
			}
		}
	}

	cout << "��ɫ���ֵ����ռ" << yellow_pixel << "������" << endl;
	cout << "ͼƬ�����ռ" << height * width << "������" << endl;
	cout << "��ɫ���ֵ����ռ�������" << (double)yellow_pixel / height / width * 100 << "%" << endl;

	delete bmp;
	for (int i = 0; i < height; i++) {
		delete all_color[i];
	}
	delete all_color;
	GdiplusShutdown(gdiplustoken);
	return 0;
}
