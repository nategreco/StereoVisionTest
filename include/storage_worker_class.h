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
#ifndef STORAGEWORKER_H
#define STORAGEWORKER_H

//3rd party libraries
#include "opencv2/opencv.hpp"

//Project libraries
#include "frame_queue_class.h"

/*****************************************************************************************/
class StorageWorker
{
	public:
		StorageWorker( FrameQueue& queue,
					   int32_t id,
					   std::string const& file_name,
					   int32_t fourcc,
					   double fps,
					   cv::Size frame_size,
					   bool is_color = true);
		void Run();
		virtual ~StorageWorker();

	protected:

	private:
		FrameQueue& queue_;
		int32_t id_;
		std::string file_name_;
		int32_t fourcc_;
		double fps_;
		cv::Size frame_size_;
		bool is_color_;

};

#endif // STORAGEWORKER_H