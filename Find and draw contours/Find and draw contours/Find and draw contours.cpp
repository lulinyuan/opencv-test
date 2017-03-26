// Find and draw contours.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//----�궨�岿��---
#define NAME_WINDOWS1 "��ԭʼͼ��"
#define NAME_WINDOWS2 "������ͼ��"
#define NAME_WINDOWS3 "����ɫͼ��"



//---------ȫ�ֱ�������-----------
Mat g_srcImage, g_grayImage;
int g_nThresh = 80;
int g_nThresh_Max = 255;
RNG g_rng(12345);
Mat g_cannyMat_output;
vector<vector<Point>>g_vContours;
vector<Vec4i>g_vHierarchy;

//-------ȫ�ֺ���������---------
static void ShowHelpText();
void on_ThreshChange( int, void*);



int main( int argc, char** argv)
{
	//�ı�������ɫ�����������Ϣ
	system("color 2F");
	ShowHelpText();

	//����ͼ��ת��Ϊ�Ҷ�ͼ��ʾ
	g_srcImage = imread("1.jpg",1);
	if (!g_srcImage.data)
	{
		printf("cannot find the file! Plese check the file\n");
	}
	imshow(NAME_WINDOWS3, g_srcImage);

	cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
	blur(g_grayImage, g_grayImage, Size(3,3));
	imshow(NAME_WINDOWS1, g_grayImage);

	//��������������ʼ��
	createTrackbar("canny��ֵ", NAME_WINDOWS1, &g_nThresh, g_nThresh_Max, on_ThreshChange);
	on_ThreshChange(0, 0);

	waitKey(0);
    return 0;
}

//----------��on_ThreshChange������--------------
void on_ThreshChange(int, void*)
{
	//��canny���Ӽ���Ե
	Canny( g_grayImage, g_cannyMat_output, g_nThresh, g_nThresh*2, 3);

	//Ѱ������
	findContours( g_cannyMat_output, g_vContours, g_vHierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

	//�������
	Mat drawing = Mat::zeros(g_cannyMat_output.size(), CV_8UC3);

	for (int i = 0; i < g_vContours.size(); i++)
	{
		Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
		drawContours( drawing, g_vContours, i, color, 2, 8, g_vHierarchy, 0, Point() );

		//��ʾЧ��ͼ
		imshow( NAME_WINDOWS2, drawing);



	}

}
//-----------------------------------��ShowHelpText( )������----------------------------------  
//      ���������һЩ������Ϣ  
//----------------------------------------------------------------------------------------------  
static void ShowHelpText()
{
	//�����ӭ��Ϣ��OpenCV�汾
	printf("\n\n\t\t\t�ǳ���л����OpenCV3������š�һ�飡\n");
	printf("\n\n\t\t\t��Ϊ����OpenCV2��ĵ�70������ʾ������\n");
	printf("\n\n\t\t\t   ��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");

	//���һЩ������Ϣ  
	printf("\n\n\t��ӭ��������ͼ����Ѱ��������ʾ������~\n\n");
	printf("\n\n\t��������˵��: \n\n"
		"\t\t���̰��������- �˳�����\n\n"
		"\t\t����������-�ı���ֵ\n");
}