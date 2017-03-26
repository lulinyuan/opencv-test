// Draw the One-dimensional histogram.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define WINDOWS_NAME1 "【原始图】"
#define WINDOWS_NAME2 "【一维H-S直方图】"


//----------------【ShowHelpText】-----------
static void ShowHelpText();


int main()
{
	//改变界面颜色并输出帮助信息
	system("color 5F");
	ShowHelpText();

	Mat srcImage = imread("1.jpg",0 );
	imshow(WINDOWS_NAME1, srcImage);
	if (!srcImage.data)
	{
		printf("cannot find the file, Plese chese it \n");
		return false;
	}

	//定义变量
	MatND dstHist;//在CV中用CvHistogram *Hist = cvCreatHist
	int dims = 1;
	float hranges[] = {0,255};
	const float *ranges[] = { hranges };
	int size = 256;
	int channel = 0;

	//计算图像的直方图
	calcHist(&srcImage, 1, &channel, Mat(), dstHist, dims, &size, ranges);
	int scale = 1;
	Mat dstImage(size * scale, size, CV_8U, Scalar(0) );


	//获取最大值和最小值
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc( dstHist, &minValue, &maxValue, 0, 0);//在cv中用的是CVGETMinMaxHistValue

	//绘制出直方图
	int hpt = saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++)
	{
		float binValue = dstHist.at<float>(i);

		int realValue = saturate_cast<int>(binValue*hpt/maxValue);
		rectangle( dstImage, Point(i*scale,size - 1),  Point((i+1)*scale - 1, size - realValue ), Scalar(255));
	}

	//显示图像
	imshow(WINDOWS_NAME2, dstImage);
	waitKey(0);

    return 0;
}
//-----------------------------------【ShowHelpText( )函数】-----------------------------
//		 描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");
}

