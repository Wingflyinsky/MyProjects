#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <gdiplus.h> //支持png等多种图像读取

#pragma comment(lib, "gdiplus.lib") //还需要连接库

using namespace std;
using namespace Gdiplus;

int main()
{
	GdiplusStartupInput gdiplusstartupinput;
	ULONG_PTR gdiplustoken;
	GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL);
	//启动库的必要手段
	wstring infilename(L"待测图片.png"); //宽字符串类型
	Bitmap* bmp = new Bitmap(infilename.c_str()); //以位图方式打开png
	UINT height = bmp->GetHeight(); //32位无符号整数类
	UINT width = bmp->GetWidth();

	cout << "width " << width << ", height " << height << endl;
	Color color;

	struct pixel_color {
		int R;
		int G;
		int B;
	};

	int yellow_pixel = 0;//黄色像素数
	pixel_color** all_color = new pixel_color*[height]; //存放像素RGB
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

	cout << "黄色部分的面积占" << yellow_pixel << "个像素" << endl;
	cout << "图片总面积占" << height * width << "个像素" << endl;
	cout << "黄色部分的面积占总面积的" << (double)yellow_pixel / height / width * 100 << "%" << endl;

	delete bmp;
	for (int i = 0; i < height; i++) {
		delete all_color[i];
	}
	delete all_color;
	GdiplusShutdown(gdiplustoken);
	return 0;
}
