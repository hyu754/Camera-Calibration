#ifndef _CALIBRATION_CLASS_H
#define _CALIBRATION_CLASS_H

using namespace cv;
using namespace std;

namespace calibration{
	class calibration_geometry;
	enum direction_t{ left, right, mono_view };//The different calibration types
	enum calibration_t{ mono, stereo };//
}
namespace calibration{

	class calibration_geometry
	{
	public:
		calibration_geometry();
		~calibration_geometry();

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
		bool calibrate(direction_t dir_in);

		void update_image_points(direction_t dir_in);

		void update_chessboard(void){ object_points.push_back(chessboard_geometry); }


		int get_numSuccess(void){ return numSuccess; }

		void get_image(cv::Mat *in_image){ image = *in_image; }
		void get_image_left(cv::Mat *in_image){ image_left = *in_image; }
		void get_image_right(cv::Mat *in_image){ image_right = *in_image; }
		std::vector<std::vector<cv::Point3f>> return_objectPoints(void){ return object_points; }


		std::vector<std::vector<cv::Point3d>> return_obejctPoints_d(void);


		std::vector<std::vector<cv::Point2f>> return_image_points(void){ return image_points; }

		std::vector<std::vector<cv::Point2f>> return_image_points_left(void){ return image_points_left; }


		std::vector<std::vector<cv::Point2d>> return_image_points_left_d(void);

		std::vector<std::vector<cv::Point2f>> return_image_points_right(void){ return image_points_right; }

		std::vector<std::vector<cv::Point2d>> return_image_points_right_d(void);

		vector<Point2f> return_left_mat(void){ return left_points_mat; }
		vector<Point2f> return_right_mat(void){ return right_points_mat; }


		//returns if real time or not
		int get_real_time_var(void){ return real_time; };

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
}


#endif // !_CALIBRATION_CLASS_H
