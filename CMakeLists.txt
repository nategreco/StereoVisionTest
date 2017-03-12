#####################################
cmake_minimum_required (VERSION 3.0) 
project (StereoVisionTest)
include_directories(include)
add_compile_options(-std=c++11 -o3)
SET( CMAKE_CXX_FLAGS "-l pthread -l wiringPi -l gps" )
find_package(OpenCV REQUIRED)
find_package(raspicam REQUIRED)
find_package(PkgConfig REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_library(DISPLAY_HANDLER_LIBRARIES src/display_handler.cpp)
add_library(GPIO_HANDLER_LIBRARIES src/gpio_handler.cpp)
add_library(IMAGE_CAPTURER_LIBRARIES src/image_capturer.cpp)
add_library(PACE_SETTER_LIBRARIES src/pace_setter_class.cpp)
add_library(FRAME_QUEUE_LIBRARIES src/frame_queue_class.cpp)
add_library(STORAGE_WORKER_LIBRARIES src/storage_worker_class.cpp)
add_library(VIDEO_WRITER_LIBRARIES src/video_writer.cpp)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable (StereoVisionTest main.cpp)
target_link_libraries(StereoVisionTest
	pthread
	${OpenCV_LIBS}
	${raspicam_CV_LIBS}
	${GTK2_LIBRARIES}
	DISPLAY_HANDLER_LIBRARIES
	GPIO_HANDLER_LIBRARIES
	IMAGE_CAPTURER_LIBRARIES
	PACE_SETTER_LIBRARIES
	VIDEO_WRITER_LIBRARIES
	FRAME_QUEUE_LIBRARIES
	STORAGE_WORKER_LIBRARIES
)
#####################################
