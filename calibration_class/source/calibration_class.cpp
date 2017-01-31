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




bool calibration_geometry::calibrate(direction_t dir_in){
	cv::Mat *img_ptr;
	std::vector<cv::Point2f> *corner_ptr;
	(dir_in == left) ? img_ptr = &image_left : img_ptr = &image_right;
	(dir_in == left) ? corner_ptr = &corners_left : corner_ptr = &corners_right;
	bool found = cv::findChessboardCorners(*img_ptr, board_size, *corner_ptr, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);




	return found;
}


void calibration_geometry::update_image_points(direction_t dir_in){
	std::vector<std::vector<cv::Point2f>> *img_point_ptr;
	std::vector<cv::Point2f> *corner_ptr;
	(dir_in == left) ? img_point_ptr = &image_points_left : img_point_ptr = &image_points_right;
	(dir_in == left) ? corner_ptr = &corners_left : corner_ptr = &corners_right;

	img_point_ptr->push_back(*corner_ptr);


}

std::vector<std::vector<cv::Point3d>> calibration_geometry::return_obejctPoints_d(void){
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

std::vector<std::vector<cv::Point2d>> calibration_geometry::return_image_points_left_d(void){

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

std::vector<std::vector<cv::Point2d>> calibration_geometry::return_image_points_right_d(void){

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


calibration_geometry::calibration_geometry()
{
	std::cout << "Enter number of corners along width: ";
	std::cin >> horz_num;

	std::cout << "Enter number of corners along horizontal: ";
	std::cin >> vert_num;


	double chess_size;
	std::cout << "Enter the size of each chessboard block (mm): ";
	std::cin >> chess_size;


	std::cout << "Enter number of boards: ";
	std::cin >> numBoards;

	std::cout << std::endl;
	int l_or_r;
	std::cout << "Mono (0) or stereo (1) camera? :";
	std::cin >> l_or_r;
	(l_or_r == 0) ? type = mono : type = stereo;

	std::cout << "Real time (1) or from file (0)";
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


}

calibration_geometry::~calibration_geometry()
{
	cout << "Destroying the calibration geometry class" << endl;
}
