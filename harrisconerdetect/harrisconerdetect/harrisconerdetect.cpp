// harrisconerdetect.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>

#define WINDOW_NAME1 "【彩色角点图】"
#define WINDOW_NAME2 "【角点检测图】"
#define WINDOW_NAME3 "【原始图】"

using namespace cv;
//全局变量声明
Mat g_srcImage, g_srcImage1, g_grayImage;
int thresh = 10;
int Max_thresh = 175;

//全局函数声明
void on_CornerHarris( int, void*);
static void ShowHelpText();





int main()
{	
	//改变系统的输出界面颜色好人输出帮助信息
	system("color 5F");
	ShowHelpText();

	//创建显示窗口
	namedWindow( WINDOW_NAME1, WINDOW_AUTOSIZE );
	namedWindow( WINDOW_NAME2, WINDOW_AUTOSIZE );
	namedWindow(WINDOW_NAME3, WINDOW_AUTOSIZE);

	//载入原始图像并进行克隆
	g_srcImage = imread("1.jpg", 1);
	if ( !g_srcImage.data)
	{
		printf("cannot find the picture ,Plese check! \n");
		return false;
	}

	g_srcImage1=g_srcImage.clone();
	imshow(WINDOW_NAME3, g_srcImage);
	
	//转化为灰度图
	cvtColor(g_srcImage1, g_grayImage, COLOR_BGR2GRAY);



	//创建滑动条进行一次初始化
	createTrackbar( "阈值", WINDOW_NAME1, &thresh, Max_thresh, on_CornerHarris );

	//调用一次回调函数并初始化
	on_CornerHarris( 0, 0);

	waitKey(0);

    return 0;
}

//--------------【on_CornerHarris()函数】----------
void on_CornerHarris( int, void* ) 
{
	//定义局部变量
	Mat dstImage;//目标图
	Mat normImage;//归一化后的图
	Mat scaledImage;//线性化的八位无符号位整型的图

	//初始化
	dstImage = Mat::zeros( g_srcImage.size(), CV_32FC1 );
	g_srcImage1 = g_srcImage.clone();
	
	//进行角点检测
	cornerHarris( g_grayImage, dstImage, 2, 3, 0.04, BORDER_DEFAULT );

	//归一化处理
	normalize( dstImage, normImage, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
	convertScaleAbs(normImage, scaledImage);//将归一化后的图像线性变换成8位无符号位整型

	//进行绘制
	//将检测到复核阈值的角点绘制出来
	for (int j = 0; j < normImage.rows; j++)
	{
		for (int i = 0; i < normImage.cols; i++)
		{
			if ((int) normImage.at<float>( j, i ) > thresh+80)
			{
				circle(g_srcImage1, Point(i, j), 5, Scalar(10, 10, 255), 2, 8, 0);
				circle(scaledImage, Point(i, j), 5, Scalar(0, 10, 255), 2, 8, 0);
			}
			
		}
	}

	//显示效果图
	imshow( WINDOW_NAME1, g_srcImage1);
	imshow( WINDOW_NAME2, scaledImage);

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
	printf("\n\n\n\t【欢迎来到Harris角点检测示例程序~】\n\n");
	printf("\n\t请调整滚动条观察图像效果~\n\n");
}

