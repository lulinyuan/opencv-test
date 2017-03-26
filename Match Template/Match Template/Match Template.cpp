// Match Template.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//---------宏定义-------
#define WINDOWS_NAME1 "【原始图片】"
#define WINDOWS_NAME2 "【效果图片】"

//-----全局变量声明---------
Mat g_srcImage, g_templateImage, g_resultImage;

int g_nMatchMethod;
int g_nMaxTrackbarNum = 5;


//-------全局函数定义-----
void on_Matching( int, void* );
static void ShowHelpText();



int main()
{
	//改变界面的颜色并输出帮助信息
	system("color 5F");
	ShowHelpText();

	//载入图像和模板
	g_srcImage = imread("1.jpg", 1);
	g_templateImage = imread("2.jpg", 1);

	//创建显示窗口
	namedWindow( WINDOWS_NAME1, WINDOW_AUTOSIZE);
	namedWindow( WINDOWS_NAME2, WINDOW_AUTOSIZE);


	//创建滑动条并进行一次初始化
	createTrackbar("方法", WINDOWS_NAME1,&g_nMatchMethod, g_nMaxTrackbarNum,on_Matching);

	on_Matching(0,0);

	waitKey(0);

    return 0;
}

//--------【on_Matching()函数】----------
void on_Matching( int, void*)
{
	//给局部变量定义
	Mat srcImage;
	g_srcImage.copyTo( srcImage );

	//初始化用于结果输出的矩阵
	int resultImage_cols = g_srcImage.cols - g_templateImage.cols + 1;
	int resultImage_rows = g_srcImage.rows - g_templateImage.rows + 1;
	g_resultImage.create(resultImage_rows, resultImage_cols, CV_32FC1);

	//进行模板匹配和标准化
	matchTemplate( g_srcImage, g_templateImage, g_resultImage, g_nMatchMethod );
	normalize( g_resultImage, g_resultImage, 0, 1, NORM_MINMAX, -1, Mat() );

	//通过函数minMaxLoc定位最匹配的位置
	double minValue, maxValue;
	Point minLocation, maxLocation;
	Point matchLocation;
	minMaxLoc( g_resultImage, &minValue, &maxValue, &minLocation, &maxLocation, Mat());

	//对于方法SQDIFE和SQDIFE_NORMED,越小的数值代表有着更高的匹配结果，而其余的方法，数值越大匹配效果越好
	if ( g_nMatchMethod == TM_SQDIFF || g_nMatchMethod == TM_SQDIFF_NORMED )
	{
		matchLocation = minLocation;
	}
	else
	{
		matchLocation = maxLocation;
	}
	
	//绘制处矩形，并显示出最终结果
	rectangle(srcImage, matchLocation, Point(matchLocation.x + g_templateImage.cols, matchLocation.y + g_templateImage.rows ), Scalar(0,0,255), 2, 8, 0);
	rectangle( g_resultImage, matchLocation, Point( matchLocation.x + g_templateImage.cols, matchLocation.y + g_templateImage.rows), Scalar(0,0,255),2, 8, 0);

	imshow( WINDOWS_NAME1, srcImage);
	imshow( WINDOWS_NAME2, g_resultImage);

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
	printf("\t欢迎来到【模板匹配】示例程序~\n");
	printf("\n\n\t请调整滑动条观察图像效果\n\n");
	printf("\n\t滑动条对应的方法数值说明: \n\n"
		"\t\t方法【0】- 平方差匹配法(SQDIFF)\n"
		"\t\t方法【1】- 归一化平方差匹配法(SQDIFF NORMED)\n"
		"\t\t方法【2】- 相关匹配法(TM CCORR)\n"
		"\t\t方法【3】- 归一化相关匹配法(TM CCORR NORMED)\n"
		"\t\t方法【4】- 相关系数匹配法(TM COEFF)\n"
		"\t\t方法【5】- 归一化相关系数匹配法(TM COEFF NORMED)\n");
}

