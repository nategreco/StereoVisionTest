/******************************************************************************************
  Date:    12.08.2016
  Author:  Nathan Greco (Nathan.Greco@gmail.com)

  Project:
      StereoVisionTest: Raspberry Pi Stereo Vision
	  http://github.com/NateGreco/StereoVisionTest.git

  Description:
      This project is for testing stereo vision on the Raspberry Pi compute module 3.
	  Requires modified raspicam libraries (see other repository) to allow use of second
	  CSI port.

  Target Hardware:
      - Raspberry Pi compute module 3
	  - RPi (v2.1) 8MP camera - camera 1
	  - RPi (v2.1) 8MP camera - camera 2
	  - 3D printed case with 65mm focal spacing (see models/StereoVisionTest.stl)

  Target Software platform:
      Debian disto (DietPi) running LDXE, with below libraries installed

  3rd Party Libraries:
      - OpenCV 3.1.0		-> http://www.opencv.org *Compiled with OpenGL support
      - Raspicam***			-> https://github.com/nategreco/raspicam
		***Must use modified library that allows assignment of CSI port
	  
  License:
	  This software is licensed under GNU GPL v3.0

  History:
      Date         Author      Description
-------------------------------------------------------------------------------------------
      12.08.2016   N. Greco    Initial creation
******************************************************************************************/


//Standard libraries
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <thread>
#include <cstdlib>
#include <mutex>
#include <atomic>

//3rd party libraries
#include "opencv2/opencv.hpp"
#include "calib3d.hpp"
#include "raspicam/raspicam_cv.h"

//DAPrototype source files
#include "pace_setter_class.h"

int main()
{
	const int kpixwidth{ 800 };
	const int kpixheight{ 480 * 2 };
	std::cout << "Program launched, capture image resolution will be " << kpixwidth <<
				 "x" << kpixheight << '\n';

	//Create camera 0
	std::cout << "Creating camera 0..." << '\n';
	raspicam::RaspiCam_Cv camera0;
	//Set properties
	camera0.set( CV_CAP_PROP_FRAME_WIDTH, kpixwidth );
	camera0.set( CV_CAP_PROP_FRAME_HEIGHT, kpixheight );
	camera0.set( CV_CAP_PROP_FORMAT, CV_8UC3 );
	//Set stereoscopic mode
	if ( !camera0.setStereoMode(2) ) {
		std::cerr << "Error setting stereoscopic mode" << '\n';
		exit(-1);
	}
	//Create window
	cv::namedWindow( "Original", cv::WINDOW_NORMAL );
	//Validate open
	if ( !camera0.open() ) {
		std::cerr << "Error opening the camera 0" << '\n';
		exit(-1);
	}

	//Create disparity window
	cv::namedWindow( "Disparity", cv::WINDOW_NORMAL );

	//Create pace setter to maintain FPS
	PaceSetter camerapacer( 5, "Main thread" );
	
	//Loop indefinitely
	for ( ;; ) {
		//Grab stereo image
		camera0.grab();

		//Copy to cv::Mat
		cv::Mat image;
		camera0.retrieve( image );
		
		//Split image
		cv::Mat left, right;
		image( cv::Rect(0,
						kpixheight / 2,
						image.cols,
						image.rows -
						kpixheight / 2) ).copyTo(left(cv::Rect(0,
															   kpixheight / 2,
															   image.cols,
															   image.rows -
															   kpixheight / 2)));
		image( cv::Rect(0,
						kpixheight / 2,
						image.cols,
						image.rows -
						kpixheight / 2) ).copyTo(right(cv::Rect(0,
															    kpixheight / 2,
															    image.cols,
															    image.rows -
															    kpixheight / 2)));
		
		//Create disparity map
		cv::StereoBM* stereobm;
		stereobm->create();
		cv::Mat disparity;
		
		
		stereobm->compute( left, right, disparity );
		
		//Update windows
		cv::imshow( "Original", image );
		cv::imshow( "Disparity", disparity );
		cv::waitKey( 1 );
	
		//Set pace
		camerapacer.SetPace();
	}

    return 0;
}
