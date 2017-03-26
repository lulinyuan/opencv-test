// Polygon bounding contour.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


//----------�궨�岿��---------
#define WINDOWS_NAME1 "����ɫԭͼ��"
#define WINDOWS_NAME2 "���Ҷ�ͼ��"
#define WINDOWS_NAME3 "��Ч��ͼ��"

//------------ȫ�ֱ�������----------
Mat g_srcImage, g_grayImage;
int g_nThresh = 20, g_nThresh_Max = 255;
RNG g_rng(12345);

//----------ȫ�ֺ�������-----------
static void ShowHelpText();
void on_ContoursChange(int, void*);





int main()
{
	//�ı�������ɫ�����������Ϣ
	system("color 2F");
	ShowHelpText();

	//����ԭͼ ƽ������ ת��Ϊ�Ҷ�ͼ ��ʾ
	g_srcImage = imread("1.jpg");
	if (!g_srcImage.data)
	{
		printf("cannot find the file!Plese check the file\n");
		return false;
	}
	imshow(WINDOWS_NAME1, g_srcImage);


	cvtColor( g_srcImage, g_grayImage, COLOR_BGR2GRAY);
	blur( g_grayImage, g_grayImage, Size(3,3));

	namedWindow(WINDOWS_NAME2, WINDOW_AUTOSIZE);
	imshow(WINDOWS_NAME2, g_grayImage);

	//������ֵ�����������ô�����
	createTrackbar("��ֵ", WINDOWS_NAME2, &g_nThresh, g_nThresh_Max, on_ContoursChange);
	on_ContoursChange(0,0);

	waitKey(0);

    return 0;
}

//----------��on_ContoursChange����������----------
void on_ContoursChange(int, void*) 
{
	//����һЩ����
	Mat threshhold_output;
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;

	//ʹ��Threshold��������Ե
	threshold(g_grayImage, threshhold_output,g_nThresh, 255, THRESH_BINARY);

	//�ҳ�����
	findContours(threshhold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	//����αƽ����� + ��ȡ���κ�Բ�α߽��
	vector<vector<Point>>contours_poly( contours.size());
	vector<Rect>boundRect( contours.size());
	vector<Point2f>center( contours.size());
	vector<float>radius( contours.size());

	//����ͼ������в���
	for (unsigned i = 0; i < contours.size(); i++)
	{
		//��ָ�����ȱƽ����������
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		//����㼯�������棨up-right�����α߽�
		boundRect[i] = boundingRect( Mat(contours_poly[i]));
		//���ڸ���2D�㼯��Ѱ����С����İ�ΧԲ��
		minEnclosingCircle(contours_poly[i], center[i], radius[i] );
	}

	//���ƶ�������� ������Χ�ľ��ο򡡣���Բ�ο�
	Mat drawing = Mat::zeros( threshhold_output.size(), CV_8UC3 );
	for (unsigned i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar( g_rng.uniform(0,255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
		//��������
		drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		//���ƾ��ο�
		rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
		//����Բ�ο�
		circle(drawing, center[i], (int)radius[i], color, 2, 8, 0);


	}

	//��ʾЧ������
	namedWindow( WINDOWS_NAME3, WINDOW_AUTOSIZE);
	imshow( WINDOWS_NAME3, drawing);

}
//-----------------------------------��ShowHelpText( )������----------------------------------  
//      ���������һЩ������Ϣ  
//----------------------------------------------------------------------------------------------  
static void ShowHelpText()
{
	//�����ӭ��Ϣ��OpenCV�汾
	printf("\n\n\t\t\t   ��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");

	//���һЩ������Ϣ  
	printf("\n\n\n\t��ӭ������������Χ�����ľ��κ�Բ�α߽��ʾ������~\n\n");
	printf("\n\n\t��������˵��: \n\n"
		"\t\t���̰�����ESC��- �˳�����\n\n"
		"\t\t���������� - �ı���ֵ\n\n");
}
