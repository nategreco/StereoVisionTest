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
#include "frame_queue_class.h"
#include "storage_worker_class.h"
#include "pace_setter_class.h"

/*****************************************************************************************/
void VideoWriterThread ( cv::Mat *image,
                         std::mutex *mutex,
                         std::atomic<bool> *exitsignal )
{

	std::cout << "Video writer thread starting!" << '\n';
	
	//Create pace setter
	const int krecfps{ 10 };
	PaceSetter videopacer( krecfps, "video writer" );

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
		videopacer.SetPace();			
	}		

	//Set path and size
	std::string filepath{ "/stereo.avi" };
	mutex->lock();
	cv::Size size{ image->cols, image->rows };
	mutex->unlock();
		
	//Shift files
	int filestokeep{ 20 };
	fileShift(filepath, filestokeep);
	
    FrameQueue queue;
	StorageWorker storage{ queue,
			               1,
			               filepath,
						   CV_FOURCC('D', 'I', 'V', 'X'),
						   static_cast<double>(krecfps),
						   size,
						   false };

    // And start the worker threads for each storage worker
    std::thread t_storagethread(&StorageWorker::Run, &storage);
	
	//Create thread variables
	std::chrono::high_resolution_clock::time_point startime{
		std::chrono::high_resolution_clock::now() };
	int32_t filelengthseconds{ 60 * 5 };

	//Loop
	while( !(*exitsignal) ) {
		try {
			//Normal Execution
			mutex->lock();
			queue.Push( image->clone() );
			mutex->unlock();
			
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
				fileShift(filepath, filestokeep);
				
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
