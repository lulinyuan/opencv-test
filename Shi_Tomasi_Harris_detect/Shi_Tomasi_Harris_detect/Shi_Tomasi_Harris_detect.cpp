// Shi_Tomasi_Harris_detect.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

#define WINDOW_NAME1 "【原始图】"
#define WINDOW_NAME2 "【Shi-Tomasi角点检测】"

using namespace cv;
using namespace std;




//全局变量声明
Mat g_srcImage, g_grayImage;
int g_maxCornerNumber = 10;
int g_maxTrackbarNumber= 500;
RNG g_rng(12345);



//全局函数声明
static void ShowHelpText();
void on_goodFeaturesToTrack_Demo( int, void* );


int main()
{
	//改变系统的输出颜色输出帮助信息
	system("color 5F");
	ShowHelpText();


	//创建显示窗口和滑动条，并进行显示和回调函数初始化
	namedWindow(WINDOW_NAME1, WINDOW_AUTOSIZE);
	namedWindow(WINDOW_NAME2, WINDOW_AUTOSIZE);

	//载入原图并转化为灰度图
	g_srcImage = imread( "1.jpg", 1 );
	if (!g_srcImage.data )
	{
		printf("cannot find the file, Plese check! ");
		return false;
	}
	imshow(WINDOW_NAME1, g_srcImage);

	cvtColor( g_srcImage, g_grayImage, COLOR_BGR2GRAY );
	


	createTrackbar("最大角点数", WINDOW_NAME2, &g_maxCornerNumber, g_maxTrackbarNumber, on_goodFeaturesToTrack_Demo );

	on_goodFeaturesToTrack_Demo( 0, 0);



	waitKey(0);

    return 0;
}

//------------【on_goodFeaturesToTrack_Demo()函数】-----------
void on_goodFeaturesToTrack_Demo(int, void*) 
{
	//对于变量小于1时的处理
	if ( g_maxCornerNumber <= 1)
	{
		g_maxCornerNumber = 1;
	}

	//Shi-Tomasi算法（goodFeatureToTrack函数）的参数准备
	vector<Point2f> corner;
	double qualityLevel = 0.01;//角点检测可接受的最小特征值
	double minDistance = 10;//角点间的最小距离
	int blockSize = 3;//计算到导数自相关矩阵时指定的邻域范围
	double k = 0.04;//权重系数
	Mat copy = g_srcImage.clone();//复制原图像到一个临时变量中，作为感兴趣区域

	//进行Shi-Tomasi角点检测
	goodFeaturesToTrack( g_grayImage, corner, g_maxCornerNumber, qualityLevel, minDistance, Mat(), blockSize, false, k );

	//输出文本信息
	cout << ">此次检测到的焦点数量为" << corner.size() << endl;


	//绘制检测到的角点
	int r = 4;
	for (unsigned int  i = 0; i < corner.size() ; i++)
	{
		//绘制点
		circle(copy, corner[i], r, Scalar(10, 10, 255), -1, 8, 0);
	}

	//显示图像
	imshow(WINDOW_NAME2, copy);

	//亚像素角点检测的参数设定
	Size winSize = Size(5, 5);
	Size zeroZone = Size(-1, -1);

	TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 40, 0.001);

	//计算出亚像素焦点位置
	cornerSubPix(g_grayImage, corner, winSize, zeroZone, criteria);

	//输出角点信息
	for (int i = 0; i < corner.size(); i++)
	{	
		cout << " \t精确点坐标[" << i << "](" << corner[i].x << "," << corner[i].y << ")" << endl;

	}


}
//-----------------------------------【ShowHelpText( )函数】----------------------------------
//          描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t非常感谢购买《OpenCV3编程入门》一书！\n");
	printf("\n\n\t\t\t此为本书OpenCV2版的第87个配套示例程序\n");
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");
	//输出一些帮助信息
	printf("\n\n\n\t欢迎来到【Shi-Tomasi角点检测】示例程序\n");
	printf("\n\t请调整滑动条观察图像效果\n\n");

}
