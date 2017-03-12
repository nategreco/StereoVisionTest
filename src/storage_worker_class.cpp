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
#include <condition_variable>
#include <mutex>
#include <queue>

//3rd party libraries
#include "opencv2/opencv.hpp"

//Project libraries
#include "storage_worker_class.h"

/*****************************************************************************************/
StorageWorker::StorageWorker( FrameQueue& queue,
							  int32_t id,
							  std::string const& file_name,
							  int32_t fourcc,
							  double fps,
							  cv::Size frame_size,
							  bool is_color) :
							  queue_(queue),
							  id_(id),
							  file_name_(file_name),
							  fourcc_(fourcc),
							  fps_(fps),
							  frame_size_(frame_size),
							  is_color_(is_color)
{
	
}

void StorageWorker::Run()
{
    cv::VideoWriter writer(file_name_, fourcc_, fps_, frame_size_, is_color_);
	
	//queue_.Reset();
	
	while ( !queue_.CheckCanceled() ) {
		if ( !queue_.CheckPaused() ) {
			cv::Mat image(queue_.Pop());
			if ( !image.empty() ) {
				writer.write(image);
			}
		} else {
			writer.release();
			queue_.ReleaseFile();
			while ( queue_.CheckPaused() ) {}
			writer.open(file_name_, fourcc_, fps_, frame_size_, is_color_);
		}
	}
	writer.release();
}

StorageWorker::~StorageWorker()
{

}
