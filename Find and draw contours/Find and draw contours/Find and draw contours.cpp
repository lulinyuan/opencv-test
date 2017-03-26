// Find and draw contours.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//----宏定义部分---
#define NAME_WINDOWS1 "【原始图】"
#define NAME_WINDOWS2 "【轮廓图】"
#define NAME_WINDOWS3 "【彩色图】"



//---------全局变量声明-----------
Mat g_srcImage, g_grayImage;
int g_nThresh = 80;
int g_nThresh_Max = 255;
RNG g_rng(12345);
Mat g_cannyMat_output;
vector<vector<Point>>g_vContours;
vector<Vec4i>g_vHierarchy;

//-------全局函数的声明---------
static void ShowHelpText();
void on_ThreshChange( int, void*);



int main( int argc, char** argv)
{
	//改变界面的颜色和输出辅助信息
	system("color 2F");
	ShowHelpText();

	//载入图像并转化为灰度图显示
	g_srcImage = imread("1.jpg",1);
	if (!g_srcImage.data)
	{
		printf("cannot find the file! Plese check the file\n");
	}
	imshow(NAME_WINDOWS3, g_srcImage);

	cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
	blur(g_grayImage, g_grayImage, Size(3,3));
	imshow(NAME_WINDOWS1, g_grayImage);

	//创建滑动条并初始化
	createTrackbar("canny阈值", NAME_WINDOWS1, &g_nThresh, g_nThresh_Max, on_ThreshChange);
	on_ThreshChange(0, 0);

	waitKey(0);
    return 0;
}

//----------【on_ThreshChange函数】--------------
void on_ThreshChange(int, void*)
{
	//用canny算子检测边缘
	Canny( g_grayImage, g_cannyMat_output, g_nThresh, g_nThresh*2, 3);

	//寻找轮廓
	findContours( g_cannyMat_output, g_vContours, g_vHierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

	//绘出轮廓
	Mat drawing = Mat::zeros(g_cannyMat_output.size(), CV_8UC3);

	for (int i = 0; i < g_vContours.size(); i++)
	{
		Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
		drawContours( drawing, g_vContours, i, color, 2, 8, g_vHierarchy, 0, Point() );

		//显示效果图
		imshow( NAME_WINDOWS2, drawing);



	}

}
//-----------------------------------【ShowHelpText( )函数】----------------------------------  
//      描述：输出一些帮助信息  
//----------------------------------------------------------------------------------------------  
static void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t非常感谢购买《OpenCV3编程入门》一书！\n");
	printf("\n\n\t\t\t此为本书OpenCV2版的第70个配套示例程序\n");
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");

	//输出一些帮助信息  
	printf("\n\n\t欢迎来到【在图形中寻找轮廓】示例程序~\n\n");
	printf("\n\n\t按键操作说明: \n\n"
		"\t\t键盘按键任意键- 退出程序\n\n"
		"\t\t滑动滚动条-改变阈值\n");
}