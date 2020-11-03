#include "bitmap.h"
#define GRAYLEVEL 256

using namespace std;
using namespace Gdiplus;

int main()
{
	GdiplusStartupInput gdiplusstartupinput;
	ULONG_PTR gdiplustoken;
	GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL); //启动库的必要手段

	wstring infilename(L"homework_boy_hist.png"); //宽字符串类型，确定输入文件名
	const char* outputName = "homework_boy_hist.bmp";//输出文件名
	Bitmap* bmp = new Bitmap(infilename.c_str()); //以位图方式打开图片
	UINT height = bmp->GetHeight(); //32位无符号整数类，表示图片高度
	UINT width = bmp->GetWidth();   //32位无符号整数类，表示图片宽度
	Color color;                    //利用该变量读取图片颜色

	int** all_grey = new int*[height]; //存放各像素灰度值
	for (int i = 0; i < height; i++) {
		all_grey[i] = new int[width];
	}
	int level[GRAYLEVEL];//不同灰度级的像素数
	//double origin[GRAYLEVEL];原始直方图，本程序中跳过了这一步;
	double accumulate[GRAYLEVEL];//累计直方图
	int reflection[GRAYLEVEL];//映射关系，第n个元素对应reflection[n]的灰度值
	//初始化灰度级像素数和累计直方图
	for (int i = 0; i < GRAYLEVEL; i++) {
		level[i] = 0;
		accumulate[i] = 0;
	}
	//读取图片像素的RGB值
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			bmp->GetPixel(x, y, &color);
			all_grey[y][x] = round((color.GetRed()+ color.GetGreen()+ color.GetBlue())/3.0);//将RGB值转化为灰度值
			level[all_grey[y][x] / (256 / GRAYLEVEL)]++;
		}
	}

	//计算累计直方图
	for (int i = 0; i < GRAYLEVEL; i++) {
		if (0 != i) {
			accumulate[i] = accumulate[i - 1];
		}
		accumulate[i] += (double)level[i] / width / height;
	}
	//确立映射关系
	for (int i = 0; i < GRAYLEVEL; i++) {
			reflection[i]= round(accumulate[i] * (GRAYLEVEL-1));
			if (reflection[i] == 256) {
				reflection[i]--;
			}
	}
	//利用映射关系更改图片各像素灰度值
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			all_grey[y][x] = reflection[all_grey[y][x]];
		}
	}
	unsigned char* pdata = new unsigned char[(width+1)*height*3]; //存放图片灰度值数据的字符串
	int position = 0;  //pdata字符串中存储的位置
	//将新灰度值信息转化为字符串
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			pdata[position]= unsigned char(all_grey[y][x]);
			pdata[position+1] = unsigned char(all_grey[y][x]);
			pdata[position+2] = unsigned char(all_grey[y][x]);
			position+=3;
		}
		for (int k = 0;width*3%4!=0&&k< 4-width*3 % 4; k++) { //对齐bmp文件
			pdata[position] = unsigned char(255);
			position++;
		}
	}
	saveBitmap(height, width, pdata, position*sizeof(char),outputName);//生成图片
	
	delete bmp;
	for (int i = 0; i < height; i++) {
		delete all_grey[i];
	}
	delete all_grey;
	delete pdata;
	GdiplusShutdown(gdiplustoken);

	return 0;
}
