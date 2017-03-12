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
#ifndef GPIO_HANDLER_H_INCLUDED
#define GPIO_HANDLER_H_INCLUDED

//Standard libraries
#include <atomic>

/*****************************************************************************************/
bool GpioHandlerSetup();
void GpioHandler( std::atomic<bool>& exitsignal );

#endif // GPIO_HANDLER_H_INCLUDED
