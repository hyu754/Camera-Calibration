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




bool calibration::calibration_geometry::calibrate(direction_t dir_in){
	cv::Mat *img_ptr;
	std::vector<cv::Point2f> *corner_ptr;
	(dir_in == left) ? img_ptr = &image_left : img_ptr = &image_right;
	(dir_in == left) ? corner_ptr = &corners_left : corner_ptr = &corners_right;
	bool found = cv::findChessboardCorners(*img_ptr, board_size, *corner_ptr, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);




	return found;
}


void calibration::calibration_geometry::update_image_points(direction_t dir_in){
	std::vector<std::vector<cv::Point2f>> *img_point_ptr;
	std::vector<cv::Point2f> *corner_ptr;
	(dir_in == left) ? img_point_ptr = &image_points_left : img_point_ptr = &image_points_right;
	(dir_in == left) ? corner_ptr = &corners_left : corner_ptr = &corners_right;

	img_point_ptr->push_back(*corner_ptr);


}

std::vector<std::vector<cv::Point3d>> calibration::calibration_geometry::return_obejctPoints_d(void){
	std::vector<std::vector<cv::Point3d>> output_vector;
	for (auto cc = object_points.begin(); cc != object_points.end(); ++cc){
		std::vector<cv::Point3d> temp;
		for (auto dd = cc->begin(); dd != cc->end(); ++dd){
			temp.push_back(Point3d(dd->x, dd->y, dd->z));
		}
		output_vector.push_back(temp);
	}
	return output_vector;
}

std::vector<std::vector<cv::Point2d>> calibration::calibration_geometry::return_image_points_left_d(void){

	std::vector<std::vector<cv::Point2d>> output_vector;
	for (auto cc = image_points_left.begin(); cc != image_points_left.end(); ++cc){
		std::vector<cv::Point2d> temp;
		for (auto dd = cc->begin(); dd != cc->end(); ++dd){
			temp.push_back(Point2d(dd->x, dd->y));
			left_points_mat.push_back(Point2f(dd->x, dd->y));
		}
		output_vector.push_back(temp);
	}
	return output_vector;
}

std::vector<std::vector<cv::Point2d>> calibration::calibration_geometry::return_image_points_right_d(void){

	std::vector<std::vector<cv::Point2d>> output_vector;
	for (auto cc = image_points_right.begin(); cc != image_points_right.end(); ++cc){
		std::vector<cv::Point2d> temp;
		for (auto dd = cc->begin(); dd != cc->end(); ++dd){
			temp.push_back(Point2d(dd->x, dd->y));
			right_points_mat.push_back(Point2f(dd->x, dd->y));

		}
		output_vector.push_back(temp);
	}
	return output_vector;
}

bool calibration::calibration_geometry::file_in_range(void){
	if ((file_counter + begin_suffix) <= end_suffix){
	
		return true;
	}
	else{
		return false;
	}
}

void calibration::calibration_geometry::read_in_images(void){
	if (type == calibration::calibration_t::stereo){
		std::string filename_left = left_prefix + std::to_string(file_counter) + image_type;
		std::string filename_right = right_prefix + std::to_string(file_counter) + image_type;
		image_left = imread(filename_left, IMREAD_GRAYSCALE);
		image_right = imread(filename_right, IMREAD_GRAYSCALE);

		std::cout << "Reading in image set : " << file_counter << std::endl;
		file_counter++;
	}
}
void calibration::calibration_geometry::display_images(void){
	if ((image_left.size == 0) | (image_right.size == 0)){
		std::cerr << "ERROR: one of the images are empty" << std::endl;
	}
	else{
		cv::imshow("left_image", image_left);
		cv::imshow("right_image", image_right);
	}
	cv::waitKey(1);
}

void calibration::calibration_geometry::calibrate_stereo_intrinsics(void){
	std::cout << "Calibrating stereo intrinsics" << std::endl;

	double rms_left = cv::calibrateCamera(object_points, image_points_left, image_left.size(), intrinsic_matrix[0], distortion_vector[0], cv::noArray(), cv::noArray());
	double rms_right = cv::calibrateCamera(object_points, image_points_right, image_left.size(), intrinsic_matrix[1], distortion_vector[1], cv::noArray(), cv::noArray());

	std::cout << "RMS for left : " << rms_left << std::endl;
	std::cout << "RMS for right : " << rms_right << std::endl;
}

void calibration::calibration_geometry::calibrate_stereo_parameters(void){
	cv::TermCriteria criteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.001);

	std::cout << "Calibrating stereo parameters " << std::endl;
	double rms_stereo = cv::stereoCalibrate(object_points, image_points_left, image_points_right, intrinsic_matrix[0], distortion_vector[0], intrinsic_matrix[1], distortion_vector[1], image_left.size(), R, T, E, F, CV_CALIB_RATIONAL_MODEL | CV_CALIB_USE_INTRINSIC_GUESS | CV_CALIB_FIX_PRINCIPAL_POINT, criteria);
	cv::stereoRectify(intrinsic_matrix[0], distortion_vector[0], intrinsic_matrix[1], distortion_vector[1], image_left.size(), R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY);
	
	std::cout << "STEREO CALIBRATE ERROR : " << rms_stereo << std::endl;
}

void calibration::calibration_geometry::write_camera_properties_stereo(void){
	std::string filename_left = "Left_Intrinsic.xml";
	std::string filename_right = "Right_Intrinsic.xml";
	std::string filename_stereo = "Stereo_Prop.xml";

	cv::FileStorage f_output_left(filename_left, cv::FileStorage::WRITE);

	f_output_left << "Intrinsic" << intrinsic_matrix[0];
	f_output_left << "Distortion" << distortion_vector[0];
	f_output_left.release();


	cv::FileStorage f_output_right(filename_right, cv::FileStorage::WRITE);

	f_output_right << "Intrinsic" << intrinsic_matrix[1];
	f_output_right << "Distortion" << distortion_vector[1];
	f_output_right.release();

	cv::FileStorage f_output_stereo(filename_stereo, cv::FileStorage::WRITE);
	f_output_stereo << "R" << R;
	f_output_stereo << "T" << T;
	f_output_stereo << "E" << E;
	f_output_stereo << "F" << F;
	f_output_stereo << "R1" << R1;
	f_output_stereo << "R2" << R2;
	f_output_stereo << "P1" << P1;
	f_output_stereo << "P2" << P2;
	f_output_stereo << "Q" << Q;
	f_output_stereo.release();
}
calibration::calibration_geometry::calibration_geometry()
{
	std::cout << "Enter number of corners along width: ";
	std::cin >> horz_num;

	std::cout << "Enter number of corners along horizontal: ";
	std::cin >> vert_num;


	double chess_size;
	std::cout << "Enter the size of each chessboard block (mm): ";
	std::cin >> chess_size;


	//std::cout << "Enter number of boards: ";
	//std::cin >> numBoards;

	std::cout << std::endl;
	int l_or_r;
	std::cout << "Mono (0) or stereo (1) camera? :";
	std::cin >> l_or_r;
	(l_or_r == 0) ? type = mono : type = stereo;

	std::cout << "Real time (1) or from file (0): ";
	std::cin >> real_time;




	//Generating the actual chessboard marker geometry, without any distortion
	double horz = 0.0f, vert = 0.0f;
	double squareSize = chess_size / 1000.0f;

	double width_ = (horz_num - 1.0)*squareSize;
	double height_ = (vert_num - 1.0)*squareSize;
	for (int i = 0; i < vert_num; i++)
		for (int j = 0; j < horz_num; j++)
			chessboard_geometry.push_back(Point3f(j*squareSize, i*squareSize, 0.0));



	number_blocks = horz_num*vert_num;

	board_size = cv::Size(horz_num, vert_num);

	//Success counter
	numSuccess = 0;


	std::cout << "Please input left image prefix  (i.e. left_1.jpg, then enter left_): ";
	std::cin >> left_prefix;

	std::cout << "Please input right image prefix  (i.e. right_1.jpg, then enter left_): ";
	std::cin >> right_prefix;

	std::cout << "Please enter starting image suffix (i.e. for a list like left_1.jpg .... left_50.jpg enter \"1\" ): ";
	std::cin >> begin_suffix;
	std::cout << "Please enter ending image suffix (i.e. for a list like left_1.jpg .... left_50.jpg enter \"50\" ): ";
	std::cin >> end_suffix;

	std::cout << "Please enter file type (.jpg, .png, etc): ";
	std::cin >> image_type;


	file_counter = begin_suffix;

}

calibration::calibration_geometry::~calibration_geometry()
{
	cout << "Destroying the calibration geometry class" << endl;
}
