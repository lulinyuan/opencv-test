// ORB_detect.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

using namespace cv;
using namespace std;
using namespace xfeatures2d;

//�궨��
#define WINDOW_NAME1 "��ԭʼͼ��"
#define WINDOW_NAME2 "��ƥ�䴰�ڡ�"



//ȫ�ֺ�������
static void ShowHelpText();


int main()
{
	//�ı����������ɫ
	system("color 5F");
	ShowHelpText();
	namedWindow(WINDOW_NAME1, WINDOW_AUTOSIZE);
	namedWindow(WINDOW_NAME2, WINDOW_AUTOSIZE);


	//����ԭͼ��ת��Ϊ�Ҷ�ͼ
	Mat srcImage = imread("1.jpg", 1);
	imshow(WINDOW_NAME1, srcImage);
	Mat grayImage;
	cvtColor(srcImage, grayImage, CV_RGB2GRAY);

	//���SIFT�����㲢��ͼ������ȡ�����������
	//��������
	Ptr<ORB> orb;
	orb = ORB::create();
	vector<KeyPoint>keyPoints;
	Mat descriptors;
	orb -> detectAndCompute(grayImage, Mat(), keyPoints, descriptors);

	//����FLANN������������ƥ��
	flann::Index flannIndex( descriptors, cv::flann::LshIndexParams(12, 20, 2),cvflann::FLANN_DIST_HAMMING );


	//��ʼ����Ƶ�ɼ�����
	VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 360);//���òɼ���Ƶ�Ŀ��
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 900);//���òɼ���Ƶ�ĸ߶�

	unsigned int frameCount = 0;

	while (1)
	{
		double time0 = static_cast<double>(getTickCount());//��¼��ʼ��ʱ��

		Mat  captureImage, captureImage_gray;//��������Mat������������Ƶ�ɼ�
		cap >> captureImage;//�ɼ���Ƶ֡
		if (captureImage.empty())
			continue;
		cvtColor(captureImage, captureImage_gray, CV_RGB2GRAY);


		//���SIFT�ؼ��㲢��ȡ����ͼ����������
		vector<KeyPoint> captureKeyPoints;
		Mat captureDescription;
		//���ú������
		orb -> detectAndCompute(captureImage, Mat(), captureKeyPoints, captureDescription);

		//ƥ��Ͳ�������������ȡ�������ڽ���������
		Mat matchIndex(captureDescription.rows, 2, CV_32SC1);
		Mat matchDistance(captureDescription.rows, 2, CV_32FC1);

		flannIndex.knnSearch(captureDescription, matchIndex, matchDistance, 2, flann::SearchParams());//����K�ڽ��㷨

		//������ʽ�㷨ѡ�����ŵ�ƥ��
		vector<DMatch> goodMatches;
		for (int  i = 0; i < matchDistance.rows; i++)
		{
			if (matchDistance.at<float>(i,0) <0.6 * matchDistance.at<float>(i,1))
			{
				DMatch dmatches(i, matchIndex.at<int>(i, 0), matchIndex.at<float>(i, 1));
				goodMatches.push_back(dmatches);
			}
		}

		//���Ʋ���ʾƥ�䴰��
		Mat resultImage;
		drawMatches(captureImage, captureKeyPoints, srcImage, keyPoints, goodMatches, resultImage);
		imshow(WINDOW_NAME2, resultImage);

		//��ʾ֡��
		cout << "֡��= " << getTickFrequency() / (getTickCount() - time0) << endl;

		//����ESC���˳�
		if (char(waitKey(1) == 27))break;


	}


    return 0;
}

//-----------------------------------��ShowHelpText( )������----------------------------------
//          ���������һЩ������Ϣ
//----------------------------------------------------------------------------------------------
void ShowHelpText()
{
	//�����ӭ��Ϣ��OpenCV�汾
	printf("\n\n\t\t\t   ��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");
	//���һЩ������Ϣ  
	printf("\n\n\t��ӭ������SURF�������⡿ʾ������\n\n");
	printf("\n\t��������˵��: \n\n"
		"\t\t���̰���ESC- �˳�����\n\n");

}