// Draw HS histogram.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

#define WINDOWS_NAME1 "原始图"
#define WINDOWS_NAME2 "H-S 效果图"
 
using namespace cv;

//--------------全局函数定义
static void ShowHelpText();


int main()
{
	//改变界面颜色和输出帮助信息
	system("color 5F");
	ShowHelpText();

	//载入图像 转化为Hsv颜色模型

	Mat srcImage, hsvImage;
	srcImage = imread("1.jpg", 1);

	cvtColor( srcImage, hsvImage, COLOR_BGR2HSV);

	//参数准备
	//将色调量化为30个等级，将饱和度量化为32个等级
	int hueBinNum = 30;//色调的直方图直条数量
	int saturationBinNum = 32; //饱和度的直方图直条数量
	int histSize[] = {hueBinNum,saturationBinNum};

	//定义色调的变化范围为0到179
	float hueRanges[] = { 0, 180 };
	//定义饱和度的变化范围为0（黑、白、灰）到255（纯光谱颜色）
	float saturationRanges[] = {0 ,256};
	const float* ranges[] = { hueRanges, saturationRanges};
	MatND dstHist;
	//参数准备， calHist函数中将计算第0通道和第1通道的直方图
	int channel[] = { 0,1 };

	//调用calHist，进行直方图计算
	calcHist(&hsvImage,//输入数组
		1,//数组个数为1
		channel,//通道索引
		Mat(), //不使用掩膜
		dstHist,//输出的目标直方图
		2,//需要计算的直方图的维度
		histSize,//存放每个维度的直方图尺寸的数组
		ranges,//每一维数组的取值范围数组
		true,//指示直方图是否均匀的标示符， TRUE表示均匀的直方图
		false );//累计标示符，false表示直方图在配置阶段会被清零

	//绘制直方图准备参数
	double maxValue = 0;
	minMaxIdx(dstHist, 0, &maxValue, 0, 0);//查找数组和子数组的全局最小值和最大值存入maxValue中 
	int scale = 10; 
	Mat histImg = Mat::zeros(saturationBinNum*scale, hueBinNum*10, CV_8UC3);

	//双层循环，进行直方图
	for (int  hue = 0; hue < hueBinNum; hue++)
		for (int saturation = 0; saturation < saturationBinNum; saturation++)
		{
			float binValue = dstHist.at<float>(hue,saturation);//直方图直条的值
			int intensity = cvRound(binValue*255/maxValue);//强度

			//正式进行绘制
			rectangle(histImg, Point(hue*scale, saturation*scale), Point((hue+1)*scale -1, (saturation+1)*scale-1), Scalar::all(intensity), FILLED);
		}

	//显示效果图

	imshow(WINDOWS_NAME1, srcImage);
	imshow(WINDOWS_NAME2, histImg);

	waitKey(0);



    return 0;
}

//-----------------------------------【ShowHelpText( )函数】-----------------------------
//		 描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t非常感谢购买《OpenCV3编程入门》一书！\n");
	printf("\n\n\t\t\t此为本书OpenCV2版的第79个配套示例程序\n");
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");
}
