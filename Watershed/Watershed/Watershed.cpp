// Watershed.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

#define WINDOW_NAME1 "【程序窗口】"
#define WINDOW_NAME2 "【分水岭算法效果图】"


using namespace cv;
using namespace std;

//-------------全局变量声明部分---------
Mat g_maskImage, g_srcImage;
Point prevPt(-1, 1);


//--------【全局函数声明部分】--------
static void ShowHelpText();
static void on_Mouse( int event, int x, int y, int flags, void*);







int main(int argc, char** argv )
{
	//改变界面颜色和输出帮助信息
	system("color 5F");
	ShowHelpText();

	//载入原图  灰度图 显示
	g_srcImage = imread("1.jpg", 1);
	imshow(WINDOW_NAME1, g_srcImage);
	Mat srcImage, grayImage;
	g_srcImage.copyTo(srcImage);

	cvtColor(g_srcImage, g_maskImage, COLOR_BGR2GRAY);
	cvtColor(g_maskImage, grayImage, COLOR_GRAY2BGR);
	g_maskImage = Scalar::all(0);


	//设置鼠标回调函数
	setMouseCallback(WINDOW_NAME1, on_Mouse, 0);

	//轮询键值
	while (1)
	{
		//获取键值
		int c = waitKey(0);
		//按键为ESC时，退出
		if ((char)c == 27)
		{
			break;
		}

		//按键为2时回复原图
		if ((char)c == '2')
		{
			g_maskImage = Scalar::all(0);
			srcImage.copyTo(g_srcImage);
			imshow("image", g_srcImage);

		}
		
		//按键为1或者空格时对图像进行处理
		if ((char)c == '1' || (char)c == ' ')
		{
			//定义一些参数
			int i, j, compCount = 0;
			vector<vector<Point>>g_vContours;
			vector<Vec4i>g_vHierarchy;

			//寻找轮廓
			findContours( g_maskImage, g_vContours, g_vHierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE );
			//轮廓为空时的处理
			if (g_vContours.empty())
				continue;

			//复制掩膜
			Mat maskImage(g_maskImage.size(), CV_32S);
			maskImage = Scalar::all(0);


			//循环绘制轮廓
			for (int  index = 0; index >= 0; index = g_vHierarchy[index][0],compCount++ )
				drawContours(maskImage, g_vContours, index, Scalar::all(compCount + 1), -1, 8, g_vHierarchy, INT_MAX);
			//compount为零时的处理
			if ( compCount == 0 )
					continue;

			//生成随机颜色
			vector<Vec3b>colorTab;
			for ( i = 0; i < compCount; i++)
			{
				int b = theRNG().uniform(0, 255);
				int g = theRNG().uniform(0, 255);
				int r = theRNG().uniform(0, 255);

				colorTab.push_back(Vec3b( (uchar)b, (uchar)g, (uchar)r));
			}

			//计算处理时间并输出到窗口中
			double dTime = (double)getTickCount();
			watershed(srcImage, maskImage);
			dTime = (double)getTickCount() - dTime;
			printf("\t处理时间 = %gms\n", dTime*1000. / getTickFrequency());
			
			
			//双层循环，将分水岭图像遍历存入watershedImage中
			Mat watershedImage(maskImage.size(), CV_8UC3);
			for ( i = 0; i < maskImage.rows; i++)
				for ( j = 0; j < maskImage.cols; j++)
				{
					int index = maskImage.at<int>(i, j);
					if (index == -1)
						watershedImage.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
					else if( index<=0 || index > compCount )
						watershedImage.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
					else
						watershedImage.at<Vec3b>(i, j) = colorTab[index-1];
				}
				//混合灰度图和分水岭效果图并显示到最终窗口
				watershedImage = watershedImage*0.5 + grayImage*0.5;
				imshow(WINDOW_NAME2,watershedImage );	
		}
		
	}

    return 0;
}

//-----------【on_Mouse函数】--------------
static void on_Mouse(int event, int x, int y, int flags, void*)
{
	//处理窗口不在窗口中的情况
	if (x <0 || x >= g_srcImage.cols || y < 0 || y >= g_srcImage.rows)
	{
		return;
	}

	//处理鼠标左键相关消息
	if (event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON))
		prevPt = Point(-1, -1);
	else if (event == EVENT_LBUTTONDOWN)
		prevPt = Point(x, y);
	//鼠标左键按下并移动，绘制出白色线条
	else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))
	{
		Point pt(x, y);
		if (prevPt.x < 0)
			prevPt = pt;
		line(g_maskImage, prevPt, pt, Scalar::all(255), 5, 8, 0);
		line(g_srcImage, prevPt, pt, Scalar::all(255), 5, 8, 0);
		prevPt = pt;
		imshow(WINDOW_NAME1, g_srcImage);
		
	}
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
	printf("\n\n\n\t欢迎来到【分水岭算法】示例程序~\n\n");
	printf("\t请先用鼠标在图片窗口中标记出大致的区域，\n\n\t然后再按键【1】或者【SPACE】启动算法。"
		"\n\n\t按键操作说明: \n\n"
		"\t\t键盘按键【1】或者【SPACE】- 运行的分水岭分割算法\n"
		"\t\t键盘按键【2】- 恢复原始图片\n"
		"\t\t键盘按键【ESC】- 退出程序\n\n\n");
}
