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
#include <atomic>
#include <stdlib.h>
#include <exception>
#include <string>

//3rd party libraries
#include <wiringPi.h>
#include <wiringPiI2C.h>

//Project libraries
#include "pace_setter_class.h"
#include "process_values_class.h"
#include "xml_reader.h"

//Preprocessor

//Physical pin numbers
#define POWERINPUTPIN 4

/*****************************************************************************************/
bool GpioHandlerSetup()
{
	//Setup GPIO pins
	wiringPiSetupGpio();
	pinMode(POWERINPUTPIN, INPUT); 
	
	return true;
}

void GpioHandler( std::atomic<bool>& exitsignal )
{
	try {

		//Shutdown logic on power loss
		if ( !digitalRead(POWERINPUTPIN) ) {
			exitsignal = true;
			std::cout << "Power loss detected, exiting!" << '\n';
			return;
		}
	} catch ( const std::exception& ex ) {
		std::cout << "GPIO handler thread threw exception: "<< ex.what() << '\n';
	} catch ( const std::string& str ) {
		std::cout << "GPIO handler thread threw exception: "<< str << '\n';
	} catch (...) {
		std::cout << "GPIO handler thread threw exception of unknown type!" << '\n';
	}
	
	return;
}
