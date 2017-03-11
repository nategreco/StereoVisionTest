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
      - Raspicam***			-> https://github.com/nategreco/raspicam/tree/feature/stereoscopicMode
		***Must use branch after merge request 27 at https://github.com/cedricve/raspicam/pull/27
	  
  License:
	  This software is licensed under GNU GPL v3.0

  History:
      Date         Author      Description
-------------------------------------------------------------------------------------------
      10.03.2016   N. Greco    Initial creation
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
#include "opencv2/calib3d.hpp"
#include "raspicam/raspicam_cv.h"

//StereoVisionTest source files
#include "display_handler.h"
#include "gpio_handler.h"
#include "image_capturer.h"
#include "pace_setter_class.h"
#include "video_writer.h"

int main()
{
	std::cout << "Program launched, starting log file..." << '\n';

	//Create shared resources
	std::atomic<bool> exitsignal{ false };
    cv::Mat captureimage;
	std::mutex capturemutex;
	
	//Start threads

	//Start image capture thread
    std::thread t_imagecapture( CaptureImageThread,
	                            &captureimage,
								&capturemutex,
								&exitsignal );
	//Start display thread
	std::thread t_displayupdate( DisplayUpdateThread,
								 &captureimage,
								 &capturemutex,
								 &exitsignal );
	//Start video writer thread
    std::thread t_videowriter( VideoWriterThread,
							   &captureimage,
							   &capturemutex,
							   &exitsignal );

    //Set pace setter class!
	PaceSetter mypacesetter( 5, "Main" );
	
	//Setup polling

	//GPIO
	bool gpiopoll{ false };
	try {
		gpiopoll = GpioHandlerSetup();
	} catch ( const std::exception& ex ) {
		std::cout << "GPIO handler setup threw exception: "<< ex.what() << '\n';
		gpiopoll = false;
	} catch ( const std::string& str ) {
		std::cout << "GPIO handler setup threw exception: "<< str << '\n';
		gpiopoll = false;
	} catch (...) {
		std::cout << "GPIO handler setup threw exception of unknown type!" << '\n';
		gpiopoll = false;
	}
    
	//Loop
	while( !(*exitsignal) ) {
		if ( (gpiopoll) ) GpioHandler( exitsignal );

		//Set Pace
		mypacesetter.SetPace();
	}

    //Handle all the threads
	t_imagecapture.join();
	t_displayupdate.join();
	t_videowriter.join();

	//Exit
	std::cout << "Program exited gracefully!"  << '\n';

    return 0;
}
