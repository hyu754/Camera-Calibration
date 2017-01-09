#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
using namespace cv;
int main(int argc, char** argv )
{
    cv::VideoCapture g_p_cap(0);
	cv::Mat image_in;
	if (!g_p_cap.isOpened())  // check if we succeeded
		return -1;
	while (1){
		Mat frame;
		g_p_cap >> frame;

		cv::imshow("image", frame);

		cv::waitKey(1);


	}
    waitKey(0);
    return 0;
}