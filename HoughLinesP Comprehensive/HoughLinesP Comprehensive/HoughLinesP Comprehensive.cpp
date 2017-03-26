// HoughLinesP Comprehensive.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


//-------------ȫ�ֱ�������---------
Mat g_srcImage, g_dstImage, g_tmpImage;
vector<Vec4i> g_lines;//����һ��ʸ���ṹg_lines���ڴ�ŵõ����߶�ʸ������
int g_nthreshold = 100;//�������յ�TrackBarλ�ò���


//------------ȫ�ֺ�������-------------
static void on_HoughLines(int, void*);




int main()
{
	//�ı�ϵͳ��ɫ
	system("color 2F");

	//����ԭʼͼ����ʾ
	Mat g_srcImage = imread("1.jpg");
	imshow("��ԭͼ��", g_srcImage);

	//����������
	namedWindow("��Ч��ͼ��", 1);
	createTrackbar("ֵ", "��Ч��ͼ��", &g_nthreshold, 200, on_HoughLines);

	//���б�Ե����ת��Ϊ�Ҷ�ͼ
	Canny(g_srcImage, g_tmpImage, 50, 200, 3);//����һ��Canny���
	cvtColor(g_tmpImage, g_dstImage, COLOR_GRAY2BGR);//ת����Ե�����ͼΪ�Ҷ�ͼ

	//����һ�λص�����������һ��HoughLinesP����
	on_HoughLines( g_nthreshold, 0 );
	HoughLinesP(g_tmpImage, g_lines, 1, CV_PI/180, 80, 50, 10);

	//��ʾЧ��ͼ
	imshow("��Ч��ͼ��", g_dstImage);


	waitKey(0);
    return 0;
}


//---------��on_HoughLines������----------
//��ñ���㡢��ñ���� ���ڻص�����
static void on_HoughLines(int, void*) 
{
	//�þֲ������洢ȫ�ֱ���
	Mat dstImage = g_dstImage.clone();
	Mat tmpImage = g_tmpImage.clone();


	//����HoughLinesP����
	vector<Vec4i>mylines;
	HoughLinesP(tmpImage, mylines, 1, CV_PI/180, g_nthreshold + 1, 50, 10);

	//ѭ������ÿһ���߶�
	for (size_t i = 0; i < mylines.size(); i++)
	{
		Vec4i l = mylines[i];
		line(dstImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(23, 180, 55), 1, CV_AA);

	}
	//��ʾͼ��
	imshow("��Ч��ͼ��", dstImage);

}
