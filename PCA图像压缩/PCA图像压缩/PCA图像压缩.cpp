#include "bitmap.h"

int main()
{
	GdiplusStartupInput gdiplusstartupinput;
	ULONG_PTR gdiplustoken;
	GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL); //启动库的必要手段
	Color color;

	wstring infilename(L"Lenna.bmp"); //宽字符串类型
	Bitmap* bmp = new Bitmap(infilename.c_str()); //以位图方式打开png
	UINT height = bmp->GetHeight(); //32位无符号整数类
	UINT width = bmp->GetWidth();
	/*读取图片并分块*/
	int** blocks = new int*[4096];
	for (int i = 0; i < 4096; i++) {
		blocks[i] = new int[64];
	}

	int blockid = 0; //块号
	int pixelid = 0; //块内像素号
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			bmp->GetPixel(x, y, &color);
			blockid = y / 8 * 64 + x / 8;
			pixelid = y % 8 * 8 + x % 8;
			/*计算每个像素的灰度值并四舍五入到整数*/
			blocks[blockid][pixelid] = round((color.GetRed() + color.GetGreen() + color.GetBlue()) / 3.0);
		}
	}

	double alpha = 0.01;               //学习因子
	double w[64];                     //权值
	for (int i = 0; i < 64; i++) {    //权值初始化
		w[i] = rand() % 100 / 1000.0;
	}
	int blockSelected;      //随机选择的训练样本
	double y;
	/*开始训练神经网络*/               
	for (int i = 0; i < TRAINING_TIME; i++) {
		y = 0;
		blockSelected = rand() % 4096;
		for (int j = 0; j < 64; j++) {
			y += blocks[blockSelected][j]/256.0 * w[j];
		}
		for (int j = 0; j < 64; j++) {
			w[j] = w[j] + alpha * (y*blocks[blockSelected][j]/256.0 - y * y*w[j]);
		}
	}

	for (blockid = 0; blockid < 4096; blockid++) {  //主成分压缩
		y = 0;
		for (int pixelid = 0; pixelid < 64; pixelid++) {
			y += w[pixelid] * blocks[blockid][pixelid]/256.0 ;
		}
		for (int pixelid = 0; pixelid < 64; pixelid++) {
			blocks[blockid][pixelid] = round( 256 * w[pixelid] * y);
		}
	}
	blockid = 0;
	pixelid = 0;
	unsigned char* pdata = new unsigned char[width * height * 3]; //存放图片的灰度值信息
	int position = 0;   //记录图片输出过程中的像素位置
	while (position < width * height * 3) {
		pdata[position] = unsigned char(blocks[blockid][pixelid]);
		pdata[position + 1] = unsigned char(blocks[blockid][pixelid]);
		pdata[position + 2] = unsigned char(blocks[blockid][pixelid]);
		position += 3;
		if (pixelid % 8 == 7) {
			if (pixelid == 63 && blockid % 64 == 63) {
				blockid++;
				pixelid = 0;
			}
			else if (blockid % 64 == 63) {
				blockid = blockid / 64 * 64;
				pixelid++;
			}
			else {
				blockid++;
				pixelid = pixelid / 8 * 8;
			}

		}
		else {
			pixelid++;
		}

	}
	/*输出图片*/
	saveBitmap(height, width, pdata, position * sizeof(char)); 
	return 0;
}
