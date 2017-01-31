cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories(${CMAKE_SOURCE_DIR}/calibration_class/include/)
add_executable( Calibration camera_calibration.cpp )



add_subdirectory(calibration_class)

target_link_libraries( Calibration ${OpenCV_LIBS} camera_calib_lib )

#set this to your opencv version and change depending if debug or release
SET(CV_VERSION 310)
SET(CV_FILE_PRE ${CV_VERSION}d.dll)


add_custom_target(dll_includes
	
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/bin/opencv_core${CV_FILE_PRE}            ${CMAKE_BINARY_DIR}/Debug/
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/bin/opencv_highgui${CV_FILE_PRE}            ${CMAKE_BINARY_DIR}/Debug/
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/bin/opencv_imgcodecs${CV_FILE_PRE}            ${CMAKE_BINARY_DIR}/Debug/
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/bin/opencv_imgproc${CV_FILE_PRE}            ${CMAKE_BINARY_DIR}/Debug/
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/bin/opencv_videoio${CV_FILE_PRE}            ${CMAKE_BINARY_DIR}/Debug/
COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/bin/opencv_calib3d${CV_FILE_PRE}            ${CMAKE_BINARY_DIR}/Debug/
COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/bin/opencv_features2d${CV_FILE_PRE}            ${CMAKE_BINARY_DIR}/Debug/
COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/bin/opencv_flann${CV_FILE_PRE}            ${CMAKE_BINARY_DIR}/Debug/



)

add_dependencies(Calibration dll_includes)