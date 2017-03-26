// Primary image mixing.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


//����ȫ�ֺ���
bool ROI_AddImage();
bool LinearBlending();
bool ROI_LinearBlending();


int main()
{
	system( "color 5F");
	if (ROI_AddImage() && LinearBlending() && ROI_LinearBlending())
	{
		cout << endl << "Success";
	}
	waitKey(0);

    return 0;
}

//ͼ���и���Ȥ�������
bool ROI_AddImage()
{
	//����ͼ��
	Mat srcImage1 = imread("1_pa.jpg");
	Mat logoImage = imread("2_logo.jpg");
	if (!srcImage1.data)
	{
		printf("read 1_pa.jpg   failed -�� \n");
		return false;
	}
	if (!srcImage1.data)
	{
		printf("read 2_logo.jpg   failed -�� \n");
		return false;
	}
	//����һ��Mat���Ͳ��������趨ROI����
	Mat imageROT = srcImage1(Rect(250, 250, logoImage.cols, logoImage.rows));

	//������Ĥ������Ϊ�Ҷ�ͼ��
	Mat mask = imread("2_logo.jpg", 0);

	//����Ĥ���Ƶ�ROI
	logoImage.copyTo( imageROT, mask);

	//��ʾ���
	namedWindow("<1>����ROIʵ��ͼ�����ʾ������");
	imshow("<1>����ROIʵ��ͼ�����ʾ������", srcImage1);

}
//����cv����addWeighted��������ʵ��ͼ�����Ի��
bool LinearBlending()
{
	//����һЩ�ֲ�����
	double alphaValue = 0.5;
	double betaValue;
	Mat srcImage2, srcImage3, dstImage;

	//��ȡͼ������ͼƬ��ҪΪͬ�������ͺͳߴ磩
	srcImage2 = imread( "mogu.jpg");
	srcImage3 = imread( "rain.jpg");
	if (!srcImage2.data)
	{
		printf("read the mogu.jpg   failed -�� \n");
		return false;
	}
	if (!srcImage3.data)
	{
		printf("read the rain.jpg   failed -�� \n");
		return false;
	}
	//����ͼ���ϼ�Ȩ����
	betaValue = (1.0 - alphaValue);
	addWeighted( srcImage2, alphaValue, srcImage3, betaValue, 0.0, dstImage );

	//��������ʾԭͼ����
	namedWindow("<2>���Ի�ϴ��ڡ�ԭͼ��", 1);
	imshow( "<2>���Ի�ϴ��ڡ�ԭͼ��",srcImage2);

	namedWindow("<3>���Ի�ϴ��ڡ�Ч��ͼ��", 1);
	imshow("<3>���Ի�ϴ��ڡ�Ч��ͼ��", dstImage);

	return true;
	
}
//���Ի��ʵ�ֺ�����ָ����������ͼ���ϣ�����cv����addWighted����������϶���
bool ROI_LinearBlending()
{
	//��ȡͼ��
	Mat srcImage4 = imread( "1_pa.jpg",1);
	Mat logoImage = imread("2_logo.jpg", 1);

	if (!srcImage4.data)
	{
		printf("read 1_pa.jpg   failed -�� \n");
		return false;
	}
	if (!logoImage.data)
	{
		printf("read 2_logo.jpg   failed -�� \n");
		return false;
	}
	//����һ��Mat���Ͳ��������趨ROI����

	//����һ
	//Mat imageROT = srcImage4(Rect(250, 250, logoImage.cols, logoImage.rows));

	//������
	Mat	imageROT = srcImage4(Range(250, 250 + logoImage.rows), Range(200, 200 + logoImage.cols));


	//����ͼ���Ի��
	addWeighted( imageROT, 0.5, logoImage, 0.3, 0., imageROT);

	//��ʾ���
	namedWindow("<4>����ROIʵ��ͼ�����ʾ������");
	imshow("<4>����ROIʵ��ͼ�����ʾ������", srcImage4);
}
