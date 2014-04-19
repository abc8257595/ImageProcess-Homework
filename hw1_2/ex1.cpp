// ex1.cpp : 实现同时对比度实验
#include <stdio.h>
#include <highgui/highgui.hpp>

#define uchar unsigned char

int main(int argc,char** argv)
{
	IplImage* img1 = cvCreateImage(cvSize(300,300),IPL_DEPTH_8U,1);
	IplImage* img2 = cvCreateImage(cvSize(300,300),IPL_DEPTH_8U,1);
	cvNamedWindow("Img1");
	cvNamedWindow("Img2");
	uchar* p_data = (uchar*)(img1->imageData);
	int step = img1->widthStep/sizeof(uchar);
	//遍历图像，img1前景128，背景64
	for(int y=0;y<img1->height;y++)
	{
		for(int x=0;x<img1->width;x++)
			p_data[y*step + x] = 64;
	}
	for(int y=125;y<175;y++)
	{
		for(int x=125;x<175;x++)
			p_data[y*step + x] = 128;
	}
	//遍历图像，img2前景128，背景192
	p_data = (uchar*)(img2->imageData);
	for(int y=0;y<img2->height;y++)
	{
		for(int x=0;x<img2->width;x++)
			p_data[y*step + x] = 192;
	}
	for(int y=125;y<175;y++)
	{
		for(int x=125;x<175;x++)
			p_data[y*step + x] = 128;
	}
	cvShowImage("Img1",img1);
	cvShowImage("Img2",img2);
	
	cvWaitKey(0);
	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	cvDestroyWindow("img1");
	cvDestroyWindow("img2");
	return 0;
}
