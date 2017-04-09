/******************************************************************************************
  Date:    03.10.2016
  Author:  Nathan Greco (Nathan.Greco@gmail.com)

  Project:
      DAPrototype: Driver Assist Prototype
	  http://github.com/NateGreco/StereoVisionTest.git

  License:
	  This software is licensed under GNU GPL v3.0
	  
******************************************************************************************/

//Standard libraries
#include <iostream>
#include <mutex>
#include <atomic>
#include <exception>
#include <string>

//3rd party libraries
#include "opencv2/opencv.hpp"
#include "raspicam/raspicam_cv.h"

//Project libraries
#include "pace_setter_class.h"

/*****************************************************************************************/
void CaptureImageThread( cv::Mat *capture,
                         std::mutex *capturemutex,
                         std::atomic<bool> *exitsignal )
{
	std::cout << "Image capturer thread starting!" << '\n';

    //Create camera
	raspicam::RaspiCam_Cv camera;
	const int kpixwidth{ 896 * 2 };
	const int kpixheight{ 480 };

	//Set properties
	camera.set( CV_CAP_PROP_FRAME_WIDTH, kpixwidth );
	camera.set( CV_CAP_PROP_FRAME_HEIGHT, kpixheight );
	camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
	//Set stereoscopic mode
	if ( !camera.setStereoMode(1) ) {
		std::cerr << "Error setting stereoscopic mode" << '\n';
		exit(-1);
	}
    //Open
	if ( !camera.open() ) {
		std::cerr << "Error opening the camera" << '\n';
		exit(-1);
	}
	std::cout << "Camera opened succesfully!" << '\n';

	//create pace setter
	PaceSetter camerapacer( 10,	"Image capturer" );

	//Loop indefinitely
	while( !(*exitsignal) ) {
		try {
			//Get image
			cv::Mat newimage;
			camera.grab();
			camera.retrieve( newimage );

			//Update common cv::Mat
			capturemutex->lock();
			cv::flip( newimage, *capture, -1 );
			//newimage.copyTo( *capture );
			capturemutex->unlock(); 

			//Set pace
			camerapacer.SetPace();
		} catch ( const std::exception& ex ) {
			std::cout << "Image Capturer thread threw exception: "<< ex.what() << '\n';
		} catch ( const std::string& str ) {
			std::cout << "Image Capturer thread threw exception: "<< str << '\n';
		} catch (...) {
			std::cout << "Image Capturer thread threw exception of unknown type!" << '\n';
		}
	}

	std::cout << "Exiting image capturer thread!" << '\n';
	return;
}
