#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>

using namespace cv;
using namespace std;

Mat Hf(Mat, int);
Mat wiener(Mat I, int noise_stddev ,int k);
Mat inverse_filter(Mat I,int k);
Mat padd_image(Mat I);
Mat get_spectrum(Mat I);
Mat get_dft(Mat I);
Mat with_noise(Mat image, int stddev);
Mat rand_noise(Mat I, int stddev);


int main(int argc, char *argv[]) {
	// noise_stddev噪声标准差，用来调节噪声的强弱
	int noise_stddev = 20;
	// kkk HF变换函数中的一个可调参数
	int kkk = 0.5;

	Mat image = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("origin img",image);

	//Hf transform and add noise 傅立叶变换和反变换，在频域实现滤波
	Mat padded = padd_image(image);
	Mat planes[2];
	Mat complexI = get_dft(padded);
	Mat Hf_result =  Hf(complexI, kkk);
	idft(Hf_result, Hf_result);
	split(Hf_result, planes);
	normalize(planes[0], planes[0], 0, 255, CV_MINMAX );
	Mat HF_image;
	planes[0].convertTo(HF_image, CV_8UC1);
	Mat HF_noise_image = with_noise(HF_image,noise_stddev);
	imshow("Hf & add noise",HF_noise_image);

	//wiener filter 维纳滤波器
	Mat wiener_image = wiener(HF_noise_image, noise_stddev , kkk);
	imshow("wiener filter", wiener_image);

	//inverse filter 逆滤波器
	Mat inverse_image = inverse_filter(HF_noise_image, kkk);
	imshow("inverse filter", inverse_image);

	waitKey(0);
}

//逆滤波
Mat inverse_filter(Mat I,int k){
	Mat padded = padd_image(I);
	Mat planes[2];
	Mat complexI = get_dft(padded);
	split(complexI, planes);	// planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))

	Mat realI = planes[0];
	Mat magI = planes[1];

	//频域HF函数与HX相乘在此 实部分
	int nl = realI.rows;
	int nc = realI.cols;
	for(int j=0;j<nl;j++){
		unsigned char *data = realI.ptr<unsigned char>(j);
		for(int i=0;i<nc;i++){
			double tmp = -k * std::pow(i*i + j*j , 5/6);
			double e1 = std::exp(tmp);
			data[i] = data[i] / e1;
		}
	}

	//虚部分
	nl = magI.rows;
	nc = magI.cols;
	for(int j=0;j<nl;j++){
		unsigned char *data = magI.ptr<unsigned char>(j);
		for(int i=0;i<nc;i++){
			double tmp = -k * std::pow(i*i + j*j , 5/6);
			double e1 = std::exp(tmp);
			data[i] = data[i] / e1;
		}
	}

	//将实部和虚部合并
	Mat inverse_result;
	merge(planes,2,inverse_result);

	//傅立叶反变换
	idft(inverse_result, inverse_result);

	//归一化处理 
	split(inverse_result, planes);
	normalize(planes[0], planes[0], 0, 255, CV_MINMAX );
	Mat normalized;
	planes[0].convertTo(normalized, CV_8UC1);

	return normalized;

}

//维纳滤波器 过程与上类似
Mat wiener(Mat I, int noise_stddev, int k = 2){
	Mat padded = padd_image(I);
	Mat image_spectrum = get_spectrum(padded);

	Mat noise = rand_noise(padded, noise_stddev);
	Mat noise_spectrum = get_spectrum(noise);

	Mat planes[2];
	Mat complexI = get_dft(padded);
	split(complexI, planes);	// planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))

	Mat realI = planes[0];
	Mat magI = planes[1];
	Mat factor = noise_spectrum / image_spectrum ;

	int nl = realI.rows;
	int nc = realI.cols;
	for(int j=0;j<nl;j++){
		unsigned char *data = realI.ptr<unsigned char>(j);
		for(int i=0;i<nc;i++){
			double tmp = -k * std::pow(i*i + j*j , 5/6);
			double e1 = std::exp(tmp);
			double e2 = std::exp(2*tmp);
			double wi = e1/(e2 + factor.at<int>(i,j));
			data[i] = data[i] * wi;
		}
	}

	nl = magI.rows;
	nc = magI.cols;
	for(int j=0;j<nl;j++){
		unsigned char *data = magI.ptr<unsigned char>(j);
		for(int i=0;i<nc;i++){
			double tmp = -k * std::pow(i*i + j*j , 5/6);
			double e1 = std::exp(tmp);
			double e2 = std::exp(2*tmp);
			double wi = e1/(e2 + factor.at<int>(i,j));
			data[i] = data[i] * wi;
		}
	}

	Mat wiener_result;
	merge(planes,2,wiener_result);

	idft(wiener_result, wiener_result);

	split(wiener_result, planes);
	normalize(planes[0], planes[0], 0, 255, CV_MINMAX );
	Mat normalized;
	planes[0].convertTo(normalized, CV_8UC1);
	return normalized;
}

//扩展 方便FFT计算
Mat padd_image(Mat I){
	Mat padded;
	int m = getOptimalDFTSize( I.rows );
	int n = getOptimalDFTSize( I.cols ); // on the border add zero pixels
	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
	return padded;
}

//取得功率谱函数
Mat get_spectrum(Mat I){
	Mat complexI = get_dft(I);
	Mat planes[2];
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
	Mat magI = planes[0];
	multiply(magI,magI,magI);
	return magI;
}

//dft运算
Mat get_dft(Mat I){
	Mat image;
	I.convertTo(image, CV_32F);
	Mat planes[] = {Mat_<float>(image), Mat::zeros(image.size(), CV_32F)};
	Mat complexI;
	merge(planes, 2, complexI);
	dft(complexI, complexI);
	return complexI;
}

// HF变换
Mat Hf(Mat complexI,int k = 2){
	Mat planes[2];
	split(complexI, planes);
	Mat realI = planes[0];
	Mat magI = planes[1];

	int nl = realI.rows;
	int nc = realI.cols;
	for(int j=0;j<nl;j++){
		unsigned char *data = realI.ptr<unsigned char>(j);
		for(int i=0;i<nc;i++){
			double tmp = std::pow(i*i + j*j , 5/6);
			data[i] = data[i] * std::exp(-k*tmp);
		}
	}

	nl = magI.rows;
	nc = magI.cols;
	for(int j=0;j<nl;j++){
		unsigned char *data = magI.ptr<unsigned char>(j);
		for(int i=0;i<nc;i++){
			double tmp = std::pow(i*i + j*j , 5/6);
			data[i] = data[i] * std::exp(-k*tmp);
		}
	}

	Mat Hf_result;
	merge(planes,2,Hf_result);
	return Hf_result;
}

// 加噪声
Mat with_noise(Mat image, int stddev){
	Mat noise(image.rows, image.cols, CV_8U);
	rand_noise(image, stddev).convertTo(noise, CV_8U);
	Mat noisy = image.clone();
	noisy += noise;
	return noisy;
}

//随机白噪声
Mat rand_noise(Mat I, int stddev){
	Mat noise = Mat::zeros(I.rows, I.cols, CV_32F);
	randn(noise,Scalar::all(0), Scalar::all(stddev));
	return noise;
}
