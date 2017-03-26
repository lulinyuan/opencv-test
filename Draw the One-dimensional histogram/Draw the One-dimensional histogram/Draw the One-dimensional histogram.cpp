// Draw the One-dimensional histogram.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define WINDOWS_NAME1 "��ԭʼͼ��"
#define WINDOWS_NAME2 "��һάH-Sֱ��ͼ��"


//----------------��ShowHelpText��-----------
static void ShowHelpText();


int main()
{
	//�ı������ɫ�����������Ϣ
	system("color 5F");
	ShowHelpText();

	Mat srcImage = imread("1.jpg",0 );
	imshow(WINDOWS_NAME1, srcImage);
	if (!srcImage.data)
	{
		printf("cannot find the file, Plese chese it \n");
		return false;
	}

	//�������
	MatND dstHist;//��CV����CvHistogram *Hist = cvCreatHist
	int dims = 1;
	float hranges[] = {0,255};
	const float *ranges[] = { hranges };
	int size = 256;
	int channel = 0;

	//����ͼ���ֱ��ͼ
	calcHist(&srcImage, 1, &channel, Mat(), dstHist, dims, &size, ranges);
	int scale = 1;
	Mat dstImage(size * scale, size, CV_8U, Scalar(0) );


	//��ȡ���ֵ����Сֵ
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc( dstHist, &minValue, &maxValue, 0, 0);//��cv���õ���CVGETMinMaxHistValue

	//���Ƴ�ֱ��ͼ
	int hpt = saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++)
	{
		float binValue = dstHist.at<float>(i);

		int realValue = saturate_cast<int>(binValue*hpt/maxValue);
		rectangle( dstImage, Point(i*scale,size - 1),  Point((i+1)*scale - 1, size - realValue ), Scalar(255));
	}

	//��ʾͼ��
	imshow(WINDOWS_NAME2, dstImage);
	waitKey(0);

    return 0;
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

