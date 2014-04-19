#include <iostream>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>

using namespace cv;

int main(int argc,char** argv)
{
     //Read input image
	Mat image = imread(argv[1],0);
	if(!image.data)
	{
		std::cout<<"file not exist"<<std::endl;
		return -1;
	}
	
	//Display the origin image
	namedWindow("Origin Image");
	imshow("Origin Image",image);	

 	Mat Pulse_Noise = image.clone();
     //Create a Salt Noise Image
	Mat salt(image.size(),CV_8U,Scalar(0));
	RNG rng(getTickCount());
	rng.fill(salt, RNG::NORMAL, 0, 10);
	threshold(salt,salt,25,255,THRESH_BINARY);
	namedWindow("salt");
	imshow("salt",salt);	

        //Create a Pepper Noise Image
	Mat pepper(image.size(),CV_8U,Scalar(255));
	rng.fill(pepper, RNG::NORMAL, 0, 10);
	threshold(pepper,pepper,25,255,THRESH_BINARY_INV);
	namedWindow("pepper");
	imshow("pepper",pepper);	

	//Create a Pulse Noise Image
	Pulse_Noise = Pulse_Noise + salt;
	bitwise_and(Pulse_Noise,pepper,Pulse_Noise);
	namedWindow("Pulse_Noise");
	imshow("Pulse_Noise",Pulse_Noise);	

      //Median Blur to git rid of pulse noise
	Mat median = Pulse_Noise.clone();
	medianBlur(median,median,3);
	namedWindow("medianBlur");
	imshow("medianBlur",median);	

	waitKey(0);

	return 0;
}
