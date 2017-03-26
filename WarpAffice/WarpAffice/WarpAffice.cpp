// WarpAffice.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//-----------【宏定义】--------
#define WINDOWS_NAME1 "原始图"
#define WINDOWS_NMAE2 "经过Warp后的图像"
#define WINDOWS_NAME3 "经过Wrap和Rotate后的窗口"

//----------全局函数声明部分
static void ShowHelpText();




int main()
{
	//改变界面颜色和显示帮助信息
	system("color 5F");
	ShowHelpText();

	//定义两组点，代表两个三角形
	Point2f srcTriangel[3];
	Point2f dstTriangel[3];

	//定义Mat变量
	Mat rotMat(2, 3, CV_32FC1);
	Mat warpMat(2, 3, CV_32FC1);
	Mat srcImage, dstImage_warp, dstImage_warp_rotate;


	//加载原图像
	srcImage = imread("1.jpg");
	if (!srcImage.data)
	{
		printf("cannot find the file! Plese check it\n");
		return false;
	}

	//设置目标图像的大小和类型和源图像一致
	dstImage_warp = Mat::zeros( srcImage.rows, srcImage.cols, srcImage.type());

	//设置源图像和目标图像上的三组点及计算放射变换
	srcTriangel[0] = Point2f(0, 0);
	srcTriangel[1] = Point2f(static_cast<float>(srcImage.cols - 1), 0);
	srcTriangel[2] = Point2f(1, static_cast<float>(srcImage.rows - 1));

	dstTriangel[0] = Point(static_cast<float>(srcImage.cols*0.0), static_cast<float>(srcImage.rows*0.33));
	dstTriangel[1] = Point(static_cast<float>(srcImage.cols*0.65), static_cast<float>(srcImage.rows*0.35));
	dstTriangel[2] = Point(static_cast<float>(srcImage.cols*0.15), static_cast<float>(srcImage.rows*0.6));

	//求得仿射变换
	warpMat = getAffineTransform( srcTriangel, dstTriangel);
	//对原图像应用刚刚求得的仿射变换
	warpAffine(srcImage, dstImage_warp, warpMat, dstImage_warp.size());

	//对图像进行缩放后在旋转
	//计算绕图像中点顺时针旋转50度缩放因子为0.6的旋转矩阵
	Point center = Point(dstImage_warp.cols / 2, dstImage_warp.rows / 2);
	double angle = -90.0;
	double scale = 0.8;

	//通过上面的旋转细节信息求旋转矩阵
	rotMat = getRotationMatrix2D(center, angle, scale);

	//旋转已经缩放的图像
	warpAffine(dstImage_warp, dstImage_warp_rotate, rotMat, dstImage_warp.size());

	//显示结果
	imshow(WINDOWS_NAME1,srcImage);
	imshow(WINDOWS_NMAE2, dstImage_warp);
	imshow(WINDOWS_NAME3, dstImage_warp_rotate);

	waitKey(0);

    return 0;
}
static void ShowHelpText()
{

	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");

	//输出一些帮助信息  
	printf("\n\n\t\t欢迎来到仿射变换综合示例程序.\n\n");
	printf("\t\t键盘按键【ESC】- 退出程序\n");
}

