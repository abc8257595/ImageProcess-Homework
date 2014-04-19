#include "opencv.hpp"
#include "legacy/compat.hpp"
#include "highgui/highgui.hpp"


//绘制直方图
IplImage* DrawHistogram(CvHistogram* hist , float scaleX = 1 , float scaleY = 1)
{
    //获取直方图中极大值
    float histMax = 0;
    cvGetMinMaxHistValue(hist , 0 ,&histMax ,0 ,0);

    //创建图像 该图像用于显示直方图
    IplImage* imgHist = cvCreateImage(cvSize(256*scaleX , 64*scaleY) , 8 ,1);

    //图像置零
    cvZero(imgHist);

    //依次绘制直方图的bin
    for(int i=0;i<255;i++)
    {
            //获取直方图的值
            float histValue = cvQueryHistValue_1D(hist , i);
            float nextValue = cvQueryHistValue_1D(hist , i+1);

            //获取四边形的四个点的坐标  详情请观看视频的讲解
            CvPoint pt1 = cvPoint(      i*scaleX , 64*scaleY);
            CvPoint pt2 = cvPoint(  (i+1)*scaleX , 64*scaleY);
            CvPoint pt3 = cvPoint(  (i+1)*scaleX , (64 - (nextValue/histMax)*64) *scaleY );
            CvPoint pt4 = cvPoint (      i*scaleX , (64 - (histValue/histMax)*64) *scaleY );

            int numPts = 5;
            CvPoint pts[5];
            pts[0] = pt1;
            pts[1] = pt2;
            pts[2] = pt3;
            pts[3] = pt4;
            pts[4] = pt1;

            //填充四边形
            cvFillConvexPoly(imgHist , pts ,numPts , cvScalar(255));
    }
    return imgHist;
}


int main(int argc, char* argv[])
{

    //加载图像
    IplImage* src = cvLoadImage("lena.jpg",0);
    
    cvNamedWindow("src");
    cvShowImage("src" , src);

    int dims = 1;  // 一维直方图
    int size =256; //bin的个数
    float range[] = {0,255}; //取值范围
    float* ranges[] = {range};

    CvHistogram* hist;

    //创建直方图
    hist = cvCreateHist(dims , &size , CV_HIST_ARRAY ,  ranges , 1 );

    //清空直方图
    cvClearHist(hist);

    cvCalcHist(&src , hist , 0 , 0 );

    //绘制直方图
    IplImage* histImage = DrawHistogram(hist,2,2);

    cvNamedWindow("Hist");
    cvShowImage("Hist"  , histImage);

    cvWaitKey(0);

    //释放
    cvReleaseImage(&src);
    cvReleaseImage(&histImage);
    cvReleaseHist(&hist);

    return 0;
}
