// Remap.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

//---------宏定义部分-----------------
#define WINDOWS_NAME "程序窗口"

using namespace cv;
using namespace std;

//--------全局变量声明-------------
Mat g_srcImage, g_dstImage;
Mat g_map_X, g_map_Y;

//------全局函数声明部分-----------
int update_map( int key );
static void ShowHelpText();



int main(int argc, char** argv )
{
	//改变系统的颜色和输出辅助信息
	system("color 2F");
	ShowHelpText();

	//载入源图和显示
	g_srcImage = imread("1.jpg");
	if ( !g_srcImage.data)
	{
		printf("cannot find the file ! plese check \n");
		return false;
	}
	imshow("原始图", g_srcImage);

	//创建和原图一样的效果图，x重映射图，y重映射图
	g_dstImage.create( g_srcImage.size(), g_srcImage.type());
	g_map_X.create(g_srcImage.size(), CV_32FC1);
	g_map_Y.create(g_srcImage.size(), CV_32FC1);

	//创建窗口并显示
	namedWindow(WINDOWS_NAME, WINDOW_AUTOSIZE);
	imshow( WINDOWS_NAME, g_srcImage);

	//轮询按键，更新map_X和map_Y的值
	while (1)
	{
		//获取键盘按键
		int key = waitKey(0);

		//判断ESC是否按下，
		if ((key & 255) == 27)
		{
			cout << "程序退出。。。。。。。。。。。。。。\n";
			break;
		}

		//根据按下的键盘来更新map_X和map_Y的值， 然后调用remap()进行重映射
		update_map(key);
		remap(g_srcImage, g_dstImage, g_map_X, g_map_Y, INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));

		//显示效果图
		imshow(WINDOWS_NAME, g_dstImage);
	}

    return 0;
}

//---------------【update_map()函数】---------------------
int update_map(int key)
{
	//双层循环，遍历每一个像素点
	for (int j = 0; j < g_srcImage.rows; j++)
	{
		for (int i = 0; i < g_srcImage.cols; i++)
		{
			switch (key)
			{
			case '1'://进行第一重映射
				if (i > g_srcImage.cols*0.25 && i < g_srcImage.cols*0.75 && j > g_srcImage.rows*0.25 &&j < g_srcImage.rows*0.75)
				{
					g_map_X.at<float>(j, i) = static_cast<float>(2 * (i - g_srcImage.cols*0.25) + 0.5);
					g_map_Y.at<float>(j, i) = static_cast<float>(2 * (j - g_srcImage.rows*0.25) + 0.5);
				}
				else
				{
					g_map_X.at<float>(j, i) = 0;
					g_map_Y.at<float>(j, i) = 0;
				}
				break;
			case '2'://进行第二种映射
				g_map_X.at<float>(j, i) = static_cast<float>(i);
				g_map_Y.at<float>(j, i) = static_cast<float>(g_srcImage.rows - j);
				break;
			case '3'://进行第三种重映射
				g_map_X.at<float>(j, i) = static_cast<float>(g_srcImage.cols-i);
				g_map_Y.at<float>(j, i) = static_cast<float>(j);
				break;
			case '4':
				g_map_X.at<float>(j, i) = static_cast<float>(g_srcImage.cols - i);
				g_map_Y.at<float>(j, i) = static_cast<float>(g_srcImage.rows - j);
				break;
			}
		}
	}
	return 1;
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
	printf("\n\t欢迎来到重映射示例程序~\n\n");
	printf("\n\t按键操作说明: \n\n"
		"\t\t键盘按键【ESC】- 退出程序\n"
		"\t\t键盘按键【1】-  第一种映射方式\n"
		"\t\t键盘按键【2】- 第二种映射方式\n"
		"\t\t键盘按键【3】- 第三种映射方式\n"
		"\t\t键盘按键【4】- 第四种映射方式\n");
}

