// ORB_detect.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

using namespace cv;
using namespace std;
using namespace xfeatures2d;

//宏定义
#define WINDOW_NAME1 "【原始图】"
#define WINDOW_NAME2 "【匹配窗口】"



//全局函数声明
static void ShowHelpText();


int main()
{
	//改变输出界面颜色
	system("color 5F");
	ShowHelpText();
	namedWindow(WINDOW_NAME1, WINDOW_AUTOSIZE);
	namedWindow(WINDOW_NAME2, WINDOW_AUTOSIZE);


	//载入原图并转化为灰度图
	Mat srcImage = imread("1.jpg", 1);
	imshow(WINDOW_NAME1, srcImage);
	Mat grayImage;
	cvtColor(srcImage, grayImage, CV_RGB2GRAY);

	//检测SIFT特征点并在图像中提取物体的描述符
	//参数定义
	Ptr<ORB> orb;
	orb = ORB::create();
	vector<KeyPoint>keyPoints;
	Mat descriptors;
	orb -> detectAndCompute(grayImage, Mat(), keyPoints, descriptors);

	//基于FLANN的描述符对象匹配
	flann::Index flannIndex( descriptors, cv::flann::LshIndexParams(12, 20, 2),cvflann::FLANN_DIST_HAMMING );


	//初始化视频采集对象
	VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 360);//设置采集视频的宽度
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 900);//设置采集视频的高度

	unsigned int frameCount = 0;

	while (1)
	{
		double time0 = static_cast<double>(getTickCount());//记录开始的时间

		Mat  captureImage, captureImage_gray;//定义两个Mat变量，用于视频采集
		cap >> captureImage;//采集视频帧
		if (captureImage.empty())
			continue;
		cvtColor(captureImage, captureImage_gray, CV_RGB2GRAY);


		//检测SIFT关键点并提取测试图像中描述符
		vector<KeyPoint> captureKeyPoints;
		Mat captureDescription;
		//调用函数检测
		orb -> detectAndCompute(captureImage, Mat(), captureKeyPoints, captureDescription);

		//匹配和测试描述符，获取两个最邻近的描述符
		Mat matchIndex(captureDescription.rows, 2, CV_32SC1);
		Mat matchDistance(captureDescription.rows, 2, CV_32FC1);

		flannIndex.knnSearch(captureDescription, matchIndex, matchDistance, 2, flann::SearchParams());//调用K邻近算法

		//根据劳式算法选出最优的匹配
		vector<DMatch> goodMatches;
		for (int  i = 0; i < matchDistance.rows; i++)
		{
			if (matchDistance.at<float>(i,0) <0.6 * matchDistance.at<float>(i,1))
			{
				DMatch dmatches(i, matchIndex.at<int>(i, 0), matchIndex.at<float>(i, 1));
				goodMatches.push_back(dmatches);
			}
		}

		//绘制并显示匹配窗口
		Mat resultImage;
		drawMatches(captureImage, captureKeyPoints, srcImage, keyPoints, goodMatches, resultImage);
		imshow(WINDOW_NAME2, resultImage);

		//显示帧率
		cout << "帧率= " << getTickFrequency() / (getTickCount() - time0) << endl;

		//按下ESC键退出
		if (char(waitKey(1) == 27))break;


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
		"\t\t键盘按键ESC- 退出程序\n\n");

}