/******************************************************************************************
  Date:    28.01.2017
  Author:  Nathan Greco (Nathan.Greco@gmail.com)

  Project:
      StereoVisionTest: Raspberry Pi Stereo Vision
	  http://github.com/NateGreco/StereoVisionTest.git

  License:
	  This software is licensed under GNU GPL v3.0
	  
******************************************************************************************/

//Standard libraries
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sys/time.h>

//Project libraries
#include "pace_setter_class.h"

/*****************************************************************************************/
PaceSetter::PaceSetter( int fps,
						std::string threadname ) :
						threadname_{ threadname },
						missedcount_{0}
{
    targetusec_ = ( 1000000 / fps );
    lastsettime_ =  std::chrono::high_resolution_clock::now();
}

void PaceSetter::SetPace()
{
	sleeptime_ = targetusec_ -
				 std::chrono::duration_cast<std::chrono::microseconds>(
				 std::chrono::high_resolution_clock::now() - lastsettime_).count();
	if ( sleeptime_ < 0 ) {
		missedcount_++;
		if ( missedcount_%30 == 0) {
			std::cout << "Thread " << threadname_ << " is not meeting pace!";
			std::cout << " Only performing at: " << std::fixed << std::setprecision(1);
			std::cout << (1000000.0 / (targetusec_ - sleeptime_)) << " fps!" << '\n';			
		}
	} else {
		std::this_thread::sleep_for(std::chrono::microseconds(sleeptime_));		
	}
    lastsettime_ =  std::chrono::high_resolution_clock::now();
}

PaceSetter::~PaceSetter()
{

}
