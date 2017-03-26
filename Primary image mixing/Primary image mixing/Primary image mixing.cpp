// Primary image mixing.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


//定义全局函数
bool ROI_AddImage();
bool LinearBlending();
bool ROI_LinearBlending();


int main()
{
	system( "color 5F");
	if (ROI_AddImage() && LinearBlending() && ROI_LinearBlending())
	{
		cout << endl << "Success";
	}
	waitKey(0);

    return 0;
}

//图像中感兴趣区域叠加
bool ROI_AddImage()
{
	//读入图像
	Mat srcImage1 = imread("1_pa.jpg");
	Mat logoImage = imread("2_logo.jpg");
	if (!srcImage1.data)
	{
		printf("read 1_pa.jpg   failed -！ \n");
		return false;
	}
	if (!srcImage1.data)
	{
		printf("read 2_logo.jpg   failed -！ \n");
		return false;
	}
	//定义一个Mat类型并给定其设定ROI区域
	Mat imageROT = srcImage1(Rect(250, 250, logoImage.cols, logoImage.rows));

	//加载掩膜（必须为灰度图）
	Mat mask = imread("2_logo.jpg", 0);

	//将掩膜复制到ROI
	logoImage.copyTo( imageROT, mask);

	//显示结果
	namedWindow("<1>利用ROI实现图像叠加示例窗口");
	imshow("<1>利用ROI实现图像叠加示例窗口", srcImage1);

}
//利用cv：：addWeighted（）函数实现图像线性混合
bool LinearBlending()
{
	//定义一些局部变量
	double alphaValue = 0.5;
	double betaValue;
	Mat srcImage2, srcImage3, dstImage;

	//读取图像（两幅图片需要为同样的类型和尺寸）
	srcImage2 = imread( "mogu.jpg");
	srcImage3 = imread( "rain.jpg");
	if (!srcImage2.data)
	{
		printf("read the mogu.jpg   failed -！ \n");
		return false;
	}
	if (!srcImage3.data)
	{
		printf("read the rain.jpg   failed -！ \n");
		return false;
	}
	//进行图像混合加权操作
	betaValue = (1.0 - alphaValue);
	addWeighted( srcImage2, alphaValue, srcImage3, betaValue, 0.0, dstImage );

	//创建并显示原图窗口
	namedWindow("<2>线性混合窗口【原图】", 1);
	imshow( "<2>线性混合窗口【原图】",srcImage2);

	namedWindow("<3>线性混合窗口【效果图】", 1);
	imshow("<3>线性混合窗口【效果图】", dstImage);

	return true;
	
}
//线性混合实现函数，指定区域线性图像混合，利用cv：：addWighted（）函数结合定义
bool ROI_LinearBlending()
{
	//读取图像
	Mat srcImage4 = imread( "1_pa.jpg",1);
	Mat logoImage = imread("2_logo.jpg", 1);

	if (!srcImage4.data)
	{
		printf("read 1_pa.jpg   failed -！ \n");
		return false;
	}
	if (!logoImage.data)
	{
		printf("read 2_logo.jpg   failed -！ \n");
		return false;
	}
	//定义一个Mat类型并给定其设定ROI区域

	//方法一
	//Mat imageROT = srcImage4(Rect(250, 250, logoImage.cols, logoImage.rows));

	//方法二
	Mat	imageROT = srcImage4(Range(250, 250 + logoImage.rows), Range(200, 200 + logoImage.cols));


	//将两图线性混合
	addWeighted( imageROT, 0.5, logoImage, 0.3, 0., imageROT);

	//显示结果
	namedWindow("<4>利用ROI实现图像叠加示例窗口");
	imshow("<4>利用ROI实现图像叠加示例窗口", srcImage4);
}
