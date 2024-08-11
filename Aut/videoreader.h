#ifndef VIDEOREADER_H
#define VIDEOREADER_H

#include <iostream>
#include <QObject>
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/opencv.hpp>
#include "static.h"
#include <mutex>

class VideoReader : public QObject
{
    Q_OBJECT
private:
    cv::VideoCapture cap;
    cv::Mat &frame;
    std::mutex &mutex;
public:
    VideoReader(cv::Mat &_frame, std::mutex &_mutex);
public Q_SLOTS:
    void read_source();
};

#endif // VIDEOREADER_H
