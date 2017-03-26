// Remap.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

//---------�궨�岿��-----------------
#define WINDOWS_NAME "���򴰿�"

using namespace cv;
using namespace std;

//--------ȫ�ֱ�������-------------
Mat g_srcImage, g_dstImage;
Mat g_map_X, g_map_Y;

//------ȫ�ֺ�����������-----------
int update_map( int key );
static void ShowHelpText();



int main(int argc, char** argv )
{
	//�ı�ϵͳ����ɫ�����������Ϣ
	system("color 2F");
	ShowHelpText();

	//����Դͼ����ʾ
	g_srcImage = imread("1.jpg");
	if ( !g_srcImage.data)
	{
		printf("cannot find the file ! plese check \n");
		return false;
	}
	imshow("ԭʼͼ", g_srcImage);

	//������ԭͼһ����Ч��ͼ��x��ӳ��ͼ��y��ӳ��ͼ
	g_dstImage.create( g_srcImage.size(), g_srcImage.type());
	g_map_X.create(g_srcImage.size(), CV_32FC1);
	g_map_Y.create(g_srcImage.size(), CV_32FC1);

	//�������ڲ���ʾ
	namedWindow(WINDOWS_NAME, WINDOW_AUTOSIZE);
	imshow( WINDOWS_NAME, g_srcImage);

	//��ѯ����������map_X��map_Y��ֵ
	while (1)
	{
		//��ȡ���̰���
		int key = waitKey(0);

		//�ж�ESC�Ƿ��£�
		if ((key & 255) == 27)
		{
			cout << "�����˳�����������������������������\n";
			break;
		}

		//���ݰ��µļ���������map_X��map_Y��ֵ�� Ȼ�����remap()������ӳ��
		update_map(key);
		remap(g_srcImage, g_dstImage, g_map_X, g_map_Y, INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));

		//��ʾЧ��ͼ
		imshow(WINDOWS_NAME, g_dstImage);
	}

    return 0;
}

//---------------��update_map()������---------------------
int update_map(int key)
{
	//˫��ѭ��������ÿһ�����ص�
	for (int j = 0; j < g_srcImage.rows; j++)
	{
		for (int i = 0; i < g_srcImage.cols; i++)
		{
			switch (key)
			{
			case '1'://���е�һ��ӳ��
				if (i > g_srcImage.cols*0.25 && i < g_srcImage.cols*0.75 && j > g_srcImage.rows*0.25 &&j < g_srcImage.rows*0.75)
				{
					g_map_X.at<float>(j, i) = static_cast<float>(2 * (i - g_srcImage.cols*0.25) + 0.5);
					g_map_Y.at<float>(j, i) = static_cast<float>(2 * (j - g_srcImage.rows*0.25) + 0.5);
				}
				else
				{
					g_map_X.at<float>(j, i) = 0;
					g_map_Y.at<float>(j, i) = 0;
				}
				break;
			case '2'://���еڶ���ӳ��
				g_map_X.at<float>(j, i) = static_cast<float>(i);
				g_map_Y.at<float>(j, i) = static_cast<float>(g_srcImage.rows - j);
				break;
			case '3'://���е�������ӳ��
				g_map_X.at<float>(j, i) = static_cast<float>(g_srcImage.cols-i);
				g_map_Y.at<float>(j, i) = static_cast<float>(j);
				break;
			case '4':
				g_map_X.at<float>(j, i) = static_cast<float>(g_srcImage.cols - i);
				g_map_Y.at<float>(j, i) = static_cast<float>(g_srcImage.rows - j);
				break;
			}
		}
	}
	return 1;
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
	printf("\n\t��ӭ������ӳ��ʾ������~\n\n");
	printf("\n\t��������˵��: \n\n"
		"\t\t���̰�����ESC��- �˳�����\n"
		"\t\t���̰�����1��-  ��һ��ӳ�䷽ʽ\n"
		"\t\t���̰�����2��- �ڶ���ӳ�䷽ʽ\n"
		"\t\t���̰�����3��- ������ӳ�䷽ʽ\n"
		"\t\t���̰�����4��- ������ӳ�䷽ʽ\n");
}

