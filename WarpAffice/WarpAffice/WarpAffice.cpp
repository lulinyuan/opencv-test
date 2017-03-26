// WarpAffice.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//-----------���궨�塿--------
#define WINDOWS_NAME1 "ԭʼͼ"
#define WINDOWS_NMAE2 "����Warp���ͼ��"
#define WINDOWS_NAME3 "����Wrap��Rotate��Ĵ���"

//----------ȫ�ֺ�����������
static void ShowHelpText();




int main()
{
	//�ı������ɫ����ʾ������Ϣ
	system("color 5F");
	ShowHelpText();

	//��������㣬��������������
	Point2f srcTriangel[3];
	Point2f dstTriangel[3];

	//����Mat����
	Mat rotMat(2, 3, CV_32FC1);
	Mat warpMat(2, 3, CV_32FC1);
	Mat srcImage, dstImage_warp, dstImage_warp_rotate;


	//����ԭͼ��
	srcImage = imread("1.jpg");
	if (!srcImage.data)
	{
		printf("cannot find the file! Plese check it\n");
		return false;
	}

	//����Ŀ��ͼ��Ĵ�С�����ͺ�Դͼ��һ��
	dstImage_warp = Mat::zeros( srcImage.rows, srcImage.cols, srcImage.type());

	//����Դͼ���Ŀ��ͼ���ϵ�����㼰�������任
	srcTriangel[0] = Point2f(0, 0);
	srcTriangel[1] = Point2f(static_cast<float>(srcImage.cols - 1), 0);
	srcTriangel[2] = Point2f(1, static_cast<float>(srcImage.rows - 1));

	dstTriangel[0] = Point(static_cast<float>(srcImage.cols*0.0), static_cast<float>(srcImage.rows*0.33));
	dstTriangel[1] = Point(static_cast<float>(srcImage.cols*0.65), static_cast<float>(srcImage.rows*0.35));
	dstTriangel[2] = Point(static_cast<float>(srcImage.cols*0.15), static_cast<float>(srcImage.rows*0.6));

	//��÷���任
	warpMat = getAffineTransform( srcTriangel, dstTriangel);
	//��ԭͼ��Ӧ�øո���õķ���任
	warpAffine(srcImage, dstImage_warp, warpMat, dstImage_warp.size());

	//��ͼ��������ź�����ת
	//������ͼ���е�˳ʱ����ת50����������Ϊ0.6����ת����
	Point center = Point(dstImage_warp.cols / 2, dstImage_warp.rows / 2);
	double angle = -90.0;
	double scale = 0.8;

	//ͨ���������תϸ����Ϣ����ת����
	rotMat = getRotationMatrix2D(center, angle, scale);

	//��ת�Ѿ����ŵ�ͼ��
	warpAffine(dstImage_warp, dstImage_warp_rotate, rotMat, dstImage_warp.size());

	//��ʾ���
	imshow(WINDOWS_NAME1,srcImage);
	imshow(WINDOWS_NMAE2, dstImage_warp);
	imshow(WINDOWS_NAME3, dstImage_warp_rotate);

	waitKey(0);

    return 0;
}
static void ShowHelpText()
{

	//�����ӭ��Ϣ��OpenCV�汾
	printf("\n\n\t\t\t   ��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");

	//���һЩ������Ϣ  
	printf("\n\n\t\t��ӭ��������任�ۺ�ʾ������.\n\n");
	printf("\t\t���̰�����ESC��- �˳�����\n");
}

