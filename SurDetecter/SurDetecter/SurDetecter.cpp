// SurDetecter.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

#define WINDOW_NAME1 "��ԭʼͼ1��"
#define WINDOW_NAME2 "��ԭʼͼ2��"
#define WINDOW_NAME3 "��ƥ��Ч��ͼ��"


//����ȫ�ֺ���
static void ShowHelpText();



int main()
{
	//�ı�ϵͳ�����������������Ϣ
	system("color 5F");
	ShowHelpText();


	Mat g_srcImage1, g_srcImage2;
	g_srcImage1 = imread("1.jpg", 0);
	g_srcImage2 = imread("2.jpg", 0);

	//������ʾ����
	namedWindow(WINDOW_NAME1, WINDOW_AUTOSIZE);
	namedWindow(WINDOW_NAME2, WINDOW_AUTOSIZE);
	namedWindow(WINDOW_NAME3, WINDOW_AUTOSIZE);

	//��ʾ����ԭͼ
	imshow(WINDOW_NAME1, g_srcImage1);
	imshow(WINDOW_NAME2, g_srcImage2);

	//���ȶ�����ͼ����������ļ��
	//����׼��
	Mat descriImage1, descriImage2;
	BFMatcher matcher;
	vector<DMatch>matches;

	vector<KeyPoint>keyPoint1;
	vector<KeyPoint>keyPoint2;
	Ptr<SURF>surf;
	surf = SURF::create(400); //2000Ϊ������ӵ�
	//���õõ��������������������
	//Ŀ�ģ��Եõ���ÿ��������������������ϵ�Mat���;����У���������Mat���͵ģ�
	surf -> detectAndCompute(g_srcImage1, Mat(), keyPoint1, descriImage1);
	surf -> detectAndCompute(g_srcImage2, Mat(), keyPoint2, descriImage2);


	//��ʽ��ʼ������ͼ���н���ƥ��
	matcher.match(descriImage1, descriImage2, matches);
	//����ƥ���
	//Mat outimg;
	//drawMatches(g_srcImage1, keyPoint1, g_srcImage2, keyPoint2, matches, outimg, Scalar(0,255,0), Scalar(0,255,0), Mat(), 2);
	
	
	
	
	//ɸѡƥ���
	sort(matches.begin(), matches.end());
	vector<DMatch>good_matches;
	int ptsPairs = std::min(50, (int)(matches.size() * 0.15));
	cout << ptsPairs << endl;
	for (int i = 0; i < ptsPairs; i++)
	{
		good_matches.push_back(matches[i]);
	}
	//����ƥ���
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

	//����ƥ��ֱ��
	line(outimg, scene_corners[0] + Point2f((float)g_srcImage1.cols, 0), scene_corners[1] + Point2f((float)g_srcImage1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);
	line(outimg, scene_corners[1] + Point2f((float)g_srcImage1.cols, 0), scene_corners[2] + Point2f((float)g_srcImage1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);
	line(outimg, scene_corners[2] + Point2f((float)g_srcImage1.cols, 0), scene_corners[3] + Point2f((float)g_srcImage1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);
	line(outimg, scene_corners[3] + Point2f((float)g_srcImage1.cols, 0), scene_corners[0] + Point2f((float)g_srcImage1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);


	imshow(WINDOW_NAME3, outimg);


	waitKey(0);

    return 0;
}

//-----------------------------------��ShowHelpText( )������----------------------------------
//          ���������һЩ������Ϣ
//----------------------------------------------------------------------------------------------
void ShowHelpText()
{
	//�����ӭ��Ϣ��OpenCV�汾
	printf("\n\n\t\t\t   ��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");
	//���һЩ������Ϣ  
	printf("\n\n\t��ӭ������SURF�������⡿ʾ������\n\n");
	printf("\n\t��������˵��: \n\n"
		"\t\t���̰��������- �˳�����\n\n");

}