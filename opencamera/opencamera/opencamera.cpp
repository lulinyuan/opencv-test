// opencamera.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
	VideoCapture capture(0);//������ͷ������Ƶ
	Mat edges,dst;

	//ѭ����ʾÿһ֡
	while(1)
	{
		//��ȡͼ��
		Mat frame;
		capture >> frame;//��ȡ��ǰ֡
		imshow("read the video",frame);

		cvtColor(frame, edges, CV_BGR2GRAY);

		blur(edges, edges, Size(7, 7));

		Canny( edges, dst, 0, 30, 3);

		imshow("cannnyed picture", dst);

		waitKey(30);//��ʱ30ms

	}
    return 0;
}


