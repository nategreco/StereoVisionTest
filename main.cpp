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
#include "raspicam/raspicam_cv.h"

//DAPrototype source files
#include "pace_setter_class.h"

int main()
{
	const int kpixwidth{ 768 };		//Must be multiple of 128!
	const int kpixheight{ 480 };
	std::cout << "Program launched, capture image resolution will be " << kpixwidth <<
				 "x" << kpixheight << '\n';

	//Create camera 0
	std::cout << "Creating camera 0..." << '\n';
	raspicam::RaspiCam_Cv camera0;
	//Set properties
	camera0.set( CV_CAP_PROP_FRAME_WIDTH, kpixwidth );
	camera0.set( CV_CAP_PROP_FRAME_HEIGHT, kpixheight );
	camera0.set( CV_CAP_PROP_FORMAT, CV_8UC3 );
	//Create window
	cv::namedWindow( "Output0", cv::WINDOW_NORMAL );
	//Validate open
	if ( !camera0.open() ) {
		std::cerr << "Error opening the camera 0" << '\n';
		exit(-1);
	}
	
	//Set stereoscopic mode
	camera0.setStereoMode(1, true, false);  //Need to get mmal enum's! 1 = side by side
/*
	//Create camera 1
	std::cout << "Creating camera 1..." << '\n';
	raspicam::RaspiCam_Cv camera1;
	//Set properties
	camera1.set( CV_CAP_PROP_FRAME_WIDTH, kpixwidth );
	camera1.set( CV_CAP_PROP_FRAME_HEIGHT, kpixheight );
	camera1.set( CV_CAP_PROP_FORMAT, CV_8UC3 );
	//Create window
	cv::namedWindow( "Output1", cv::WINDOW_NORMAL );
	//Validate open
	if ( !camera1.open( 1 ) ) {
		std::cerr << "Error opening the camera 1" << '\n';
		exit(-1);
	}
*/
	//Create disparity window
	//cv::namedWindow( "Output2", cv::WINDOW_NORMAL );

	//Create pace setter to maintain FPS
	PaceSetter camerapacer( 5, "Main thread" );
	
	//Loop indefinitely
	for ( ;; ) {
		//Grab images - Will need to make simultaneous threads in future synchronization
		camera0.grab();
		//camera1.grab();

		//Copy to cv::Mat
		cv::Mat image0;
		//cv::Mat image1;
		camera0.retrieve( image0 );
		//camera1.retrieve( image1 );
		
		//Create disparity map
		//v::StereoBM* stereobm = cv::createStereoBM();
		//cv::Mat disparity;
		//stereobm->compute( image0, image1, disparity );
		
		//Update windows
		cv::imshow( "Output0", image0 );
		//cv::imshow( "Output1", image1 );
		//cv::imshow( "Output2", disparity );
		cv::waitKey( 1 );
	
		//Set pace
		camerapacer.SetPace();
	}

    return 0;
}
