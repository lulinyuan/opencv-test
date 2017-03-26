// SurDetecter.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

#define WINDOW_NAME1 "【原始图1】"
#define WINDOW_NAME2 "【原始图2】"
#define WINDOW_NAME3 "【匹配效果图】"


//定义全局函数
static void ShowHelpText();



int main()
{
	//改变系统输出界面和输出辅助信息
	system("color 5F");
	ShowHelpText();


	Mat g_srcImage1, g_srcImage2;
	g_srcImage1 = imread("1.jpg", 0);
	g_srcImage2 = imread("2.jpg", 0);

	//创建显示窗口
	namedWindow(WINDOW_NAME1, WINDOW_AUTOSIZE);
	namedWindow(WINDOW_NAME2, WINDOW_AUTOSIZE);
	namedWindow(WINDOW_NAME3, WINDOW_AUTOSIZE);

	//显示两张原图
	imshow(WINDOW_NAME1, g_srcImage1);
	imshow(WINDOW_NAME2, g_srcImage2);

	//首先对两幅图进行特征点的检测
	//参数准备
	Mat descriImage1, descriImage2;
	BFMatcher matcher;
	vector<DMatch>matches;

	vector<KeyPoint>keyPoint1;
	vector<KeyPoint>keyPoint2;
	Ptr<SURF>surf;
	surf = SURF::create(400); //2000为检测算子的
	//利用得到特征点计算特征描述子
	//目的：对得到的每个特征点进行描述，整合到Mat类型矩阵中（计算结果是Mat类型的）
	surf -> detectAndCompute(g_srcImage1, Mat(), keyPoint1, descriImage1);
	surf -> detectAndCompute(g_srcImage2, Mat(), keyPoint2, descriImage2);


	//正式开始在两侧图像中进行匹配
	matcher.match(descriImage1, descriImage2, matches);
	//绘制匹配点
	//Mat outimg;
	//drawMatches(g_srcImage1, keyPoint1, g_srcImage2, keyPoint2, matches, outimg, Scalar(0,255,0), Scalar(0,255,0), Mat(), 2);
	
	
	
	
	//筛选匹配点
	sort(matches.begin(), matches.end());
	vector<DMatch>good_matches;
	int ptsPairs = std::min(50, (int)(matches.size() * 0.15));
	cout << ptsPairs << endl;
	for (int i = 0; i < ptsPairs; i++)
	{
		good_matches.push_back(matches[i]);
	}
	//绘制匹配点
	Mat outimg;
	drawMatches(g_srcImage1, keyPoint1, g_srcImage2, keyPoint2, good_matches, outimg, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	
	vector<Point2f>obj;
	vector<Point2f>scene;


	for (size_t i = 0; i < good_matches.size(); i++)
	{
		obj.push_back(keyPoint1[good_matches[i].queryIdx].pt);
		scene.push_back(keyPoint2[good_matches[i].trainIdx].pt);
	}
	vector<Point2f>obj_corners(4);
	obj_corners[0] = Point(0, 0);
	obj_corners[1] = Point(g_srcImage1.cols, 0);
	obj_corners[2] = Point(g_srcImage1.cols, g_srcImage1.rows);
	obj_corners[3] = Point(0, g_srcImage1.rows);
	vector<Point2f>scene_corners(4);

	Mat H = findHomography(obj, scene, RANSAC);
	perspectiveTransform( obj_corners, scene_corners, H);

	//绘制匹配直线
	line(outimg, scene_corners[0] + Point2f((float)g_srcImage1.cols, 0), scene_corners[1] + Point2f((float)g_srcImage1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);
	line(outimg, scene_corners[1] + Point2f((float)g_srcImage1.cols, 0), scene_corners[2] + Point2f((float)g_srcImage1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);
	line(outimg, scene_corners[2] + Point2f((float)g_srcImage1.cols, 0), scene_corners[3] + Point2f((float)g_srcImage1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);
	line(outimg, scene_corners[3] + Point2f((float)g_srcImage1.cols, 0), scene_corners[0] + Point2f((float)g_srcImage1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);


	imshow(WINDOW_NAME3, outimg);


	waitKey(0);

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
		"\t\t键盘按键任意键- 退出程序\n\n");

}