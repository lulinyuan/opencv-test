// EdgeDetection.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//-----------ȫ�ֱ�������
//ԭͼ���Ҷ�ͼ��Ŀ��ͼ
Mat g_srcImage, g_grayImage, g_dstImage;

//Canny��Ե�����ر���
Mat g_cannyDetectedEdges;
int g_cannyLowThreshold = 1;//Trackerλ�ò���

//Sobel��Ե�����ر���
Mat g_sobelGradient_X, g_sobelGradient_Y;
Mat g_sobelAbsGradient_X, g_sobelAbsGradient_Y;
int g_sobelKernelSize = 1;

//Scharr�˲�����ر���
Mat g_scharrGradient_X, g_scharrGradient_Y;
Mat g_scharrAbsGradient_X, g_scharrAbsGradient_Y;

//-------------ȫ�ֺ�����������
static void ShowHelpText();
static void on_Canny(int, void*);//Canny��Ե��ⴰ�ڻ������Ļص�����
static void on_Sobel(int, void*);//Sobel��Ե��ⴰ�ڻ������Ļص�����
void Scharr();//��װ��Scharr��Ե������ش��뺯��



int main()
{
	//ϵͳ��ɫ����ʾϵͳ��ɫ
	system("color 5F");
	ShowHelpText();

	//����Դͼ 
	g_srcImage = imread("1.jpg");
	if (!g_srcImage.data)
	{
		printf("cannot read the ppicture !plese check!\n");
		return false;
	}
	//��ʾԭʼͼ
	namedWindow("��ԭʼͼ��");
	imshow("��ԭʼͼ��", g_srcImage);

	//������srcͬ���ͺʹ�С�ľ���dst
	g_dstImage.create( g_srcImage.size(), g_srcImage.type() );

	//��ԭͼת��Ϊ�Ҷ�ͼ
	cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);

	//������ʾ����
	namedWindow("Canny detection", WINDOW_AUTOSIZE);
	namedWindow("Sobel detection", WINDOW_AUTOSIZE);
	//����Trackbar
	createTrackbar("����ֵ", "Canny detection", &g_cannyLowThreshold, 120, on_Canny);
	createTrackbar("����ֵ", "Sobel detection", &g_sobelKernelSize, 3, on_Sobel);

	//���ûص�����
	on_Canny(0, 0);
	on_Sobel(0, 0);

	//���÷�װScharr��Ե������ĺ���
	Scharr();

	//��ѯ��ȡ������Ϣ�� �����¡�q�����˳�

	while ((char(waitKey(1)) != 'q'))
	{

	}
    return 0;
}
//-----------------------------------��ShowHelpText( )������----------------------------------
//		���������һЩ������Ϣ
//----------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//�����ӭ��Ϣ��OpenCV�汾
	printf("\n\n\t\t\t   ��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");

	//���һЩ������Ϣ
	printf("\n\n\t���гɹ���������������۲�ͼ��Ч��~\n\n"
		"\t���¡�q����ʱ�������˳���\n");
}
//-----------------------------��on_Canny()������-----------------------------------
void on_Canny(int, void*)
{
	//��ʹ��3*3�ں�������
	blur(g_grayImage, g_cannyDetectedEdges, Size(3, 3));

	//����Canny����
	Canny( g_cannyDetectedEdges, g_cannyDetectedEdges, g_cannyLowThreshold, g_cannyLowThreshold * 3, 3);

	//��g_dstImage��Ԫ������
	g_dstImage = Scalar::all(0);

	//ʹ��Canny��������ı�Եͼg_cannyDetectedEdages��Ϊ���룬����ԭͼg_srcImage������g_dstImage  
	g_srcImage.copyTo(g_dstImage, g_cannyDetectedEdges);
	//��ʾЧ��ͼ
	imshow("Canny detection", g_dstImage);
}
//--------------------------��on_Sobel������-----------------------

void on_Sobel(int, void*)
{
	//��X������ݶ�
	Sobel(g_srcImage, g_sobelGradient_X, CV_16S, 1, 0, (2 * g_sobelKernelSize + 1), 1, 1, BORDER_DEFAULT);

	//�������ֵ�������ͽ��ת����8λ
	convertScaleAbs(g_sobelGradient_X, g_sobelAbsGradient_X);

	//���Y������ݶ�
	Sobel(g_srcImage, g_sobelGradient_Y, CV_16S, 0, 1, (2 * g_sobelKernelSize + 1), 1, 1, BORDER_DEFAULT);
	//�������ֵ�������ͽ��ת����8λ
	convertScaleAbs(g_sobelGradient_Y, g_sobelAbsGradient_Y);

	//�ϲ��ݶ�
	addWeighted(g_sobelAbsGradient_X, 0.5, g_sobelAbsGradient_Y, 0.5, 0, g_dstImage);

	//��ʾЧ��ͼ
	imshow("Sobel detection",g_dstImage);

}

//----------------��Scharr()������-------------
void Scharr() 
{
	//��X������ݶ�
	Scharr(g_srcImage, g_scharrGradient_X, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);

	//�������ֵ�������ͽ��ת����8λ
	convertScaleAbs(g_scharrGradient_X, g_scharrAbsGradient_X);

	//���Y������ݶ�
	Scharr(g_srcImage, g_scharrGradient_Y, CV_16S, 0, 1, 1, 0, BORDER_DEFAULT);
	//�������ֵ�������ͽ��ת����8λ
	convertScaleAbs(g_scharrGradient_Y, g_scharrAbsGradient_Y);

	//�ϲ��ݶ�
	addWeighted(g_scharrAbsGradient_X, 0.5, g_scharrAbsGradient_Y, 0.5, 0, g_dstImage);

	//��ʾЧ��ͼ
	imshow("Scharr Detection", g_dstImage);

}