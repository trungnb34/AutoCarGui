#ifndef STATIC_H
#define STATIC_H

#include <iostream>

#define URL "http://localhost:5000"
#define STOP_STREAM_API "stop_stream"
#define START_STREAM_API "start_stream"
#define GO "Go"
#define STOP "Stop"
#define LOCATION_DIRECTION "locations_direction"
#define MANUAL "Manual"
#define AUTOMATIC "Automatic"
#define STATUS_RUN_ALL_CAR "status_run_all_car"
#define AUTOMATIC_ALL_ROBOT "automatic_all_robot"

struct STATIC_STREAM {
public:
    bool isStream = false;
    std::string url = "";
    bool restart = false;
    static STATIC_STREAM& get_instance() {
        static STATIC_STREAM instance;
        return instance;
    }
};

#endif // STATIC_H
