#include "videoreader.h"
#include <QThread>

VideoReader::VideoReader(cv::Mat &_frame, std::mutex &_mutex) : frame(_frame), mutex(_mutex) {}

void VideoReader::read_source() {
    // std::cout << "VAO DAY" << std::endl;
    while (true) {
        // std::cout << "VAO DAY" << std::endl;
        if (! STATIC_STREAM::get_instance().url.length() && !STATIC_STREAM::get_instance().isStream) {
            QThread::sleep(3);
            continue;
        }
        cap = cv::VideoCapture(STATIC_STREAM::get_instance().url);
        if (!cap.isOpened()) {
            QThread::sleep(3);
            continue;
        }

        int fps = cap.get(cv::CAP_PROP_FPS);
        // std::cout << "fps : " << fps << std::endl;
        int index = 0;
        while (true) {
            // mutex.lock();
            cap.read(frame);
            // mutex.unlock();
            if (frame.empty()) {
                break;
            }
            // std::cout << "index : " << index ++ << std::endl;
            // std::cout << "frame : " << frame.size() << std::endl;
            if (!STATIC_STREAM::get_instance().isStream || STATIC_STREAM::get_instance().restart) {
                STATIC_STREAM::get_instance().restart = false;
                break;
            }
        }
        // cap.release();
        // std::cout << "AAAAA" << std::endl;
    }
}
