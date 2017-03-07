/******************************************************************************************
  Date:    12.08.2016
  Author:  Nathan Greco (Nathan.Greco@gmail.com)

  Project:
      DAPrototype: Driver Assist Prototype
	  http://github.com/NateGreco/DAPrototype.git

  License:
	  This software is licensed under GNU GPL v3.0
	  
******************************************************************************************/

//Standard libraries
#include <iostream>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <queue>
#include <chrono>
#include <exception>
#include <string>

//3rd party libraries
#include "opencv2/opencv.hpp"

//Project libraries
#include "video_writer.h"
#include "pace_setter_class.h"

/*****************************************************************************************/
void VideoWriterThread ( cv::Mat *orgimage,
                         std::mutex *capturemutex,
                         cv::Mat *modimage,
                         std::mutex *displaymutex,
                         std::atomic<bool> *exitsignal )
{

	std::cout << "Video writer thread starting!" << '\n';
	
	//Create pace setter
	PaceSetter videopacer(settings::cam::krecfps, "video writer");
	
	//Check image is initialized
	if ( settings::cam::krecordorgimage ) {
		while ( orgimage->empty() ) {
			if (*exitsignal) {
				return;
			}
			videopacer.SetPace();			
		}		
	} else {
		while ( modimage->empty() ) {
			if (*exitsignal) {
				return;
			}
			videopacer.SetPace();
		}		
	}

	std::string filepath;
	cv::Size size;

	if ( settings::cam::krecordorgimage ) {
		filepath = settings::cam::kfilepath + settings::cam::korgfilename;
		size = cv::Size(orgimage->cols, orgimage->rows);
	} else {
		filepath = settings::cam::kfilepath + settings::cam::kmodfilename;
		size = cv::Size(modimage->cols, modimage->rows);
	}
				
	//Shift files
	fileShift(filepath, settings::cam::kfilestokeep);
	
    FrameQueue queue;
	StorageWorker storage{ queue,
			               1,
			               filepath,
						   CV_FOURCC('D', 'I', 'V', 'X'),
						   static_cast<double>(settings::cam::krecfps),
						   size,
						   true };

    // And start the worker threads for each storage worker
    std::thread t_storagethread(&StorageWorker::Run, &storage);
	
	//Create thread variables
	std::chrono::high_resolution_clock::time_point startime{
		std::chrono::high_resolution_clock::now() };
	int32_t filelengthseconds{ 60 * settings::cam::kminperfile };

	//Loop
	while( !(*exitsignal) ) {
		try {
			//Normal Execution
			if ( settings::cam::krecordorgimage ) {
				capturemutex->lock();
				queue.Push(orgimage->clone());
				capturemutex->unlock();
			} else {
				displaymutex->lock();
				queue.Push(modimage->clone());	
				displaymutex->unlock();
			}
			
			//New file changeover
			if (static_cast<long>(std::chrono::duration_cast<std::chrono::seconds>(
				std::chrono::high_resolution_clock::now() - startime).count()) > filelengthseconds) {

				//Stop thread
				queue.Stop();
				
				while (!queue.CheckReleased()) {
					//Wait
					videopacer.SetPace();
				}
				
				//Shift files
				fileShift(filepath, settings::cam::kfilestokeep);
				
				//Restart thread
				queue.Restart();

				//Restart timer
				startime = std::chrono::high_resolution_clock::now();
			}
					
			videopacer.SetPace();
		} catch ( const std::exception& ex ) {
			std::cout << "Video Writer thread threw exception: "<< ex.what() << '\n';
		} catch ( const std::string& str ) {
			std::cout << "Video Writer thread threw exception: "<< str << '\n';
		} catch (...) {
			std::cout << "Video Writer thread threw exception of unknown type!" << '\n';
		}
	}

    queue.Cancel();
	t_storagethread.join();
	
	std::cout << "Exiting video writer thread!" << '\n';
	return;
}

/*****************************************************************************************/
int fileShift( std::string filename,
               int numOfFiles )
{
    int i(numOfFiles), j(0), k(0);
    //split extension
    k = filename.find_last_of(".");
    //delete last file if present
    remove(((filename.substr(0,k) + "_" + std::to_string(i)) + filename.substr(k,filename.length() - k)).c_str());
    //rename existing files
    while(i != 1)
    {
        std::string newName = filename.substr(0,k) + "_" + std::to_string(i) + filename.substr(k,filename.length() - k);
        std::string oldName = filename.substr(0,k) + "_" + std::to_string(--i) + filename.substr(k,filename.length() - k);
        j = j + rename(oldName.c_str(), newName.c_str());
    }
    std::string newName = filename.substr(0,k) + "_" + std::to_string(i--) + filename.substr(k,filename.length() - k);
    j = j + i + rename(filename.c_str(), newName.c_str());
    return j;
}
