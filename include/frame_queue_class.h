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
#ifndef FRAMEQUEUE_H
#define FRAMEQUEUE_H

//Standard libraries
#include <mutex>
#include <queue>

//3rd party libraries
#include "opencv2/opencv.hpp"

/*****************************************************************************************/
class FrameQueue
{
	public:
		FrameQueue();
		struct cancelled {};
		void Push( cv::Mat const& image );
		cv::Mat Pop();
		void Cancel();
		void Stop();
		void Restart();
		bool CheckCanceled();
		bool CheckPaused();
		bool CheckReleased();
		void ReleaseFile();
		FrameQueue& operator= (const FrameQueue &q);
		virtual ~FrameQueue();

	protected:

	private:
		std::queue<cv::Mat> queue_;
		std::mutex mutex_;
		bool canceled_;
		bool paused_;
		bool released_;
};

#endif // FRAMEQUEUE_H
