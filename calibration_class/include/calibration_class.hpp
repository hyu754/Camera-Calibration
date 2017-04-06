#ifndef _CALIBRATION_CLASS_H
#define _CALIBRATION_CLASS_H

using namespace cv;
using namespace std;

namespace calibration{
	class calibration_geometry;
	enum direction_t{ left, right, mono_view };//The different calibration types
	enum calibration_t{ mono, stereo };//
}

class calibration::calibration_geometry
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

		void increase_sucess(void){ numSuccess++; std::cout <<"Number of successful calibrations : "<< numSuccess << std::endl; }
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

		//information regarding reading images
		//Get file prefix and variable
		std::string left_prefix, right_prefix;
		int begin_suffix, end_suffix;
		std::string image_type;

		//function to check if file is still in range
		bool file_in_range(void);


		//read in images from file
		void read_in_images(void);

		//read in images from stereo camera
		void read_in_images_camera(cv::Mat image_left_in, cv::Mat image_right_in);

		//display the images read from file
		void display_images(void);

		//getting address of images
		cv::Mat* get_image_pointer(direction_t in_d){			if (in_d == direction_t::left){				return &image_left;			}			else {				return &image_right;			}		}

		//calibrate the stereo camera intrinsics

		void calibrate_stereo_intrinsics(void);

		//Find stereo calibration parameters
		void calibrate_stereo_parameters(void);

		//write camera parameters for stereo case
		void write_camera_properties_stereo(void);
	protected:
		cv::Mat intrinsic_matrix[2];
		cv::Mat distortion_vector[2];

		//Stereo calibration matrices
		cv::Mat  R, T, E, F;
		cv::Mat  R1, R2, P1, P2, Q;
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

		//file counter 
		int file_counter = 0;
	};


#endif // !_CALIBRATION_CLASS_H
