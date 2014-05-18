#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>

using namespace cv;
using namespace std;

static void help()
{
    printf("\nThis program demonstrated the use of the discrete Fourier transform (dft)\n"
           "The dft of an image is taken and it's power spectrum is displayed.\n"
           "Usage:\n"
            "./dft [image_name -- default lena.jpg]\n");
}

// rearrange the quadrants of Fourier image
// so that the origin is at the image center
Mat rearrange(const Mat& src,const int cx,const int cy){

    Mat dst = src.clone();

    Mat tmp;
    Mat q0(dst, Rect(0, 0, cx, cy));
    Mat q1(dst, Rect(cx, 0, cx, cy));
    Mat q2(dst, Rect(0, cy, cx, cy));
    Mat q3(dst, Rect(cx, cy, cx, cy));

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    return dst;
}

const char* keys =
{
    "{1| |lena.jpg|input image file}"
};

int main(int argc, const char ** argv)
{
    help();
    CommandLineParser parser(argc, argv, keys);
    string filename = parser.get<string>("1");

    Mat img = imread(filename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
    if( img.empty() )
    {
        help();
        printf("Cannot read image file: %s\n", filename.c_str());
        return -1;
    }
    imshow("src image",img);

    int M = getOptimalDFTSize( img.rows );
    int N = getOptimalDFTSize( img.cols );
    Mat padded;
    copyMakeBorder(img, padded, 0, M - img.rows, 0, N - img.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;
    merge(planes, 2, complexImg);

    dft(complexImg, complexImg);

    // compute log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2))
    split(complexImg, planes);
    magnitude(planes[0], planes[1], planes[0]);
    Mat mag = planes[0];
    mag += Scalar::all(1);
    log(mag, mag);

    // crop the spectrum, if it has an odd number of rows or columns
    mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));

    // normalization
    normalize(mag, mag, 0, 1, CV_MINMAX);
    imshow("origin magnitude",mag);

    int cx = mag.cols/2;
    int cy = mag.rows/2;
    // rearrange the quadrants of Fourier image
    Mat mag_rearrange = rearrange(mag,cx,cy);
    imshow("spectrum magnitude", mag_rearrange);

    Size filter_size(75,75);

    //Low Pass Filter
    Mat lowPass(mag.size(),mag.type(),Scalar(0));
    Mat filter_region = lowPass(Rect(cx-filter_size.width/2, cy-filter_size.height/2 ,filter_size.width,filter_size.height));
    Mat grab = mag_rearrange(Rect(cx-filter_size.width/2, cy-filter_size.height/2 ,filter_size.width,filter_size.height));
    Mat tmp = grab.clone();
    add(tmp,filter_region,filter_region);
    imshow("lowPass",lowPass);

    Mat lowPass_rearrange = rearrange(lowPass,cx,cy);
    imshow("lo",lowPass_rearrange);

    Mat lowPass_image;
    idft(lowPass_rearrange,lowPass_image);
    imshow("lowPass_image",lowPass_image);

    //High Pass Filter
    Mat highPass = mag_rearrange.clone();
    filter_region = highPass(Rect(cx-filter_size.width/2, cy-filter_size.height/2 ,filter_size.width,filter_size.height));
    filter_region = Scalar::all(0);
    imshow("highPass",highPass);

    Mat highPass_rearrange = rearrange(highPass,cx,cy);

    waitKey(0);
    return 0;
}
