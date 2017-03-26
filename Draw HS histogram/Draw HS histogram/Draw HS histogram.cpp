// Draw HS histogram.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

#define WINDOWS_NAME1 "ԭʼͼ"
#define WINDOWS_NAME2 "H-S Ч��ͼ"
 
using namespace cv;

//--------------ȫ�ֺ�������
static void ShowHelpText();


int main()
{
	//�ı������ɫ�����������Ϣ
	system("color 5F");
	ShowHelpText();

	//����ͼ�� ת��ΪHsv��ɫģ��

	Mat srcImage, hsvImage;
	srcImage = imread("1.jpg", 1);

	cvtColor( srcImage, hsvImage, COLOR_BGR2HSV);

	//����׼��
	//��ɫ������Ϊ30���ȼ��������Ͷ�����Ϊ32���ȼ�
	int hueBinNum = 30;//ɫ����ֱ��ͼֱ������
	int saturationBinNum = 32; //���Ͷȵ�ֱ��ͼֱ������
	int histSize[] = {hueBinNum,saturationBinNum};

	//����ɫ���ı仯��ΧΪ0��179
	float hueRanges[] = { 0, 180 };
	//���履�Ͷȵı仯��ΧΪ0���ڡ��ס��ң���255����������ɫ��
	float saturationRanges[] = {0 ,256};
	const float* ranges[] = { hueRanges, saturationRanges};
	MatND dstHist;
	//����׼���� calHist�����н������0ͨ���͵�1ͨ����ֱ��ͼ
	int channel[] = { 0,1 };

	//����calHist������ֱ��ͼ����
	calcHist(&hsvImage,//��������
		1,//�������Ϊ1
		channel,//ͨ������
		Mat(), //��ʹ����Ĥ
		dstHist,//�����Ŀ��ֱ��ͼ
		2,//��Ҫ�����ֱ��ͼ��ά��
		histSize,//���ÿ��ά�ȵ�ֱ��ͼ�ߴ������
		ranges,//ÿһά�����ȡֵ��Χ����
		true,//ָʾֱ��ͼ�Ƿ���ȵı�ʾ���� TRUE��ʾ���ȵ�ֱ��ͼ
		false );//�ۼƱ�ʾ����false��ʾֱ��ͼ�����ý׶λᱻ����

	//����ֱ��ͼ׼������
	double maxValue = 0;
	minMaxIdx(dstHist, 0, &maxValue, 0, 0);//����������������ȫ����Сֵ�����ֵ����maxValue�� 
	int scale = 10; 
	Mat histImg = Mat::zeros(saturationBinNum*scale, hueBinNum*10, CV_8UC3);

	//˫��ѭ��������ֱ��ͼ
	for (int  hue = 0; hue < hueBinNum; hue++)
		for (int saturation = 0; saturation < saturationBinNum; saturation++)
		{
			float binValue = dstHist.at<float>(hue,saturation);//ֱ��ͼֱ����ֵ
			int intensity = cvRound(binValue*255/maxValue);//ǿ��

			//��ʽ���л���
			rectangle(histImg, Point(hue*scale, saturation*scale), Point((hue+1)*scale -1, (saturation+1)*scale-1), Scalar::all(intensity), FILLED);
		}

	//��ʾЧ��ͼ

	imshow(WINDOWS_NAME1, srcImage);
	imshow(WINDOWS_NAME2, histImg);

	waitKey(0);



    return 0;
}

//-----------------------------------��ShowHelpText( )������-----------------------------
//		 ���������һЩ������Ϣ
//----------------------------------------------------------------------------------------------
void ShowHelpText()
{
	//�����ӭ��Ϣ��OpenCV�汾
	printf("\n\n\t\t\t�ǳ���л����OpenCV3������š�һ�飡\n");
	printf("\n\n\t\t\t��Ϊ����OpenCV2��ĵ�79������ʾ������\n");
	printf("\n\n\t\t\t   ��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");
}
