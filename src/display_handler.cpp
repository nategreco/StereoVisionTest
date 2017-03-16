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

//Project libraries
#include "pace_setter_class.h"

/*****************************************************************************************/
void DisplayUpdateThread( cv::Mat *image,
                          std::mutex *mutex,
	                      std::atomic<bool> *exitsignal )
{
	std::cout << "Display handler thread starting!" << '\n';
	
	//create pace setter
	PaceSetter displaypacer( 5, "display handler" );

	for ( ;; ) {
		//Check if cv::Mat initialized
		mutex->lock();
		if ( !image->empty() ) {
			mutex->unlock();
			break;
		}
		mutex->unlock();
		
		//Check for program exit
		if (*exitsignal) {
			return;
		}
		displaypacer.SetPace();			
	}	

	//Create initial cv::Mat
	mutex->lock();
	cv::Mat imagetemp{ 480, 800, image->type(), cv::Scalar(0) };
	const int kcolstart{ ((image->cols / 2) - imagetemp.cols) / 2 };
	mutex->unlock();

	//Initialize display with first image
	std::cout << "Attempting to open display..." << '\n';
	cv::namedWindow( "Output", cv::WINDOW_NORMAL );
	cv::setWindowProperty( "Output", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN );
	std::cout << "Display opened!" << '\n';
	
	//Loop
	while( !(*exitsignal) ) {
		try {
			//Get latest image
			mutex->lock();
			cv::Mat newimage{ image->clone() };
			mutex->unlock();
			newimage( cv::Rect(kcolstart, 0, imagetemp.cols, imagetemp.rows) ).copyTo(
				imagetemp(cv::Rect(0, 0, imagetemp.cols, imagetemp.rows)));
			cv::imshow( "Output", imagetemp );
			cv::waitKey( 1 );
			
			//Set pace
			displaypacer.SetPace();
		} catch ( const std::exception& ex ) {
			std::cout << "Display handler thread threw exception: "<< ex.what() << '\n';
		} catch ( const std::string& str ) {
			std::cout << "Display handler thread threw exception: "<< str << '\n';
		} catch (...) {
			std::cout << "Display handler thread threw exception of unknown type!" << '\n';
		}
	}
	
	std::cout << "Exiting display handler thread!" << '\n';
	return;
}
