// Convexhull.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//----------宏定义----------
#define WINDOWS_NAME1 "【彩色原图】"
#define WINDOWS_NAME2 "【灰度图】"
#define WINDOWS_NAME3 "【效果图】"

//-------全局变量定义----------
Mat g_srcImage, g_grayImage;
int g_nThresh = 50, g_nThresh_Max = 255;
RNG g_rng(12345);
Mat srcImage_copy = g_srcImage.clone();
Mat g_threshholdImage_ouput;
vector<vector<Point>>g_vContours;
vector<Vec4i>g_vHierarchy;

//-------全局函数声明---------
static void ShowHelpText();
void on_ThreshChange(int, void*);




int main()
{
	//改变界面颜色和输出帮助信息
	system("color 2F");
	ShowHelpText();
	

	//载入原图、灰度图、降噪和显示
	g_srcImage = imread("1.jpg", 1);
	if (!g_srcImage.data)
	{
		printf("cannot find the file ! Plese check the file \n");
		return false;
	}
	imshow(WINDOWS_NAME1, g_srcImage);

	cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
	blur(g_grayImage, g_grayImage, Size(3,3));

	namedWindow(WINDOWS_NAME2, WINDOW_AUTOSIZE);
	imshow(WINDOWS_NAME2, g_grayImage);

	//创建阈值滑动条
	createTrackbar( "阈值:", WINDOWS_NAME2, &g_nThresh, g_nThresh_Max, on_ThreshChange);
	on_ThreshChange(0,0);

	waitKey(0);


    return 0;
}
//-------【on_ThreshChange函数】------
void on_ThreshChange(int, void*) 
{
	//对图像进行二值化处理， 控制阈值
	threshold( g_grayImage, g_threshholdImage_ouput, g_nThresh, 255, THRESH_BINARY);

	//寻找轮廓
	findContours( g_threshholdImage_ouput, g_vContours, g_vHierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0,0));

	//遍历每一个轮廓，寻找凸包
	vector<vector<Point>>hull(g_vContours.size());
	for (unsigned i = 0; i < g_vContours.size(); i++)
	{
		convexHull( Mat(g_vContours[i]), hull[i], false );

	}

	//绘制轮廓和凸包
	Mat drawing = Mat::zeros(g_threshholdImage_ouput.size(), CV_8UC3);
	for (unsigned i = 0; i < g_vContours.size(); i++)
	{
		Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
		drawContours(drawing, g_vContours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		drawContours(drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point());

	}

	//显示效果图
	imshow(WINDOWS_NAME3, drawing);

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


