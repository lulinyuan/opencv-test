// Multi channel image mixing.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//定义全局函数
bool MultiChannelBlending();

int main()
{
	system("color 9F");

	if (MultiChannelBlending())
	{
		cout << endl << "\n The program is running Successfully !" ;
	}
	waitKey(0);

    return 0;
}

//分离相应通道函数
bool MultiChannelBlending()
{
	//定义相关变量
	Mat srcImage;
	Mat logoImage;
	vector<Mat>channels;
	Mat imageBlueChannel;
	Mat	imageGreenChannel;
	Mat imageRedChannel;

	//操作蓝色通道变量
	logoImage = imread("1_logo.jpg",0);
	 srcImage = imread("2_jugg.jpg");

	 //处理找不到相应文件
	 if (!logoImage.data)
	 {
		 cout << endl << "cannot find the 1_logo.jpg plese check! \n";
		 return false;
	 }
	 if (!srcImage.data)
	 {
		 cout << endl << "cannot find the 1_logo.jpg plese check! \n";
		 return false;
	 }
	
	//把三通道彩色图像变成三个单通道图像
	split(srcImage, channels);//分离色彩通道
	imageBlueChannel = channels.at(0);//将原图的蓝色通道值引用返回给imageBlueChennal;
	//将得到的蓝色通道图的与原图进行混合、
	addWeighted( imageBlueChannel(Rect(500,250,logoImage.cols,logoImage.rows)), 1.0,
		logoImage,0.5, 0., imageBlueChannel(Rect(500,250,logoImage.cols,logoImage.rows)));

	//将图像合并
	merge(channels,srcImage);
	//显示窗口
	namedWindow("<1>orignal picture + logo bule channel ");
	imshow("<1>orignal picture + logo bule channel " ,srcImage );

	//操作绿色通道变量
	logoImage = imread("1_logo.jpg", 0);
	srcImage = imread("2_jugg.jpg");

	//把三通道彩色图像变成三个单通道图像
	split(srcImage, channels);//分离色彩通道
	imageGreenChannel = channels.at(1);//将原图的绿色通道值引用返回给imageBlueChennal;
									  //将得到的绿色通道图的与原图进行混合、
	addWeighted(imageGreenChannel(Rect(500, 250, logoImage.cols, logoImage.rows)), 1.0,
		logoImage, 0.5, 0., imageGreenChannel(Rect(500, 250, logoImage.cols, logoImage.rows)));

	//将图像合并
	merge(channels, srcImage);
	//显示窗口
	namedWindow("<2>orignal picture + logo Green channel ");
	imshow("<2>orignal picture + logo Green channel ", srcImage);


	//操作红色通道变量
	logoImage = imread("1_logo.jpg", 0);
	srcImage = imread("2_jugg.jpg");

	//把三通道彩色图像变成三个单通道图像
	split(srcImage, channels);//分离色彩通道
	imageRedChannel = channels.at(2);//将原图的红色通道值引用返回给imageBlueChennal;
									  //将得到的红色通道图的与原图进行混合、
	addWeighted(imageRedChannel(Rect(500, 250, logoImage.cols, logoImage.rows)), 1.0,
		logoImage, 0.5, 0., imageRedChannel(Rect(500, 250, logoImage.cols, logoImage.rows)));

	//将图像合并
	merge(channels, srcImage);
	//显示窗口
	namedWindow("<3>orignal picture + logo Red channel ");
	imshow("<3>orignal picture + logo Red channel ", srcImage);

	return true;

}
