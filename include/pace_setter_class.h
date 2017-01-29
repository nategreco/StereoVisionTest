/******************************************************************************************
  Date:    28.01.2017
  Author:  Nathan Greco (Nathan.Greco@gmail.com)

  Project:
      DAPrototype: Driver Assist Prototype
	  http://github.com/NateGreco/StereoVisionTest.git

  License:
	  This software is licensed under GNU GPL v3.0
	  
******************************************************************************************/

//Header guard
#ifndef PACESETTER_H
#define PACESETTER_H

//Standard libraries
#include <chrono>
#include <string>

/*****************************************************************************************/
class PaceSetter
{
    public:
        PaceSetter( int fps,
					std::string threadname );
        void SetPace();
        virtual ~PaceSetter();

    protected:

    private:
        long targetusec_;
		long sleeptime_;
		int missedcount_;
		const std::string threadname_;
        std::chrono::high_resolution_clock::time_point lastsettime_;
};

#endif // PACESETTER_H
