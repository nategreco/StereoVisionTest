/******************************************************************************************
  Date:    12.08.2016
  Author:  Nathan Greco (Nathan.Greco@gmail.com)

  Project:
      StereoVisionTest: Raspberry Pi Stereo Vision
	  http://github.com/NateGreco/DAPrototype.git

  License:
	  This software is licensed under GNU GPL v3.0
	  
******************************************************************************************/

//Standard libraries
#include <iostream>
#include <algorithm>
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
	raspicam::RaspiCam_Cv Camera;

	//Set properties
	Camera.set( CV_CAP_PROP_FRAME_WIDTH, settings::cam::kpixwidth );
	Camera.set( CV_CAP_PROP_FRAME_HEIGHT, settings::cam::kpixheight );
	Camera.set( CV_CAP_PROP_FORMAT, CV_8UC3 );

    //Open
	if ( !Camera.open() ) {
		std::cerr << "Error opening the camera" << '\n';
		exit(-1);
	}
	std::cout << "Camera opened succesfully!" << '\n';

	//create pace setter
	PaceSetter camerapacer( std::max(std::max(settings::disp::kupdatefps,
											  settings::cam::krecfps),
									 settings::ldw::kupdatefps),
							"Image capturer");

	//Loop indefinitely
	while( !(*exitsignal) ) {
		try {
			cv::Mat newimage;
			Camera.grab();
			Camera.retrieve( newimage );
			cv::flip( newimage, newimage, -1 );
			//resize image
			if ( newimage.rows != settings::cam::kpixheight ) {
				cv::resize( newimage,
							newimage,
							cv::Size(settings::cam::kpixwidth,
									 settings::cam::kpixheight) );
			}
			
			capturemutex->lock();
			*capture = newimage;
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
