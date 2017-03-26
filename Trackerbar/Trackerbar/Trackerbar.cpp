// Trackerbar.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>

using namespace cv;
#define WINDOWS_NAME "linearly blend"    //Ϊ���ڱ��ⶨ���

//***********************ȫ�ֱ�������*****************************
// ������ȫ�ֱ���
//****************************************************************
const int g_nMaxAlphaValue = 100;//Alphaֵ�����ֵ
int	g_nAlphaValueSlider;//��������Ӧ��ֵ
double g_dAlphaValue;
double g_dBetValue;

//�����洢ͼ��ĵı���
Mat g_srcImage1;
Mat g_srcImage2;
Mat g_dstImage;


//on_Trackerbar()����
//��������Ӧ�����Ļص�����


void on_Trackerbar(int, void*)
{
	//�����ǰֵ��������ֵ�ı���
	g_dAlphaValue = (double)g_nAlphaValueSlider/g_nMaxAlphaValue;
	//��BETAֵΪ1��ȥalphaֵ
	g_dBetValue = (1 - g_dAlphaValue);


	//����alpha��BETA��ֵ�������Ի��
	addWeighted(g_srcImage1,g_dAlphaValue,g_srcImage2,g_dBetValue,0.0,g_dstImage);

	std::cout<<"g_dAlphaValue:"<< g_dAlphaValue << "g_dBetValue:" <<g_dBetValue<<std::endl;
	//��ʾЧ��ͼ
	imshow(WINDOWS_NAME,g_dstImage);

}


int main(int argc, char** argv)
{
	//����ͼ������ͼ��ߴ���Ҫ��ͬ��
	g_srcImage1 = imread("1.jpg");
	g_srcImage2 = imread("2.jpg");
	
	if ( !g_srcImage1.data )
	{
		printf("��ȡ��һͼƬʧ�ܣ���ȷ��Ŀ¼���Ƿ�����Ӧ�ļ�����~! \n");
		return -1;
	}
	if (!g_srcImage2.data)
	{
		printf("��ȡ�ڶ�ͼƬʧ�ܣ���ȷ��Ŀ¼���Ƿ�����Ӧ�ļ�����~! \n");
		return -1;
	}

	//���û�����ֵΪ70
	g_nAlphaValueSlider = 70;
	//��������
	namedWindow(WINDOWS_NAME,1);
	//�ڴ����Ĵ����д���һ���������ؼ�
	char TrackbarName[50];
	sprintf( TrackbarName, "Transparency value %d",g_nMaxAlphaValue);

	createTrackbar( TrackbarName,WINDOWS_NAME,&g_nAlphaValueSlider,g_nMaxAlphaValue,on_Trackerbar );

	//����ڻص���������ʾ
	on_Trackerbar( g_nAlphaValueSlider, 0 );


	//�������ֹͣ
	waitKey(0);


    return 0;
}

