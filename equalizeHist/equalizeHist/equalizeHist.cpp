// equalizeHist.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>

using namespace cv;




int main()
{
	//����Դͼ
	Mat dstImage;
	Mat srcImage = imread("1.jpg");
	if (!srcImage.data)
	{
		printf("cannot find the file! Plese check!\n");
	}
	imshow("ԭʼ�Ĳ�ɫͼ", srcImage);
	 
	//ת��Ϊ�Ҷ�ͼ����ʾ
	cvtColor( srcImage, srcImage, COLOR_BGR2GRAY);
	imshow("ԭʼͼ", srcImage);

	//����ֱ��ͼ����
	equalizeHist( srcImage, dstImage);

	imshow("����ֱ��ͼ������ͼ��", dstImage);


	waitKey();
	
    return 0;
}

