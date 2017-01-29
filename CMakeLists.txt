#####################################
cmake_minimum_required (VERSION 3.0) 
project (StereoVisionTest)
include_directories(include)
add_compile_options(-std=c++11 -o3)
find_package(OpenCV REQUIRED)
find_package(raspicam REQUIRED)
find_package(PkgConfig REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_library(PACE_SETTER_LIBRARIES src/pace_setter_class.cpp)
add_executable (StereoVisionTest main.cpp)
target_link_libraries(StereoVisionTest
	${OpenCV_LIBS}
	${raspicam_CV_LIBS}
	PACE_SETTER_LIBRARIES
)
#####################################
