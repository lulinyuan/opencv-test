// Trackerbar.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>

using namespace cv;
#define WINDOWS_NAME "linearly blend"    //为窗口标题定义宏

//***********************全局变量声明*****************************
// 描述：全局变量
//****************************************************************
const int g_nMaxAlphaValue = 100;//Alpha值的最大值
int	g_nAlphaValueSlider;//滑动条对应的值
double g_dAlphaValue;
double g_dBetValue;

//声明存储图像的的变量
Mat g_srcImage1;
Mat g_srcImage2;
Mat g_dstImage;


//on_Trackerbar()函数
//描述：响应滑条的回调函数


void on_Trackerbar(int, void*)
{
	//求出当前值相对于最大值的比例
	g_dAlphaValue = (double)g_nAlphaValueSlider/g_nMaxAlphaValue;
	//则BETA值为1减去alpha值
	g_dBetValue = (1 - g_dAlphaValue);


	//根据alpha和BETA的值进行线性混合
	addWeighted(g_srcImage1,g_dAlphaValue,g_srcImage2,g_dBetValue,0.0,g_dstImage);

	std::cout<<"g_dAlphaValue:"<< g_dAlphaValue << "g_dBetValue:" <<g_dBetValue<<std::endl;
	//显示效果图
	imshow(WINDOWS_NAME,g_dstImage);

}


int main(int argc, char** argv)
{
	//加载图像（两个图像尺寸需要相同）
	g_srcImage1 = imread("1.jpg");
	g_srcImage2 = imread("2.jpg");
	
	if ( !g_srcImage1.data )
	{
		printf("读取第一图片失败，请确认目录下是否有相应文件存在~! \n");
		return -1;
	}
	if (!g_srcImage2.data)
	{
		printf("读取第二图片失败，请确认目录下是否有相应文件存在~! \n");
		return -1;
	}

	//设置滑条初值为70
	g_nAlphaValueSlider = 70;
	//创建窗口
	namedWindow(WINDOWS_NAME,1);
	//在创建的窗体中创建一个滑动条控件
	char TrackbarName[50];
	sprintf( TrackbarName, "Transparency value %d",g_nMaxAlphaValue);

	createTrackbar( TrackbarName,WINDOWS_NAME,&g_nAlphaValueSlider,g_nMaxAlphaValue,on_Trackerbar );

	//结果在回调函数中显示
	on_Trackerbar( g_nAlphaValueSlider, 0 );


	//按任意键停止
	waitKey(0);


    return 0;
}

