// HoughCircles.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main()
{

	//����Դͼ
	Mat srcImage = imread("1.jpg");

	//��ʾԭͼ
	imshow("ԭʼͼ", srcImage);

	//����һЩ����
	Mat tmpImage, dstImage;

	//ԭͼת��Ϊ�Ҷ�ͼ
	cvtColor( srcImage, tmpImage, COLOR_BGR2GRAY);
	//��˹�˲�ƽ������
	GaussianBlur(tmpImage, tmpImage, Size(9, 9), 2, 2);


	//���л���Բ�任
	vector<Vec3f>circles;
	HoughCircles(tmpImage, circles, HOUGH_GRADIENT, 1.5, 10, 200, 100, 0, 0);


	//������ͼ�л�Բ
	for (size_t i = 0; i < circles.size(); i++)
	{
		//��������
		Point center( cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);

		cout << "Բ������:(" << center<< ")\n";

		//����Բ��Բ��
		circle(srcImage, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		//����Բ������
		circle(srcImage, center, radius, Scalar(155, 50, 255), 3, 8, 0);


	}

	//��ʾЧ��ͼ
	imshow("Ч��ͼ", srcImage);

	waitKey(0);



    return 0;
}

