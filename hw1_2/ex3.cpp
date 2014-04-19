//ex3.cpp 幅度分辨率变换实验
#include <highgui/highgui.hpp>

#define uchar unsigned char
#define GREY_256_TO_64 1
#define GREY_256_TO_16 2 
#define GREY_256_TO_8 3
#define GREY_256_TO_4 4
#define GREY_256_TO_2 5

//灰度变换函数
int greyScale(const IplImage* src,IplImage* dst,const int scale)
{
	uchar* p_s_data = (uchar*)(src->imageData);
	uchar* p_d_data = (uchar*)(dst->imageData);
	int step = src->widthStep/sizeof(uchar);
	int step2 = dst->widthStep/sizeof(uchar);
	int tmp;
	for(int y=0;y<src->height;y++)
	{
		for(int x=0;x<src->width;x++)
		{
			switch(scale)
			{
				case 1 : tmp = p_s_data[y*step + x]/4*4 + 2;break;
				case 2 : tmp = p_s_data[y*step + x]/16*16 + 8;break;
				case 3 : tmp = p_s_data[y*step + x]/32*32 + 16;break;
				case 4 : tmp = p_s_data[y*step + x]/64*64 + 32;break;
				case 5 : tmp = p_s_data[y*step + x]/128*128 + 64;break;
			}
			p_d_data[y*step2 + x] = tmp;
		}
	}
	return 0;	
}

int main(int argc,char** argv)
{
	//原lena.jpg图像为256灰度级
	IplImage* src = cvLoadImage("lena.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* img_6_bit = cvCreateImage(cvSize(512,512),IPL_DEPTH_8U,1);
	IplImage* img_4_bit = cvCreateImage(cvSize(512,512),IPL_DEPTH_8U,1);
	IplImage* img_3_bit = cvCreateImage(cvSize(512,512),IPL_DEPTH_8U,1);
	IplImage* img_2_bit = cvCreateImage(cvSize(512,512),IPL_DEPTH_8U,1);
	IplImage* img_1_bit = cvCreateImage(cvSize(512,512),IPL_DEPTH_8U,1);

	//灰度变换
	greyScale(src,img_6_bit,GREY_256_TO_64);
	greyScale(src,img_4_bit,GREY_256_TO_16);
	greyScale(src,img_3_bit,GREY_256_TO_8);
	greyScale(src,img_2_bit,GREY_256_TO_4);
	greyScale(src,img_1_bit,GREY_256_TO_2);

	//显示图像
	cvNamedWindow("Grey-8-bits");
	cvNamedWindow("Grey-6-bits");
	cvNamedWindow("Grey-4-bits");
	cvNamedWindow("Grey-3-bits");
	cvNamedWindow("Grey-2-bits");
	cvNamedWindow("Grey-1-bits");

	cvShowImage("Grey-8-bits",src);
	cvShowImage("Grey-6-bits",img_6_bit);
	cvShowImage("Grey-4-bits",img_4_bit);
	cvShowImage("Grey-3-bits",img_3_bit);
	cvShowImage("Grey-2-bits",img_2_bit);
	cvShowImage("Grey-1-bits",img_1_bit);

	cvWaitKey(0);

	//释放
	cvReleaseImage(&src);
	cvReleaseImage(&img_6_bit);
	cvReleaseImage(&img_4_bit);
	cvReleaseImage(&img_3_bit);
	cvReleaseImage(&img_2_bit);
	cvReleaseImage(&img_1_bit);
	return 0;
}
