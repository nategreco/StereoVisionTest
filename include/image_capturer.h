/******************************************************************************************
  Date:    03.10.2016
  Author:  Nathan Greco (Nathan.Greco@gmail.com)

  Project:
      DAPrototype: Driver Assist Prototype
	  http://github.com/NateGreco/StereoVisionTest.git

  License:
	  This software is licensed under GNU GPL v3.0
	  
******************************************************************************************/

//Header guard
#ifndef IMAGE_CAPTURER_H_INCLUDED
#define IMAGE_CAPTURER_H_INCLUDED

//Standard libraries
#include <mutex>
#include <atomic>

//3rd party libraries
#include "opencv2/opencv.hpp"

/*****************************************************************************************/
void CaptureImageThread( cv::Mat *capture,
						 std::mutex *capturemutex,
						 std::atomic<bool> *exitsignal);

#endif // IMAGE_CAPTURER_H_INCLUDED
