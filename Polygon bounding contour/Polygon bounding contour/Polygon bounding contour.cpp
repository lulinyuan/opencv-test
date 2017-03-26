// Polygon bounding contour.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


//----------宏定义部分---------
#define WINDOWS_NAME1 "【彩色原图】"
#define WINDOWS_NAME2 "【灰度图】"
#define WINDOWS_NAME3 "【效果图】"

//------------全局变量声明----------
Mat g_srcImage, g_grayImage;
int g_nThresh = 20, g_nThresh_Max = 255;
RNG g_rng(12345);

//----------全局函数声明-----------
static void ShowHelpText();
void on_ContoursChange(int, void*);





int main()
{
	//改变界面的颜色并输出帮助信息
	system("color 2F");
	ShowHelpText();

	//载入原图 平滑处理 转化为灰度图 显示
	g_srcImage = imread("1.jpg");
	if (!g_srcImage.data)
	{
		printf("cannot find the file!Plese check the file\n");
		return false;
	}
	imshow(WINDOWS_NAME1, g_srcImage);


	cvtColor( g_srcImage, g_grayImage, COLOR_BGR2GRAY);
	blur( g_grayImage, g_grayImage, Size(3,3));

	namedWindow(WINDOWS_NAME2, WINDOW_AUTOSIZE);
	imshow(WINDOWS_NAME2, g_grayImage);

	//设置阈值滑动条并调用处理函数
	createTrackbar("阈值", WINDOWS_NAME2, &g_nThresh, g_nThresh_Max, on_ContoursChange);
	on_ContoursChange(0,0);

	waitKey(0);

    return 0;
}

//----------【on_ContoursChange（）函数】----------
void on_ContoursChange(int, void*) 
{
	//定义一些参数
	Mat threshhold_output;
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;

	//使用Threshold函数检测边缘
	threshold(g_grayImage, threshhold_output,g_nThresh, 255, THRESH_BINARY);

	//找出轮廓
	findContours(threshhold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	//多边形逼近轮廓 + 获取矩形和圆形边界框
	vector<vector<Point>>contours_poly( contours.size());
	vector<Rect>boundRect( contours.size());
	vector<Point2f>center( contours.size());
	vector<float>radius( contours.size());

	//遍历图向的所有部分
	for (unsigned i = 0; i < contours.size(); i++)
	{
		//用指定精度逼近多边形曲线
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		//计算点集的最外面（up-right）矩形边界
		boundRect[i] = boundingRect( Mat(contours_poly[i]));
		//对于给定2D点集，寻找最小面积的包围圆形
		minEnclosingCircle(contours_poly[i], center[i], radius[i] );
	}

	//绘制多边形轮廓 ＋　包围的矩形框　＋　圆形框
	Mat drawing = Mat::zeros( threshhold_output.size(), CV_8UC3 );
	for (unsigned i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar( g_rng.uniform(0,255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
		//绘制轮廓
		drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		//绘制矩形框
		rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
		//绘制圆形框
		circle(drawing, center[i], (int)radius[i], color, 2, 8, 0);


	}

	//显示效果窗口
	namedWindow( WINDOWS_NAME3, WINDOW_AUTOSIZE);
	imshow( WINDOWS_NAME3, drawing);

}
//-----------------------------------【ShowHelpText( )函数】----------------------------------  
//      描述：输出一些帮助信息  
//----------------------------------------------------------------------------------------------  
static void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");

	//输出一些帮助信息  
	printf("\n\n\n\t欢迎来到【创建包围轮廓的矩形和圆形边界框】示例程序~\n\n");
	printf("\n\n\t按键操作说明: \n\n"
		"\t\t键盘按键【ESC】- 退出程序\n\n"
		"\t\t滑动滚动条 - 改变阈值\n\n");
}
