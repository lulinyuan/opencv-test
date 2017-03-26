// HoughCircles.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main()
{

	//载入源图
	Mat srcImage = imread("1.jpg");

	//显示原图
	imshow("原始图", srcImage);

	//定义一些变量
	Mat tmpImage, dstImage;

	//原图转化为灰度图
	cvtColor( srcImage, tmpImage, COLOR_BGR2GRAY);
	//高斯滤波平滑处理
	GaussianBlur(tmpImage, tmpImage, Size(9, 9), 2, 2);


	//进行霍夫圆变换
	vector<Vec3f>circles;
	HoughCircles(tmpImage, circles, HOUGH_GRADIENT, 1.5, 10, 200, 100, 0, 0);


	//依次在图中花圆
	for (size_t i = 0; i < circles.size(); i++)
	{
		//参数定义
		Point center( cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);

		cout << "圆心坐标:(" << center<< ")\n";

		//绘制圆的圆心
		circle(srcImage, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		//绘制圆的轮廓
		circle(srcImage, center, radius, Scalar(155, 50, 255), 3, 8, 0);


	}

	//显示效果图
	imshow("效果图", srcImage);

	waitKey(0);



    return 0;
}

