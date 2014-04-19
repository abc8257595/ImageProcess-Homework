#include <iostream>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>

using namespace cv;

int main(int argc,char** argv)
{
     //Read input image
	Mat image = imread(argv[1],1);
	if(!image.data)
	{
		std::cout<<"file not exist"<<std::endl;
		return -1;
	}
	
	//Display the origin image
	namedWindow("Origin Image");
	imshow("Origin Image",image);	

     //Create a Gussian Noise Mask
 	Mat Gussian_Noise_1 = image.clone();
	Mat mask(image.size(),CV_8UC3,Scalar(0));
	RNG rng(getTickCount());
	rng.fill(mask, RNG::NORMAL, 0, 40);
	//do a bitwise_or operation to pollute the original image
	bitwise_or(mask,Gussian_Noise_1,Gussian_Noise_1);

        //Display the Gussian Noise image 1
	namedWindow("Gussian Noise 1");
	imshow("Gussian Noise 1",Gussian_Noise_1);	

     //Display the Gussian Noise image 2
 	Mat Gussian_Noise_2 = image.clone();
	rng.fill(mask, RNG::NORMAL, 0, 40);
	bitwise_or(mask,Gussian_Noise_2,Gussian_Noise_2);

     //Display the Gussian Noise image 3
 	Mat Gussian_Noise_3 = image.clone();
	rng.fill(mask, RNG::NORMAL, 0, 40);
	bitwise_or(mask,Gussian_Noise_3,Gussian_Noise_3);

     //Display the Gussian Noise image 4
 	Mat Gussian_Noise_4 = image.clone();
	rng.fill(mask, RNG::NORMAL, 0, 40);
	bitwise_or(mask,Gussian_Noise_4,Gussian_Noise_4);

	//Average the Gussian Noise Image to get a better image
	Mat result(image.size(),CV_8UC3,Scalar(0));
	//result = 0.25*Gussian_Noise_1 + 0.25*Gussian_Noise_2 + 0.25*Gussian_Noise_3 + 0.25*Gussian_Noise_4;
	//result = 0.33*Gussian_Noise_1 + 0.33*Gussian_Noise_2 + 0.33*Gussian_Noise_3;
	//result = 0.5*Gussian_Noise_1 + 0.5*Gussian_Noise_2 ;
	GaussianBlur(Gussian_Noise_1,result,cv::Size(5,5),1.5);

	namedWindow("result");
	imshow("result",result);	
	waitKey(0);

	return 0;
}
