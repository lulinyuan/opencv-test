// impaint.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define WINDOWS_NAME0 "【原图参考图】"
#define WINDOWS_NAME1 "【原始图】"
#define WINDOWS_NAME2 "【修补后的效果图】"

//-----------【全局变量】----------
Mat srcImage0, srcImage1, inpaintMask;
Point previousPoint(-1,-1);//原来的点坐标
//------全局函数部分
static void ShowHelpText();
static void On_Mouse(int event, int x, int y, int flags, void* );



//------------【On_Mouse函数】-------


int main()
{
	//改变界面颜色输出帮助信息
	system("color 5F");
	ShowHelpText();

	//载入原图并进行掩膜初始化
	 Mat srcImage = imread("1.jpg",-1);
	if (!srcImage.data)
	{
		printf("cannot find the file! Plese check it \n");
		return false;
	}
	srcImage0 = srcImage.clone();
	srcImage1 = srcImage.clone();
	inpaintMask = Mat::zeros(srcImage1.size(), CV_8U);

	//显示原始图
	imshow(WINDOWS_NAME0, srcImage0);
	imshow(WINDOWS_NAME1, srcImage1);

	//设置鼠标回调消息
	setMouseCallback(WINDOWS_NAME1, On_Mouse, 0);

	//轮询按键， 根据不同按键进行处理
	while (1)
	{
		//获取鼠标键值
		char key = waitKey(0);
		//键值为ESC，退出
		if (key == 27)
			break;

		//键值为2 恢复显示原图
		if ( key == '2')
		{
			inpaintMask = Scalar::all(0);
			srcImage.copyTo(srcImage1);
			imshow( WINDOWS_NAME1, srcImage1);
		}

		//键值为1或者space时 处理图像
		if (key == '1' || key == ' ')
		{
			Mat inpaintedImage;
			inpaint(srcImage, inpaintMask, inpaintedImage, 3, INPAINT_TELEA);
			imshow(WINDOWS_NAME2, inpaintedImage);

		}

	}



    return 0;
}

//--------【On_Mouse()函数】------------
void On_Mouse(int event, int x, int y, int flags, void*)
{
	//鼠标左键弹起消息
	if (event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON))
		previousPoint = Point(-1, -1);
	else if (event == EVENT_LBUTTONDOWN)
		previousPoint = Point(x, y);
	else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))
	{
		Point pt(x,y);
		if (previousPoint.x < 0)
			previousPoint = pt;
		//绘制白色线条
		line( inpaintMask, previousPoint, pt, Scalar::all(255), 5, 8, 0);
		line( srcImage1, previousPoint, pt, Scalar::all(255), 5, 8, 0);
		previousPoint = pt;
		imshow(WINDOWS_NAME1, srcImage1);

	}

}
//-----------------------------------【ShowHelpText( )函数】----------------------------------
//          描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");

	//输出一些帮助信息
	printf("\n\n\n\t欢迎来到【图像修复】示例程序~\n");
	printf("\n\t请在进行图像修复操作之前，在【原始图】窗口中进行适量的绘制"
		"\n\n\t按键操作说明: \n\n"
		"\t\t【鼠标左键】-在图像上绘制白色线条\n\n"
		"\t\t键盘按键【ESC】- 退出程序\n\n"
		"\t\t键盘按键【1】或【SPACE】-进行图像修复操作 \n\n");
}

