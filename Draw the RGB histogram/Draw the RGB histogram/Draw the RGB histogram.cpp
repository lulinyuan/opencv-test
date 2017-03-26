// Draw the RGB histogram.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define WINDOWS_NAME1 "【原始图】" 
#define WINDOWS_NAME2 "【RGB直方图】" 

//-------【全局函数声明】---------
static  void  ShowHelpText();

int main()
{
	//改变界面颜色和输出帮助信息
	system("color 5F");
	ShowHelpText();


	//载入原图 显示
	Mat srcImage = imread("1.jpg",1);
	imshow(WINDOWS_NAME1,srcImage);

	//参数准备
	int bins = 256;
	int hist_size[] = { bins };
	float range[] = { 0, 256 };
	const float* ranges[] = { range };
	MatND redHist, greenHist, blueHist;
	int channels_r[] = { 0 };

	//进行直方图的计算（红色分量部分）
	calcHist(&srcImage, 1, channels_r, Mat(), redHist, 1, hist_size, ranges, true, false);

	//进行直方图的计算（绿色分量部分）
	int channels_g[] = { 1 };
	calcHist(&srcImage, 1, channels_g, Mat(), greenHist, 1, hist_size, ranges, true, false);

	//进行直方图的计算（蓝色分量部分）
	int channels_b[] = { 2 };
	calcHist(&srcImage, 1, channels_b, Mat(), blueHist, 1, hist_size, ranges, true, false);


	//绘制出三色直方图
	//参数准备
	double maxValue_red, maxValue_green, maxValue_blue;
	
	minMaxLoc(redHist, 0, &maxValue_red, 0, 0 );
	minMaxLoc(redHist, 0, &maxValue_green, 0, 0);
	minMaxLoc(redHist, 0, &maxValue_blue, 0, 0);

	int scale = 1;
	int histHeight = 256;
	Mat histImage = Mat::zeros(histHeight, bins*3, CV_8UC3);

	//开始绘制
	for (int  i = 0; i < bins; i++)
	{
		//参数准备
		float binValue_red = redHist.at<float>(i);
		float binValue_green = greenHist.at<float>(i);
		float binValue_blue = blueHist.at<float>(i);

		int intensity_red = cvRound(binValue_red*histHeight/maxValue_red);//设置绘制的高度
		int intensity_green = cvRound(binValue_green*histHeight / maxValue_red);//设置绘制的高度
		int intensity_blue = cvRound(binValue_blue*histHeight / maxValue_red);//设置绘制的高度


		//绘制红色分量的直方图
		rectangle( histImage, Point(i*scale, histHeight - 1), Point((i + 1)*scale - 1, histHeight - intensity_red),Scalar(255, 0, 0));

		//绘制绿色分量的直方图
		rectangle(histImage, Point((i+bins)*scale, histHeight - 1), Point((i + bins + 1)*scale - 1, histHeight - intensity_green), Scalar(0, 255, 0));

		//绘制蓝色分量的直方图
		rectangle(histImage, Point((i + 2*bins)*scale, histHeight - 1), Point((i + 2*bins+ 1)*scale - 1, histHeight - intensity_blue), Scalar(0, 0, 255));

	}
	//显示一维RGB的直方图

	imshow(WINDOWS_NAME2, histImage);

	waitKey(0);

    return 0;

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
