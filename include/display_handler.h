/******************************************************************************************
  Date:    12.08.2016
  Author:  Nathan Greco (Nathan.Greco@gmail.com)

  Project:
      DAPrototype: Driver Assist Prototype
	  http://github.com/NateGreco/DAPrototype.git

  License:
	  This software is licensed under GNU GPL v3.0
	  
******************************************************************************************/

//Header guard
#ifndef DISPLAY_HANDLER_H_INCLUDED
#define DISPLAY_HANDLER_H_INCLUDED

//Standard libraries
#include <mutex>
#include <atomic>

//3rd party libraries
#include "opencv2/opencv.hpp"

/*****************************************************************************************/
void DisplayUpdateThread( cv::Mat *image,
						  std::mutex *displaymutex,
						  std::atomic<bool> *exitsignal);

#endif // DISPLAY_HANDLER_H_INCLUDED
