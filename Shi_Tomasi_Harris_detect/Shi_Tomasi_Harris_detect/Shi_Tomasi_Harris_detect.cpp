// Shi_Tomasi_Harris_detect.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

#define WINDOW_NAME1 "��ԭʼͼ��"
#define WINDOW_NAME2 "��Shi-Tomasi�ǵ��⡿"

using namespace cv;
using namespace std;




//ȫ�ֱ�������
Mat g_srcImage, g_grayImage;
int g_maxCornerNumber = 10;
int g_maxTrackbarNumber= 500;
RNG g_rng(12345);



//ȫ�ֺ�������
static void ShowHelpText();
void on_goodFeaturesToTrack_Demo( int, void* );


int main()
{
	//�ı�ϵͳ�������ɫ���������Ϣ
	system("color 5F");
	ShowHelpText();


	//������ʾ���ںͻ���������������ʾ�ͻص�������ʼ��
	namedWindow(WINDOW_NAME1, WINDOW_AUTOSIZE);
	namedWindow(WINDOW_NAME2, WINDOW_AUTOSIZE);

	//����ԭͼ��ת��Ϊ�Ҷ�ͼ
	g_srcImage = imread( "1.jpg", 1 );
	if (!g_srcImage.data )
	{
		printf("cannot find the file, Plese check! ");
		return false;
	}
	imshow(WINDOW_NAME1, g_srcImage);

	cvtColor( g_srcImage, g_grayImage, COLOR_BGR2GRAY );
	


	createTrackbar("���ǵ���", WINDOW_NAME2, &g_maxCornerNumber, g_maxTrackbarNumber, on_goodFeaturesToTrack_Demo );

	on_goodFeaturesToTrack_Demo( 0, 0);



	waitKey(0);

    return 0;
}

//------------��on_goodFeaturesToTrack_Demo()������-----------
void on_goodFeaturesToTrack_Demo(int, void*) 
{
	//���ڱ���С��1ʱ�Ĵ���
	if ( g_maxCornerNumber <= 1)
	{
		g_maxCornerNumber = 1;
	}

	//Shi-Tomasi�㷨��goodFeatureToTrack�������Ĳ���׼��
	vector<Point2f> corner;
	double qualityLevel = 0.01;//�ǵ���ɽ��ܵ���С����ֵ
	double minDistance = 10;//�ǵ�����С����
	int blockSize = 3;//���㵽��������ؾ���ʱָ��������Χ
	double k = 0.04;//Ȩ��ϵ��
	Mat copy = g_srcImage.clone();//����ԭͼ��һ����ʱ�����У���Ϊ����Ȥ����

	//����Shi-Tomasi�ǵ���
	goodFeaturesToTrack( g_grayImage, corner, g_maxCornerNumber, qualityLevel, minDistance, Mat(), blockSize, false, k );

	//����ı���Ϣ
	cout << ">�˴μ�⵽�Ľ�������Ϊ" << corner.size() << endl;


	//���Ƽ�⵽�Ľǵ�
	int r = 4;
	for (unsigned int  i = 0; i < corner.size() ; i++)
	{
		//���Ƶ�
		circle(copy, corner[i], r, Scalar(10, 10, 255), -1, 8, 0);
	}

	//��ʾͼ��
	imshow(WINDOW_NAME2, copy);

	//�����ؽǵ���Ĳ����趨
	Size winSize = Size(5, 5);
	Size zeroZone = Size(-1, -1);

	TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 40, 0.001);

	//����������ؽ���λ��
	cornerSubPix(g_grayImage, corner, winSize, zeroZone, criteria);

	//����ǵ���Ϣ
	for (int i = 0; i < corner.size(); i++)
	{	
		cout << " \t��ȷ������[" << i << "](" << corner[i].x << "," << corner[i].y << ")" << endl;

	}


}
//-----------------------------------��ShowHelpText( )������----------------------------------
//          ���������һЩ������Ϣ
//----------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//�����ӭ��Ϣ��OpenCV�汾
	printf("\n\n\t\t\t�ǳ���л����OpenCV3������š�һ�飡\n");
	printf("\n\n\t\t\t��Ϊ����OpenCV2��ĵ�87������ʾ������\n");
	printf("\n\n\t\t\t   ��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");
	//���һЩ������Ϣ
	printf("\n\n\n\t��ӭ������Shi-Tomasi�ǵ��⡿ʾ������\n");
	printf("\n\t������������۲�ͼ��Ч��\n\n");

}
