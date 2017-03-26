// equalizeHist.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>

using namespace cv;




int main()
{
	//载入源图
	Mat dstImage;
	Mat srcImage = imread("1.jpg");
	if (!srcImage.data)
	{
		printf("cannot find the file! Plese check!\n");
	}
	imshow("原始的彩色图", srcImage);
	 
	//转化为灰度图并显示
	cvtColor( srcImage, srcImage, COLOR_BGR2GRAY);
	imshow("原始图", srcImage);

	//进行直方图处理
	equalizeHist( srcImage, dstImage);

	imshow("经过直方图处理后的图像", dstImage);


	waitKey();
	
    return 0;
}

