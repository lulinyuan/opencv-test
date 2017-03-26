// harrisconerdetect.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>

#define WINDOW_NAME1 "����ɫ�ǵ�ͼ��"
#define WINDOW_NAME2 "���ǵ���ͼ��"
#define WINDOW_NAME3 "��ԭʼͼ��"

using namespace cv;
//ȫ�ֱ�������
Mat g_srcImage, g_srcImage1, g_grayImage;
int thresh = 10;
int Max_thresh = 175;

//ȫ�ֺ�������
void on_CornerHarris( int, void*);
static void ShowHelpText();





int main()
{	
	//�ı�ϵͳ�����������ɫ�������������Ϣ
	system("color 5F");
	ShowHelpText();

	//������ʾ����
	namedWindow( WINDOW_NAME1, WINDOW_AUTOSIZE );
	namedWindow( WINDOW_NAME2, WINDOW_AUTOSIZE );
	namedWindow(WINDOW_NAME3, WINDOW_AUTOSIZE);

	//����ԭʼͼ�񲢽��п�¡
	g_srcImage = imread("1.jpg", 1);
	if ( !g_srcImage.data)
	{
		printf("cannot find the picture ,Plese check! \n");
		return false;
	}

	g_srcImage1=g_srcImage.clone();
	imshow(WINDOW_NAME3, g_srcImage);
	
	//ת��Ϊ�Ҷ�ͼ
	cvtColor(g_srcImage1, g_grayImage, COLOR_BGR2GRAY);



	//��������������һ�γ�ʼ��
	createTrackbar( "��ֵ", WINDOW_NAME1, &thresh, Max_thresh, on_CornerHarris );

	//����һ�λص���������ʼ��
	on_CornerHarris( 0, 0);

	waitKey(0);

    return 0;
}

//--------------��on_CornerHarris()������----------
void on_CornerHarris( int, void* ) 
{
	//����ֲ�����
	Mat dstImage;//Ŀ��ͼ
	Mat normImage;//��һ�����ͼ
	Mat scaledImage;//���Ի��İ�λ�޷���λ���͵�ͼ

	//��ʼ��
	dstImage = Mat::zeros( g_srcImage.size(), CV_32FC1 );
	g_srcImage1 = g_srcImage.clone();
	
	//���нǵ���
	cornerHarris( g_grayImage, dstImage, 2, 3, 0.04, BORDER_DEFAULT );

	//��һ������
	normalize( dstImage, normImage, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
	convertScaleAbs(normImage, scaledImage);//����һ�����ͼ�����Ա任��8λ�޷���λ����

	//���л���
	//����⵽������ֵ�Ľǵ���Ƴ���
	for (int j = 0; j < normImage.rows; j++)
	{
		for (int i = 0; i < normImage.cols; i++)
		{
			if ((int) normImage.at<float>( j, i ) > thresh+80)
			{
				circle(g_srcImage1, Point(i, j), 5, Scalar(10, 10, 255), 2, 8, 0);
				circle(scaledImage, Point(i, j), 5, Scalar(0, 10, 255), 2, 8, 0);
			}
			
		}
	}

	//��ʾЧ��ͼ
	imshow( WINDOW_NAME1, g_srcImage1);
	imshow( WINDOW_NAME2, scaledImage);

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
	printf("\n\n\n\t����ӭ����Harris�ǵ���ʾ������~��\n\n");
	printf("\n\t������������۲�ͼ��Ч��~\n\n");
}

