// Reflection projection.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>


using namespace cv;
using namespace std;

#define WINDOWS_NAME "【原始图】"
#define WINDOWS_NAME1 "【H-S直方图】"
#define WINDOWS_NAME2 "【反射投影图】"

//------全局变量声明--------
Mat g_srcImage, g_hsvImage, g_hueImage;
int g_bins = 30;


//----【全局函数声明部分】-------
static void ShowHelpText();
void on_BinChange(int, void*);


int main()
{
	//改变输出界面的颜色和输出帮助信息
	system("color 5F");
	ShowHelpText();

	//载入图像 转化为HSV空间
	g_srcImage = imread("1.jpg",1);
	if (!g_srcImage.data)
	{
		printf("cannot find the file，Plese check it!\n");
		return false;
	}
	cvtColor( g_srcImage, g_hsvImage, CV_BGR2HSV);


	//分离hue色调通道
	g_hueImage.create( g_hsvImage.size(),g_hsvImage.depth());
	int ch[] = { 0,0 };
	mixChannels(&g_hsvImage, 1, &g_hueImage, 1, ch, 1);

	//创建Trackbar 来输入bin的数目
	namedWindow(WINDOWS_NAME,WINDOW_AUTOSIZE );
	createTrackbar( "色调组距", WINDOWS_NAME, &g_bins, 180, on_BinChange);

	on_BinChange( 0, 0);


	//显示效果图
	imshow(WINDOWS_NAME, g_srcImage );

	waitKey(0);


    return 0;
}

//---------------【on_BinChange()函数】
void on_BinChange( int, void*)
{
	//参数准备
	MatND hist;
	int histSize = MAX( g_bins, 2);
	float hue_range[] = { 0, 180 };
	const float* ranges = { hue_range };
	//计算直方图并归一化
	calcHist( &g_hueImage, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false );
	normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	//计算反向投影
	MatND backproj;
	calcBackProject( &g_hueImage, 1, 0, hist, backproj, &ranges, 1, true );

	//显示反向投影
	imshow(WINDOWS_NAME1, backproj);

	//绘制直方图的参数准备
	int w = 400, h = 400;
	int bin_w = cvRound((double) w / histSize);
	Mat histImg = Mat::zeros( w, h, CV_8UC3);

	//绘制直方图
	for (int  i = 0; i < g_bins; i++)
	{
		rectangle(histImg, Point(i*bin_w, h), Point((i + 1)*bin_w, h - cvRound(hist.at<float>(i)*h / 255.0)), Scalar(100, 123, 255), -1);
	}


	//显示直方图窗口
	imshow(WINDOWS_NAME2, histImg );

}


//-----------------------------------【ShowHelpText( )函数】----------------------------------
//          描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");

	//输出一些帮助信息
	printf("\n\n\t欢迎来到【反向投影】示例程序\n\n");
	printf("\n\t请调整滑动条观察图像效果\n\n");

}

