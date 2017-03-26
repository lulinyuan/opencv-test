// Convexhull.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//----------�궨��----------
#define WINDOWS_NAME1 "����ɫԭͼ��"
#define WINDOWS_NAME2 "���Ҷ�ͼ��"
#define WINDOWS_NAME3 "��Ч��ͼ��"

//-------ȫ�ֱ�������----------
Mat g_srcImage, g_grayImage;
int g_nThresh = 50, g_nThresh_Max = 255;
RNG g_rng(12345);
Mat srcImage_copy = g_srcImage.clone();
Mat g_threshholdImage_ouput;
vector<vector<Point>>g_vContours;
vector<Vec4i>g_vHierarchy;

//-------ȫ�ֺ�������---------
static void ShowHelpText();
void on_ThreshChange(int, void*);




int main()
{
	//�ı������ɫ�����������Ϣ
	system("color 2F");
	ShowHelpText();
	

	//����ԭͼ���Ҷ�ͼ���������ʾ
	g_srcImage = imread("1.jpg", 1);
	if (!g_srcImage.data)
	{
		printf("cannot find the file ! Plese check the file \n");
		return false;
	}
	imshow(WINDOWS_NAME1, g_srcImage);

	cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
	blur(g_grayImage, g_grayImage, Size(3,3));

	namedWindow(WINDOWS_NAME2, WINDOW_AUTOSIZE);
	imshow(WINDOWS_NAME2, g_grayImage);

	//������ֵ������
	createTrackbar( "��ֵ:", WINDOWS_NAME2, &g_nThresh, g_nThresh_Max, on_ThreshChange);
	on_ThreshChange(0,0);

	waitKey(0);


    return 0;
}
//-------��on_ThreshChange������------
void on_ThreshChange(int, void*) 
{
	//��ͼ����ж�ֵ������ ������ֵ
	threshold( g_grayImage, g_threshholdImage_ouput, g_nThresh, 255, THRESH_BINARY);

	//Ѱ������
	findContours( g_threshholdImage_ouput, g_vContours, g_vHierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0,0));

	//����ÿһ��������Ѱ��͹��
	vector<vector<Point>>hull(g_vContours.size());
	for (unsigned i = 0; i < g_vContours.size(); i++)
	{
		convexHull( Mat(g_vContours[i]), hull[i], false );

	}

	//����������͹��
	Mat drawing = Mat::zeros(g_threshholdImage_ouput.size(), CV_8UC3);
	for (unsigned i = 0; i < g_vContours.size(); i++)
	{
		Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
		drawContours(drawing, g_vContours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		drawContours(drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point());

	}

	//��ʾЧ��ͼ
	imshow(WINDOWS_NAME3, drawing);

}
//-----------------------------------��ShowHelpText( )������-----------------------------
//		 ���������һЩ������Ϣ
//----------------------------------------------------------------------------------------------
void ShowHelpText()
{
	//�����ӭ��Ϣ��OpenCV�汾
	printf("\n\n\t\t\t   ��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");
}


