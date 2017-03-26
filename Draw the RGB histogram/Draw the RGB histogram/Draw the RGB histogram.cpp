// Draw the RGB histogram.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define WINDOWS_NAME1 "��ԭʼͼ��" 
#define WINDOWS_NAME2 "��RGBֱ��ͼ��" 

//-------��ȫ�ֺ���������---------
static  void  ShowHelpText();

int main()
{
	//�ı������ɫ�����������Ϣ
	system("color 5F");
	ShowHelpText();


	//����ԭͼ ��ʾ
	Mat srcImage = imread("1.jpg",1);
	imshow(WINDOWS_NAME1,srcImage);

	//����׼��
	int bins = 256;
	int hist_size[] = { bins };
	float range[] = { 0, 256 };
	const float* ranges[] = { range };
	MatND redHist, greenHist, blueHist;
	int channels_r[] = { 0 };

	//����ֱ��ͼ�ļ��㣨��ɫ�������֣�
	calcHist(&srcImage, 1, channels_r, Mat(), redHist, 1, hist_size, ranges, true, false);

	//����ֱ��ͼ�ļ��㣨��ɫ�������֣�
	int channels_g[] = { 1 };
	calcHist(&srcImage, 1, channels_g, Mat(), greenHist, 1, hist_size, ranges, true, false);

	//����ֱ��ͼ�ļ��㣨��ɫ�������֣�
	int channels_b[] = { 2 };
	calcHist(&srcImage, 1, channels_b, Mat(), blueHist, 1, hist_size, ranges, true, false);


	//���Ƴ���ɫֱ��ͼ
	//����׼��
	double maxValue_red, maxValue_green, maxValue_blue;
	
	minMaxLoc(redHist, 0, &maxValue_red, 0, 0 );
	minMaxLoc(redHist, 0, &maxValue_green, 0, 0);
	minMaxLoc(redHist, 0, &maxValue_blue, 0, 0);

	int scale = 1;
	int histHeight = 256;
	Mat histImage = Mat::zeros(histHeight, bins*3, CV_8UC3);

	//��ʼ����
	for (int  i = 0; i < bins; i++)
	{
		//����׼��
		float binValue_red = redHist.at<float>(i);
		float binValue_green = greenHist.at<float>(i);
		float binValue_blue = blueHist.at<float>(i);

		int intensity_red = cvRound(binValue_red*histHeight/maxValue_red);//���û��Ƶĸ߶�
		int intensity_green = cvRound(binValue_green*histHeight / maxValue_red);//���û��Ƶĸ߶�
		int intensity_blue = cvRound(binValue_blue*histHeight / maxValue_red);//���û��Ƶĸ߶�


		//���ƺ�ɫ������ֱ��ͼ
		rectangle( histImage, Point(i*scale, histHeight - 1), Point((i + 1)*scale - 1, histHeight - intensity_red),Scalar(255, 0, 0));

		//������ɫ������ֱ��ͼ
		rectangle(histImage, Point((i+bins)*scale, histHeight - 1), Point((i + bins + 1)*scale - 1, histHeight - intensity_green), Scalar(0, 255, 0));

		//������ɫ������ֱ��ͼ
		rectangle(histImage, Point((i + 2*bins)*scale, histHeight - 1), Point((i + 2*bins+ 1)*scale - 1, histHeight - intensity_blue), Scalar(0, 0, 255));

	}
	//��ʾһάRGB��ֱ��ͼ

	imshow(WINDOWS_NAME2, histImage);

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
