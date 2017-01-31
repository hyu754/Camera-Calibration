#include <stdio.h>

#include <iostream>
#include <vector>


#include <opencv2\opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>
#include <opencv2/calib3d.hpp>
#include "calibration_class.hpp"
using namespace cv;
using namespace std;

using namespace calibration;

int main(int argc, char** argv )
{
    //creating the calibration class, this class is responsible for storing the image chessboard points.
	calibration_geometry var;


	VideoCapture *cap;
	
	if (var.get_real_time_var() == 1){
		cap = new VideoCapture(0);
		if (cap->isOpened() == false){
			std::cerr << "ERROR: could not open camera for real time capture" << std::endl;
			return -1;
		}

	}
	



	if (var.get_real_time_var() == 0){

		if (var.type == calibration::calibration_t::stereo){

		}

	}



	if (cap != NULL)delete cap;


    return 0;
}