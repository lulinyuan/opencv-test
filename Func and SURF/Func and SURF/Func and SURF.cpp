// Func and SURF.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace std;
using namespace xfeatures2d;

#define WINDOW_NAME1 "��ԭʼƥ��ͼ��"
#define WINDOW_NAME2 "��ƥ��ͼ��"



//ȫ�ֺ�������
static void ShowHelpText();



int main()
{
	//�ı�����������ɫ
	system("color 5F");
	ShowHelpText();


	//������ʾ����
	namedWindow(WINDOW_NAME1, WINDOW_AUTOSIZE);
	namedWindow(WINDOW_NAME2, WINDOW_AUTOSIZE);

	//��ȡͼ��ת��Ϊ�Ҷ�ͼ
	Mat trainImage = imread("1.jpg",1);
	imshow(WINDOW_NAME1, trainImage);
	Mat trainImage_gray;
	cvtColor(trainImage, trainImage_gray, COLOR_BGR2GRAY);

	//���Surf�ؼ��㡢��ȡѵ��ͼ������
	Ptr<SURF> surf;
	surf = SURF::create(80);
	vector<KeyPoint> train_keyPoint;
	Mat trainDescriptor;

	surf->detectAndCompute(trainImage_gray, Mat(), train_keyPoint, trainDescriptor );

	//��������FLANN������ƥ�����
	FlannBasedMatcher matcher;
	vector<Mat>train_desc_collection(1, trainDescriptor);
	matcher.add(train_desc_collection);
	matcher.train();


	//������Ƶ���󡢶���֡��
	VideoCapture cap(0);
	unsigned int frameCount = 0;//֡��

	//����ѭ����ֱ��q��������
	while ( char (waitKey(1)) != 'q')
	{
		//�����趨
		int64 time0 = getTickCount();

		Mat testImage, testImage_gray;
		cap >> testImage;//�ɼ���Ƶ��testImage��
		if ( testImage.empty() )
		{
			continue;
		}

		//ת��Ϊ�Ҷ�ͼ
		cvtColor( testImage, testImage_gray, COLOR_BGR2GRAY );

		//���S�ؼ��㡢��ȡ����ͼ���������
		vector<KeyPoint>test_keyPoint;
		Mat testDescriptor;
		surf->detectAndCompute(testImage_gray, Mat(), test_keyPoint, testDescriptor);

		//ƥ��ѵ���Ͳ���������
		vector<vector<DMatch>>matches;
		matcher.knnMatch(testDescriptor, matches, 2);

		//������ʽ�㷨��Lowe's algorithm��,�õ������ƥ���
		vector<DMatch>goodMatches;
		for (unsigned int  i = 0; i < matches.size(); i++)
		{
			if (matches[i][0].distance < 0.6 * matches[i][1].distance)
				goodMatches.push_back(matches[i][0]);
		}

		//����ƥ�䴰�ڲ���ʾ���
		Mat dstImage;
		drawMatches(testImage, test_keyPoint, trainImage, train_keyPoint, goodMatches, dstImage);

		imshow(WINDOW_NAME2, dstImage);


		//���֡����Ϣ
		cout << "��ǰ֡�ʣ�" << getTickFrequency() / (getTickCount() - time0) << endl;

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
		"\t\t���̰���Q- �˳�����\n\n");

}