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
#include "frame_queue_class.h"

/*****************************************************************************************/
FrameQueue::FrameQueue() : canceled_(false)
{
	
}

void FrameQueue::Cancel()
{
    mutex_.lock();
    canceled_ = true;
    mutex_.unlock();
}

void FrameQueue::Stop()
{
    mutex_.lock();
    paused_ = true;
    mutex_.unlock();
}

void FrameQueue::Restart()
{
    mutex_.lock();
    paused_ = released_ = false;
    mutex_.unlock();
}

bool FrameQueue::CheckCanceled()
{
    mutex_.lock();
    bool canceledstatus{canceled_};
    mutex_.unlock();
	return canceledstatus;
}

bool FrameQueue::CheckPaused()
{
    mutex_.lock();
    bool pausedstatus{paused_};
    mutex_.unlock();
	return pausedstatus;
}

bool FrameQueue::CheckReleased()
{
    mutex_.lock();
    bool releasedstatus{released_};
    mutex_.unlock();
	return releasedstatus;
}

void FrameQueue::ReleaseFile()
{
    mutex_.lock();
    released_ = true;
    mutex_.unlock();
	return;
}

void FrameQueue::Push(cv::Mat const& image)
{
    mutex_.lock();
    queue_.push(image);
    mutex_.unlock();
}

cv::Mat FrameQueue::Pop()
{
	cv::Mat image;
    while ( queue_.empty() ) {
        if ( canceled_ || paused_ ) {
            return image;
        }
    }

	mutex_.lock();
    image = queue_.front();
    queue_.pop();
	mutex_.unlock();
    return image;
}

FrameQueue::~FrameQueue()
{

}
