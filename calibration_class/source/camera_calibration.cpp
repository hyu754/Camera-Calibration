#include <stdio.h>

#include <iostream>
#include <vector>



#include <opencv2\opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>
#include <opencv2/calib3d.hpp>
#include <camera_calibration_lib.hpp>


using namespace cv;
using namespace std;


class calibration_geometry
{
public:
	calibration_geometry();
	~calibration_geometry();
	enum direction_t{ left, right, mono_view };//The different calibration types
	enum calibration_t{ mono, stereo };//
	direction_t dir;
	calibration_t type;
	int numBoards;
	int horz_num;
	int vert_num;
	int number_blocks;
	int numSuccess;
	
	cv::Size board_size;
	std::vector<cv::Point2f> corners;
	std::vector<cv::Point2f> corners_left;
	std::vector<cv::Point2f> corners_right;

	void increase_sucess(void){ numSuccess++; std::cout << type << " calibration success. With " << numSuccess << " calibrations" << std::endl; }
	bool calibrate(direction_t dir_in){
		cv::Mat *img_ptr;
		std::vector<cv::Point2f> *corner_ptr;
		(dir_in == left) ? img_ptr = &image_left : img_ptr = &image_right;
		(dir_in == left) ? corner_ptr = &corners_left : corner_ptr = &corners_right;
		bool found = cv::findChessboardCorners(*img_ptr, board_size, *corner_ptr, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);




		return found;
	}
	void update_image_points(direction_t dir_in){
		std::vector<std::vector<cv::Point2f>> *img_point_ptr;
		std::vector<cv::Point2f> *corner_ptr;
		(dir_in == left) ? img_point_ptr = &image_points_left : img_point_ptr = &image_points_right;
		(dir_in == left) ? corner_ptr = &corners_left : corner_ptr = &corners_right;

		img_point_ptr->push_back(*corner_ptr);


	}
	void update_chessboard(void){
		object_points.push_back(chessboard_geometry);
	}
	int get_numSuccess(void){ return numSuccess; }

	void get_image(cv::Mat *in_image){ image = *in_image; }
	void get_image_left(cv::Mat *in_image){ image_left = *in_image; }
	void get_image_right(cv::Mat *in_image){ image_right = *in_image; }
	std::vector<std::vector<cv::Point3f>> return_objectPoints(void){ return object_points; }


	std::vector<std::vector<cv::Point3d>> return_obejctPoints_d(void){
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




	std::vector<std::vector<cv::Point2f>> return_image_points(void){ return image_points; }
	std::vector<std::vector<cv::Point2f>> return_image_points_left(void){ return image_points_left; }


	std::vector<std::vector<cv::Point2d>> return_image_points_left_d(void){

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
	std::vector<std::vector<cv::Point2f>> return_image_points_right(void){ return image_points_right; }

	std::vector<std::vector<cv::Point2d>> return_image_points_right_d(void){

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

	vector<Point2f> return_left_mat(void){ return left_points_mat; }
	vector<Point2f> return_right_mat(void){ return right_points_mat; }


	//returns if real time or not
	int get_real_time_var(void){ return real_time; }
	
private:
	cv::Mat image;

	//if calibrating a stereo_camera
	cv::Mat image_left;
	cv::Mat image_right;


	vector<Point2f> left_points_mat;
	vector<Point2f> right_points_mat;
	std::vector<std::vector<cv::Point3f>> object_points;
	std::vector<std::vector<cv::Point2f>> image_points;
	std::vector<std::vector<cv::Point2f>> image_points_left;
	std::vector<std::vector<cv::Point2f>> image_points_right;
	std::vector<cv::Point3f> chessboard_geometry;

	//variable to indicate if it will be real time calibration or input, currently real time is only supported for mono 
	int real_time;
};

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

