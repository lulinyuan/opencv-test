// impaint.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define WINDOWS_NAME0 "��ԭͼ�ο�ͼ��"
#define WINDOWS_NAME1 "��ԭʼͼ��"
#define WINDOWS_NAME2 "���޲����Ч��ͼ��"

//-----------��ȫ�ֱ�����----------
Mat srcImage0, srcImage1, inpaintMask;
Point previousPoint(-1,-1);//ԭ���ĵ�����
//------ȫ�ֺ�������
static void ShowHelpText();
static void On_Mouse(int event, int x, int y, int flags, void* );



//------------��On_Mouse������-------


int main()
{
	//�ı������ɫ���������Ϣ
	system("color 5F");
	ShowHelpText();

	//����ԭͼ��������Ĥ��ʼ��
	 Mat srcImage = imread("1.jpg",-1);
	if (!srcImage.data)
	{
		printf("cannot find the file! Plese check it \n");
		return false;
	}
	srcImage0 = srcImage.clone();
	srcImage1 = srcImage.clone();
	inpaintMask = Mat::zeros(srcImage1.size(), CV_8U);

	//��ʾԭʼͼ
	imshow(WINDOWS_NAME0, srcImage0);
	imshow(WINDOWS_NAME1, srcImage1);

	//�������ص���Ϣ
	setMouseCallback(WINDOWS_NAME1, On_Mouse, 0);

	//��ѯ������ ���ݲ�ͬ�������д���
	while (1)
	{
		//��ȡ����ֵ
		char key = waitKey(0);
		//��ֵΪESC���˳�
		if (key == 27)
			break;

		//��ֵΪ2 �ָ���ʾԭͼ
		if ( key == '2')
		{
			inpaintMask = Scalar::all(0);
			srcImage.copyTo(srcImage1);
			imshow( WINDOWS_NAME1, srcImage1);
		}

		//��ֵΪ1����spaceʱ ����ͼ��
		if (key == '1' || key == ' ')
		{
			Mat inpaintedImage;
			inpaint(srcImage, inpaintMask, inpaintedImage, 3, INPAINT_TELEA);
			imshow(WINDOWS_NAME2, inpaintedImage);

		}

	}



    return 0;
}

//--------��On_Mouse()������------------
void On_Mouse(int event, int x, int y, int flags, void*)
{
	//������������Ϣ
	if (event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON))
		previousPoint = Point(-1, -1);
	else if (event == EVENT_LBUTTONDOWN)
		previousPoint = Point(x, y);
	else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))
	{
		Point pt(x,y);
		if (previousPoint.x < 0)
			previousPoint = pt;
		//���ư�ɫ����
		line( inpaintMask, previousPoint, pt, Scalar::all(255), 5, 8, 0);
		line( srcImage1, previousPoint, pt, Scalar::all(255), 5, 8, 0);
		previousPoint = pt;
		imshow(WINDOWS_NAME1, srcImage1);

	}

}
//-----------------------------------��ShowHelpText( )������----------------------------------
//          ���������һЩ������Ϣ
//----------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//�����ӭ��Ϣ��OpenCV�汾
	printf("\n\n\t\t\t   ��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");

	//���һЩ������Ϣ
	printf("\n\n\n\t��ӭ������ͼ���޸���ʾ������~\n");
	printf("\n\t���ڽ���ͼ���޸�����֮ǰ���ڡ�ԭʼͼ�������н��������Ļ���"
		"\n\n\t��������˵��: \n\n"
		"\t\t����������-��ͼ���ϻ��ư�ɫ����\n\n"
		"\t\t���̰�����ESC��- �˳�����\n\n"
		"\t\t���̰�����1����SPACE��-����ͼ���޸����� \n\n");
}

