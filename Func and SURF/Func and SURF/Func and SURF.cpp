// Func and SURF.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace std;
using namespace xfeatures2d;

#define WINDOW_NAME1 "【原始匹配图】"
#define WINDOW_NAME2 "【匹配图】"



//全局函数声明
static void ShowHelpText();



int main()
{
	//改变界面的字体颜色
	system("color 5F");
	ShowHelpText();


	//创建显示窗口
	namedWindow(WINDOW_NAME1, WINDOW_AUTOSIZE);
	namedWindow(WINDOW_NAME2, WINDOW_AUTOSIZE);

	//读取图像并转化为灰度图
	Mat trainImage = imread("1.jpg",1);
	imshow(WINDOW_NAME1, trainImage);
	Mat trainImage_gray;
	cvtColor(trainImage, trainImage_gray, COLOR_BGR2GRAY);

	//检测Surf关键点、提取训练图像描述
	Ptr<SURF> surf;
	surf = SURF::create(80);
	vector<KeyPoint> train_keyPoint;
	Mat trainDescriptor;

	surf->detectAndCompute(trainImage_gray, Mat(), train_keyPoint, trainDescriptor );

	//创建基于FLANN的描述匹配对象
	FlannBasedMatcher matcher;
	vector<Mat>train_desc_collection(1, trainDescriptor);
	matcher.add(train_desc_collection);
	matcher.train();


	//创建视频对象、定义帧率
	VideoCapture cap(0);
	unsigned int frameCount = 0;//帧数

	//不断循环，直到q键被按下
	while ( char (waitKey(1)) != 'q')
	{
		//参数设定
		int64 time0 = getTickCount();

		Mat testImage, testImage_gray;
		cap >> testImage;//采集视频到testImage中
		if ( testImage.empty() )
		{
			continue;
		}

		//转化为灰度图
		cvtColor( testImage, testImage_gray, COLOR_BGR2GRAY );

		//检测S关键点、提取测试图像的描述符
		vector<KeyPoint>test_keyPoint;
		Mat testDescriptor;
		surf->detectAndCompute(testImage_gray, Mat(), test_keyPoint, testDescriptor);

		//匹配训练和测试描述符
		vector<vector<DMatch>>matches;
		matcher.knnMatch(testDescriptor, matches, 2);

		//根据劳式算法（Lowe's algorithm）,得到优秀的匹配点
		vector<DMatch>goodMatches;
		for (unsigned int  i = 0; i < matches.size(); i++)
		{
			if (matches[i][0].distance < 0.6 * matches[i][1].distance)
				goodMatches.push_back(matches[i][0]);
		}

		//绘制匹配窗口并显示结果
		Mat dstImage;
		drawMatches(testImage, test_keyPoint, trainImage, train_keyPoint, goodMatches, dstImage);

		imshow(WINDOW_NAME2, dstImage);


		//输出帧率信息
		cout << "当前帧率：" << getTickFrequency() / (getTickCount() - time0) << endl;

	}

	return 0;
}

//-----------------------------------【ShowHelpText( )函数】----------------------------------
//          描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");
	//输出一些帮助信息  
	printf("\n\n\t欢迎来到【SURF特征点检测】示例程序\n\n");
	printf("\n\t按键操作说明: \n\n"
		"\t\t键盘按键Q- 退出程序\n\n");

}