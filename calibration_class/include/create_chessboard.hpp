#ifndef _CREATE_CHESSBOARD
#define _CREATE_CHESSBOARD



//Adapted from http://answers.opencv.org/users/1099/haris/

void generate_chess_board( int num_h,int num_v,std::string file_name){
	int blockSize=200;

	int _num_vert = num_v;
	int _num_horz = num_h;
	int imageSizey = blockSize*_num_vert;
	int imageSizex = blockSize*_num_horz;
	Mat chessBoard(imageSizey, imageSizex, CV_8UC3, Scalar::all(0));
    unsigned char color=0;

	for (int i = 0; i<imageSizex; i = i + blockSize){
      color=~color;
	  for (int j = 0; j<imageSizey; j = j + blockSize){
       Mat ROI=chessBoard(Rect(i,j,blockSize,blockSize));
       ROI.setTo(Scalar::all(color));
       color=~color;
      }
     }
    imshow("Chess board", chessBoard);

	cv::imwrite(file_name+".jpg",chessBoard);
	cv::waitKey();
}



#endif // !_CREATE_CHESSBOARD
