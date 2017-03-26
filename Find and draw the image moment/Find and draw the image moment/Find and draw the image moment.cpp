// Find and draw the image moment.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv; 
using namespace std;

//----------【宏定义部分】----------
#define WINDOWS_NAME1 "【原始彩色图】"
#define WINDOWS_NAME2 "【灰度图】"
#define WINDOWS_NAME3 "【图像轮廓】"

//---------【全局变量声明】--------
Mat g_srcImage, g_gragImage;
int g_nThresh = 20, g_nThresh_Max = 255;
RNG g_rng(12345);
Mat g_cannny_output;
vector<vector<Point>>g_vContours;
vector<Vec4i>g_vHierarchy;

//--------全局函数声明-------
void on_ContoursChange( int, void*);
static void ShowHelpText();



int main()
{
	//改变界面颜色和输出帮助信息
	system("color 5F");
	ShowHelpText();

	//载入原图 转化为灰度图 平滑处理 显示
	g_srcImage = imread("1.jpg",1);
	if (!g_srcImage.data)
	{
		printf("cannot find the file! Plese check the file \n");
		return false;
	}

	imshow(WINDOWS_NAME1,g_srcImage);

	cvtColor(g_srcImage, g_gragImage, COLOR_BGR2GRAY);
	blur( g_gragImage, g_gragImage,Size(3,3));
	
	namedWindow(WINDOWS_NAME2, WINDOW_AUTOSIZE);
	imshow(WINDOWS_NAME2, g_gragImage);

	//创建阈值的滑动条，并进行初始化
	createTrackbar( "阈值", WINDOWS_NAME2, &g_nThresh, g_nThresh_Max,on_ContoursChange);
	on_ContoursChange(0, 0);


	waitKey(0);



    return 0;
}


//----------【on_ContoursChange函数】---------------
void on_ContoursChange( int, void*) 
{
	//使用canny检测边缘
	Canny(g_gragImage, g_cannny_output,g_nThresh, g_nThresh*2, 3);

	//找出轮廓
	findContours( g_cannny_output, g_vContours, g_vHierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	//计算矩
	vector<Moments>mu(g_vContours.size());
	for (unsigned int i = 0; i < g_vContours.size(); i++)
	{
		mu[i] = moments( g_vContours[i], false );

	}

	//计算中心矩
	vector<Point2f>mc(g_vContours.size());
	for (unsigned int i = 0; i < g_vContours.size(); i++)
	{
		mc[i] = Point2f(static_cast<float>(mu[i].m10 / mu[i].m00), static_cast<float>(mu[i].m01 / mu[i].m00));

	}

	//绘制轮廓
	Mat drawing = Mat::zeros( g_cannny_output.size(), CV_8UC3);
	for (unsigned int  i = 0; i < g_vContours.size(); i++)
	{
		Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
		//绘制外层和内层轮廓
		drawContours( drawing, g_vContours, i, color, 2, 8, g_vHierarchy, 0,	Point() );
		//绘制园
		circle( drawing, mc[i], 4, color, -1, 8, 0);
	}

	//显示
	namedWindow(WINDOWS_NAME3,WINDOW_AUTOSIZE);
	imshow(WINDOWS_NAME3, drawing);



	//通过m00计算轮廓面积并且和OpenCV函数对比
	printf("\t 输出内容：面积和轮廓长度");
	for (unsigned int  i = 0; i < g_vContours.size(); i++)
	{
		printf(">通过m00计算轮廓[%d]的面积：（M_00）= %.2f \n OpenCV函数计算出的面积 = %.2f，长度：%.2f \n\n", i, mu[i].m00, 
			contourArea(g_vContours[i]), arcLength( g_vContours[1], true));
		Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
		drawContours(drawing, g_vContours, i, color, 2, 8, g_vHierarchy, 0, Point());
		//绘制园
		circle(drawing, mc[i], 4, color, -1, 8, 0);
	}


}

//-----------------------------------【ShowHelpText( )函数】-----------------------------
//		 描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t非常感谢购买《OpenCV3编程入门》一书！\n");
	printf("\n\n\t\t\t此为本书OpenCV2版的第76个配套示例程序\n");
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");
}