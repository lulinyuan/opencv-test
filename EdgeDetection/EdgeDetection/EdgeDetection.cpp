// EdgeDetection.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//-----------全局变量声明
//原图、灰度图、目标图
Mat g_srcImage, g_grayImage, g_dstImage;

//Canny边缘检测相关变量
Mat g_cannyDetectedEdges;
int g_cannyLowThreshold = 1;//Tracker位置参数

//Sobel边缘检测相关变量
Mat g_sobelGradient_X, g_sobelGradient_Y;
Mat g_sobelAbsGradient_X, g_sobelAbsGradient_Y;
int g_sobelKernelSize = 1;

//Scharr滤波器相关变量
Mat g_scharrGradient_X, g_scharrGradient_Y;
Mat g_scharrAbsGradient_X, g_scharrAbsGradient_Y;

//-------------全局函数声明部分
static void ShowHelpText();
static void on_Canny(int, void*);//Canny边缘检测窗口滑动条的回调函数
static void on_Sobel(int, void*);//Sobel边缘检测窗口滑动条的回调函数
void Scharr();//封装了Scharr边缘检测的相关代码函数



int main()
{
	//系统颜色和显示系统颜色
	system("color 5F");
	ShowHelpText();

	//载入源图 
	g_srcImage = imread("1.jpg");
	if (!g_srcImage.data)
	{
		printf("cannot read the ppicture !plese check!\n");
		return false;
	}
	//显示原始图
	namedWindow("【原始图】");
	imshow("【原始图】", g_srcImage);

	//创建与src同类型和大小的矩阵dst
	g_dstImage.create( g_srcImage.size(), g_srcImage.type() );

	//将原图转化为灰度图
	cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);

	//创建显示窗口
	namedWindow("Canny detection", WINDOW_AUTOSIZE);
	namedWindow("Sobel detection", WINDOW_AUTOSIZE);
	//创建Trackbar
	createTrackbar("参数值", "Canny detection", &g_cannyLowThreshold, 120, on_Canny);
	createTrackbar("参数值", "Sobel detection", &g_sobelKernelSize, 3, on_Sobel);

	//调用回调函数
	on_Canny(0, 0);
	on_Sobel(0, 0);

	//调用封装Scharr边缘检测代码的函数
	Scharr();

	//轮询获取按键信息， 若按下“q”则退出

	while ((char(waitKey(1)) != 'q'))
	{

	}
    return 0;
}
//-----------------------------------【ShowHelpText( )函数】----------------------------------
//		描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");

	//输出一些帮助信息
	printf("\n\n\t运行成功，请调整滚动条观察图像效果~\n\n"
		"\t按下“q”键时，程序退出。\n");
}
//-----------------------------【on_Canny()函数】-----------------------------------
void on_Canny(int, void*)
{
	//先使用3*3内核来降噪
	blur(g_grayImage, g_cannyDetectedEdges, Size(3, 3));

	//调用Canny函数
	Canny( g_cannyDetectedEdges, g_cannyDetectedEdges, g_cannyLowThreshold, g_cannyLowThreshold * 3, 3);

	//将g_dstImage中元素清零
	g_dstImage = Scalar::all(0);

	//使用Canny算子输出的边缘图g_cannyDetectedEdages作为掩码，来将原图g_srcImage拷贝到g_dstImage  
	g_srcImage.copyTo(g_dstImage, g_cannyDetectedEdges);
	//显示效果图
	imshow("Canny detection", g_dstImage);
}
//--------------------------【on_Sobel函数】-----------------------

void on_Sobel(int, void*)
{
	//求X方向的梯度
	Sobel(g_srcImage, g_sobelGradient_X, CV_16S, 1, 0, (2 * g_sobelKernelSize + 1), 1, 1, BORDER_DEFAULT);

	//计算绝对值，并经就结果转换成8位
	convertScaleAbs(g_sobelGradient_X, g_sobelAbsGradient_X);

	//求解Y方向的梯度
	Sobel(g_srcImage, g_sobelGradient_Y, CV_16S, 0, 1, (2 * g_sobelKernelSize + 1), 1, 1, BORDER_DEFAULT);
	//计算绝对值，并经就结果转换成8位
	convertScaleAbs(g_sobelGradient_Y, g_sobelAbsGradient_Y);

	//合并梯度
	addWeighted(g_sobelAbsGradient_X, 0.5, g_sobelAbsGradient_Y, 0.5, 0, g_dstImage);

	//显示效果图
	imshow("Sobel detection",g_dstImage);

}

//----------------【Scharr()函数】-------------
void Scharr() 
{
	//求X方向的梯度
	Scharr(g_srcImage, g_scharrGradient_X, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);

	//计算绝对值，并经就结果转换成8位
	convertScaleAbs(g_scharrGradient_X, g_scharrAbsGradient_X);

	//求解Y方向的梯度
	Scharr(g_srcImage, g_scharrGradient_Y, CV_16S, 0, 1, 1, 0, BORDER_DEFAULT);
	//计算绝对值，并经就结果转换成8位
	convertScaleAbs(g_scharrGradient_Y, g_scharrAbsGradient_Y);

	//合并梯度
	addWeighted(g_scharrAbsGradient_X, 0.5, g_scharrAbsGradient_Y, 0.5, 0, g_dstImage);

	//显示效果图
	imshow("Scharr Detection", g_dstImage);

}