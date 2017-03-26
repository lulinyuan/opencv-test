// Reflection projection.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>


using namespace cv;
using namespace std;

#define WINDOWS_NAME "��ԭʼͼ��"
#define WINDOWS_NAME1 "��H-Sֱ��ͼ��"
#define WINDOWS_NAME2 "������ͶӰͼ��"

//------ȫ�ֱ�������--------
Mat g_srcImage, g_hsvImage, g_hueImage;
int g_bins = 30;


//----��ȫ�ֺ����������֡�-------
static void ShowHelpText();
void on_BinChange(int, void*);


int main()
{
	//�ı�����������ɫ�����������Ϣ
	system("color 5F");
	ShowHelpText();

	//����ͼ�� ת��ΪHSV�ռ�
	g_srcImage = imread("1.jpg",1);
	if (!g_srcImage.data)
	{
		printf("cannot find the file��Plese check it!\n");
		return false;
	}
	cvtColor( g_srcImage, g_hsvImage, CV_BGR2HSV);


	//����hueɫ��ͨ��
	g_hueImage.create( g_hsvImage.size(),g_hsvImage.depth());
	int ch[] = { 0,0 };
	mixChannels(&g_hsvImage, 1, &g_hueImage, 1, ch, 1);

	//����Trackbar ������bin����Ŀ
	namedWindow(WINDOWS_NAME,WINDOW_AUTOSIZE );
	createTrackbar( "ɫ�����", WINDOWS_NAME, &g_bins, 180, on_BinChange);

	on_BinChange( 0, 0);


	//��ʾЧ��ͼ
	imshow(WINDOWS_NAME, g_srcImage );

	waitKey(0);


    return 0;
}

//---------------��on_BinChange()������
void on_BinChange( int, void*)
{
	//����׼��
	MatND hist;
	int histSize = MAX( g_bins, 2);
	float hue_range[] = { 0, 180 };
	const float* ranges = { hue_range };
	//����ֱ��ͼ����һ��
	calcHist( &g_hueImage, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false );
	normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	//���㷴��ͶӰ
	MatND backproj;
	calcBackProject( &g_hueImage, 1, 0, hist, backproj, &ranges, 1, true );

	//��ʾ����ͶӰ
	imshow(WINDOWS_NAME1, backproj);

	//����ֱ��ͼ�Ĳ���׼��
	int w = 400, h = 400;
	int bin_w = cvRound((double) w / histSize);
	Mat histImg = Mat::zeros( w, h, CV_8UC3);

	//����ֱ��ͼ
	for (int  i = 0; i < g_bins; i++)
	{
		rectangle(histImg, Point(i*bin_w, h), Point((i + 1)*bin_w, h - cvRound(hist.at<float>(i)*h / 255.0)), Scalar(100, 123, 255), -1);
	}


	//��ʾֱ��ͼ����
	imshow(WINDOWS_NAME2, histImg );

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
	printf("\n\n\t��ӭ����������ͶӰ��ʾ������\n\n");
	printf("\n\t������������۲�ͼ��Ч��\n\n");

}

