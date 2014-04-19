#include <iostream>

#include <opencv.hpp>
#include <highgui/highgui.hpp>
#include "histogram.h"

using namespace std;

int main(int argc,char** argv)
{
	// Read input image
	cv::Mat image= cv::imread(argv[1],0);
	if (!image.data)
		return 0; 

	// Display the image
	cv::namedWindow("Image");
	cv::imshow("Image",image);

        // The histogram object
	Histogram1D h;

	// Compute the histogram
	cv::MatND histo= h.getHistogram(image);

	// Display a histogram as an image
	cv::namedWindow("Histogram");
	cv::imshow("Histogram",h.getHistogramImage(image));


	// Equalize the image
	cv::Mat eq= h.equalize(image);

	// Show the result
	cv::namedWindow("Equalized Image");
	cv::imshow("Equalized Image",eq);

	// Show the new histogram
	cv::namedWindow("Equalized Histogram");
	cv::imshow("Equalized Histogram",h.getHistogramImage(eq));

	// Stretch the image ignoring bins with less than 5 pixels
	cv::Mat str= h.stretch(image,5);

	// Show the result
	cv::namedWindow("Stretched Image");
	cv::imshow("Stretched Image",str);

	// Show the new histogram
	cv::namedWindow("Stretched Histogram");
	cv::imshow("Stretched Histogram",h.getHistogramImage(str));

	// Create an image inversion table
	int dim(256);
	cv::MatND lookup(1,&dim,CV_8U);
	
	for (int i=0; i<256; i++) {
		
		lookup.at<unsigned char>(i)= 255-i;
	}

	// Apply lookup and display negative image
	cv::namedWindow("Negative image");
	cv::imshow("Negative image",h.applyLookUp(image,lookup));

	cv::waitKey();
	return 0;
}

