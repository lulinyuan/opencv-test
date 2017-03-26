// opencamera.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
	VideoCapture capture(0);//从摄像头读入视频
	Mat edges,dst;

	//循环显示每一帧
	while(1)
	{
		//读取图像
		Mat frame;
		capture >> frame;//读取当前帧
		imshow("read the video",frame);

		cvtColor(frame, edges, CV_BGR2GRAY);

		blur(edges, edges, Size(7, 7));

		Canny( edges, dst, 0, 30, 3);

		imshow("cannnyed picture", dst);

		waitKey(30);//延时30ms

	}
    return 0;
}


