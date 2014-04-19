//ex2.cpp  空间分辨率变换实验
#include <highgui/highgui.hpp>

#define uchar unsigned char

//分辨率减半变换
int resolution(const IplImage* src,IplImage* dst)
{
	uchar* p_s_data = (uchar*)(src->imageData);
	uchar* p_d_data = (uchar*)(dst->imageData);
	int step = src->widthStep/sizeof(uchar);
	int step2 = dst->widthStep/sizeof(uchar);
	for(int y=0;y<dst->height;y++)
	{
		for(int x=0;x<dst->width;x++)
		{
			int sum = p_s_data[y*2*step + x*2] + p_s_data[y*2*step + x*2 + 1] + p_s_data[(y*2+1)*step + x*2] + p_s_data[(y*2+1)*step + x*2 + 1];
			p_d_data[y*step2 + x] = sum/4;
		}
	}
	return 0;
}

int main(int argc,char** argv)
{
	//原lena.jpg图像转为灰度图，为512*512，新建256*256，128*128，64*64，32*32，16*16图像
	IplImage* src = cvLoadImage("lena.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* img256 = cvCreateImage(cvSize(256,256),IPL_DEPTH_8U,1);
	IplImage* img128 = cvCreateImage(cvSize(128,128),IPL_DEPTH_8U,1);
	IplImage* img64 = cvCreateImage(cvSize(64,64),IPL_DEPTH_8U,1);
	IplImage* img32 = cvCreateImage(cvSize(32,32),IPL_DEPTH_8U,1);
	IplImage* img16 = cvCreateImage(cvSize(16,16),IPL_DEPTH_8U,1);
	
	//分辨率减半变换
	resolution(src,img256);
	resolution(img256,img128);
	resolution(img128,img64);
	resolution(img64,img32);
	resolution(img32,img16);

	//显示在窗口
	cvNamedWindow("512*512");
	cvNamedWindow("256*256");
	cvNamedWindow("128*128");
	cvNamedWindow("64*64");
	cvNamedWindow("32*32");
	cvNamedWindow("16*16");

	cvShowImage("512*512",src);
	cvShowImage("256*256",img256);
	cvShowImage("128*128",img128);
	cvShowImage("64*64",img64);
	cvShowImage("32*32",img32);
	cvShowImage("16*16",img16);

	cvWaitKey(0);

	//释放
	cvReleaseImage(&src);
	cvReleaseImage(&img256);
	cvReleaseImage(&img128);
	cvReleaseImage(&img64);
	cvReleaseImage(&img32);
	cvReleaseImage(&img16);
	return 0;
}
