// Find and draw the image moment.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv; 
using namespace std;

//----------���궨�岿�֡�----------
#define WINDOWS_NAME1 "��ԭʼ��ɫͼ��"
#define WINDOWS_NAME2 "���Ҷ�ͼ��"
#define WINDOWS_NAME3 "��ͼ��������"

//---------��ȫ�ֱ���������--------
Mat g_srcImage, g_gragImage;
int g_nThresh = 20, g_nThresh_Max = 255;
RNG g_rng(12345);
Mat g_cannny_output;
vector<vector<Point>>g_vContours;
vector<Vec4i>g_vHierarchy;

//--------ȫ�ֺ�������-------
void on_ContoursChange( int, void*);
static void ShowHelpText();



int main()
{
	//�ı������ɫ�����������Ϣ
	system("color 5F");
	ShowHelpText();

	//����ԭͼ ת��Ϊ�Ҷ�ͼ ƽ������ ��ʾ
	g_srcImage = imread("1.jpg",1);
	if (!g_srcImage.data)
	{
		printf("cannot find the file! Plese check the file \n");
		return false;
	}

	imshow(WINDOWS_NAME1,g_srcImage);

	cvtColor(g_srcImage, g_gragImage, COLOR_BGR2GRAY);
	blur( g_gragImage, g_gragImage,Size(3,3));
	
	namedWindow(WINDOWS_NAME2, WINDOW_AUTOSIZE);
	imshow(WINDOWS_NAME2, g_gragImage);

	//������ֵ�Ļ������������г�ʼ��
	createTrackbar( "��ֵ", WINDOWS_NAME2, &g_nThresh, g_nThresh_Max,on_ContoursChange);
	on_ContoursChange(0, 0);


	waitKey(0);



    return 0;
}


//----------��on_ContoursChange������---------------
void on_ContoursChange( int, void*) 
{
	//ʹ��canny����Ե
	Canny(g_gragImage, g_cannny_output,g_nThresh, g_nThresh*2, 3);

	//�ҳ�����
	findContours( g_cannny_output, g_vContours, g_vHierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	//�����
	vector<Moments>mu(g_vContours.size());
	for (unsigned int i = 0; i < g_vContours.size(); i++)
	{
		mu[i] = moments( g_vContours[i], false );

	}

	//�������ľ�
	vector<Point2f>mc(g_vContours.size());
	for (unsigned int i = 0; i < g_vContours.size(); i++)
	{
		mc[i] = Point2f(static_cast<float>(mu[i].m10 / mu[i].m00), static_cast<float>(mu[i].m01 / mu[i].m00));

	}

	//��������
	Mat drawing = Mat::zeros( g_cannny_output.size(), CV_8UC3);
	for (unsigned int  i = 0; i < g_vContours.size(); i++)
	{
		Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
		//���������ڲ�����
		drawContours( drawing, g_vContours, i, color, 2, 8, g_vHierarchy, 0,	Point() );
		//����԰
		circle( drawing, mc[i], 4, color, -1, 8, 0);
	}

	//��ʾ
	namedWindow(WINDOWS_NAME3,WINDOW_AUTOSIZE);
	imshow(WINDOWS_NAME3, drawing);



	//ͨ��m00��������������Һ�OpenCV�����Ա�
	printf("\t ������ݣ��������������");
	for (unsigned int  i = 0; i < g_vContours.size(); i++)
	{
		printf(">ͨ��m00��������[%d]���������M_00��= %.2f \n OpenCV�������������� = %.2f�����ȣ�%.2f \n\n", i, mu[i].m00, 
			contourArea(g_vContours[i]), arcLength( g_vContours[1], true));
		Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
		drawContours(drawing, g_vContours, i, color, 2, 8, g_vHierarchy, 0, Point());
		//����԰
		circle(drawing, mc[i], 4, color, -1, 8, 0);
	}


}

//-----------------------------------��ShowHelpText( )������-----------------------------
//		 ���������һЩ������Ϣ
//----------------------------------------------------------------------------------------------
void ShowHelpText()
{
	//�����ӭ��Ϣ��OpenCV�汾
	printf("\n\n\t\t\t�ǳ���л����OpenCV3������š�һ�飡\n");
	printf("\n\n\t\t\t��Ϊ����OpenCV2��ĵ�76������ʾ������\n");
	printf("\n\n\t\t\t   ��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");
}