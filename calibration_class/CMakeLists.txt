ADD_LIBRARY(camera_calib_lib source/calibration_class.cpp)

target_link_libraries(camera_calib_lib ${OpenCV_LIBS})