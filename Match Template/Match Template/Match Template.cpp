// Match Template.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//---------�궨��-------
#define WINDOWS_NAME1 "��ԭʼͼƬ��"
#define WINDOWS_NAME2 "��Ч��ͼƬ��"

//-----ȫ�ֱ�������---------
Mat g_srcImage, g_templateImage, g_resultImage;

int g_nMatchMethod;
int g_nMaxTrackbarNum = 5;


//-------ȫ�ֺ�������-----
void on_Matching( int, void* );
static void ShowHelpText();



int main()
{
	//�ı�������ɫ�����������Ϣ
	system("color 5F");
	ShowHelpText();

	//����ͼ���ģ��
	g_srcImage = imread("1.jpg", 1);
	g_templateImage = imread("2.jpg", 1);

	//������ʾ����
	namedWindow( WINDOWS_NAME1, WINDOW_AUTOSIZE);
	namedWindow( WINDOWS_NAME2, WINDOW_AUTOSIZE);


	//����������������һ�γ�ʼ��
	createTrackbar("����", WINDOWS_NAME1,&g_nMatchMethod, g_nMaxTrackbarNum,on_Matching);

	on_Matching(0,0);

	waitKey(0);

    return 0;
}

//--------��on_Matching()������----------
void on_Matching( int, void*)
{
	//���ֲ���������
	Mat srcImage;
	g_srcImage.copyTo( srcImage );

	//��ʼ�����ڽ������ľ���
	int resultImage_cols = g_srcImage.cols - g_templateImage.cols + 1;
	int resultImage_rows = g_srcImage.rows - g_templateImage.rows + 1;
	g_resultImage.create(resultImage_rows, resultImage_cols, CV_32FC1);

	//����ģ��ƥ��ͱ�׼��
	matchTemplate( g_srcImage, g_templateImage, g_resultImage, g_nMatchMethod );
	normalize( g_resultImage, g_resultImage, 0, 1, NORM_MINMAX, -1, Mat() );

	//ͨ������minMaxLoc��λ��ƥ���λ��
	double minValue, maxValue;
	Point minLocation, maxLocation;
	Point matchLocation;
	minMaxLoc( g_resultImage, &minValue, &maxValue, &minLocation, &maxLocation, Mat());

	//���ڷ���SQDIFE��SQDIFE_NORMED,ԽС����ֵ�������Ÿ��ߵ�ƥ������������ķ�������ֵԽ��ƥ��Ч��Խ��
	if ( g_nMatchMethod == TM_SQDIFF || g_nMatchMethod == TM_SQDIFF_NORMED )
	{
		matchLocation = minLocation;
	}
	else
	{
		matchLocation = maxLocation;
	}
	
	//���ƴ����Σ�����ʾ�����ս��
	rectangle(srcImage, matchLocation, Point(matchLocation.x + g_templateImage.cols, matchLocation.y + g_templateImage.rows ), Scalar(0,0,255), 2, 8, 0);
	rectangle( g_resultImage, matchLocation, Point( matchLocation.x + g_templateImage.cols, matchLocation.y + g_templateImage.rows), Scalar(0,0,255),2, 8, 0);

	imshow( WINDOWS_NAME1, srcImage);
	imshow( WINDOWS_NAME2, g_resultImage);

}     


//-----------------------------------��ShowHelpText( )������----------------------------------
//          ���������һЩ������Ϣ
//----------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//�����ӭ��Ϣ��OpenCV�汾
	printf("\n\n\t\t\t   ��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");
	//���һЩ������Ϣ
	printf("\t��ӭ������ģ��ƥ�䡿ʾ������~\n");
	printf("\n\n\t������������۲�ͼ��Ч��\n\n");
	printf("\n\t��������Ӧ�ķ�����ֵ˵��: \n\n"
		"\t\t������0��- ƽ����ƥ�䷨(SQDIFF)\n"
		"\t\t������1��- ��һ��ƽ����ƥ�䷨(SQDIFF NORMED)\n"
		"\t\t������2��- ���ƥ�䷨(TM CCORR)\n"
		"\t\t������3��- ��һ�����ƥ�䷨(TM CCORR NORMED)\n"
		"\t\t������4��- ���ϵ��ƥ�䷨(TM COEFF)\n"
		"\t\t������5��- ��һ�����ϵ��ƥ�䷨(TM COEFF NORMED)\n");
}

