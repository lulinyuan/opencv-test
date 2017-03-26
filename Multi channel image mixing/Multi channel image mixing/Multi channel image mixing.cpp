// Multi channel image mixing.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//����ȫ�ֺ���
bool MultiChannelBlending();

int main()
{
	system("color 9F");

	if (MultiChannelBlending())
	{
		cout << endl << "\n The program is running Successfully !" ;
	}
	waitKey(0);

    return 0;
}

//������Ӧͨ������
bool MultiChannelBlending()
{
	//������ر���
	Mat srcImage;
	Mat logoImage;
	vector<Mat>channels;
	Mat imageBlueChannel;
	Mat	imageGreenChannel;
	Mat imageRedChannel;

	//������ɫͨ������
	logoImage = imread("1_logo.jpg",0);
	 srcImage = imread("2_jugg.jpg");

	 //�����Ҳ�����Ӧ�ļ�
	 if (!logoImage.data)
	 {
		 cout << endl << "cannot find the 1_logo.jpg plese check! \n";
		 return false;
	 }
	 if (!srcImage.data)
	 {
		 cout << endl << "cannot find the 1_logo.jpg plese check! \n";
		 return false;
	 }
	
	//����ͨ����ɫͼ����������ͨ��ͼ��
	split(srcImage, channels);//����ɫ��ͨ��
	imageBlueChannel = channels.at(0);//��ԭͼ����ɫͨ��ֵ���÷��ظ�imageBlueChennal;
	//���õ�����ɫͨ��ͼ����ԭͼ���л�ϡ�
	addWeighted( imageBlueChannel(Rect(500,250,logoImage.cols,logoImage.rows)), 1.0,
		logoImage,0.5, 0., imageBlueChannel(Rect(500,250,logoImage.cols,logoImage.rows)));

	//��ͼ��ϲ�
	merge(channels,srcImage);
	//��ʾ����
	namedWindow("<1>orignal picture + logo bule channel ");
	imshow("<1>orignal picture + logo bule channel " ,srcImage );

	//������ɫͨ������
	logoImage = imread("1_logo.jpg", 0);
	srcImage = imread("2_jugg.jpg");

	//����ͨ����ɫͼ����������ͨ��ͼ��
	split(srcImage, channels);//����ɫ��ͨ��
	imageGreenChannel = channels.at(1);//��ԭͼ����ɫͨ��ֵ���÷��ظ�imageBlueChennal;
									  //���õ�����ɫͨ��ͼ����ԭͼ���л�ϡ�
	addWeighted(imageGreenChannel(Rect(500, 250, logoImage.cols, logoImage.rows)), 1.0,
		logoImage, 0.5, 0., imageGreenChannel(Rect(500, 250, logoImage.cols, logoImage.rows)));

	//��ͼ��ϲ�
	merge(channels, srcImage);
	//��ʾ����
	namedWindow("<2>orignal picture + logo Green channel ");
	imshow("<2>orignal picture + logo Green channel ", srcImage);


	//������ɫͨ������
	logoImage = imread("1_logo.jpg", 0);
	srcImage = imread("2_jugg.jpg");

	//����ͨ����ɫͼ����������ͨ��ͼ��
	split(srcImage, channels);//����ɫ��ͨ��
	imageRedChannel = channels.at(2);//��ԭͼ�ĺ�ɫͨ��ֵ���÷��ظ�imageBlueChennal;
									  //���õ��ĺ�ɫͨ��ͼ����ԭͼ���л�ϡ�
	addWeighted(imageRedChannel(Rect(500, 250, logoImage.cols, logoImage.rows)), 1.0,
		logoImage, 0.5, 0., imageRedChannel(Rect(500, 250, logoImage.cols, logoImage.rows)));

	//��ͼ��ϲ�
	merge(channels, srcImage);
	//��ʾ����
	namedWindow("<3>orignal picture + logo Red channel ");
	imshow("<3>orignal picture + logo Red channel ", srcImage);

	return true;

}
