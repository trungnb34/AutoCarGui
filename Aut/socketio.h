#ifndef SOCKETIO_H
#define SOCKETIO_H

#include <sio_client.h>
#include <iostream>
#include "static.h"
#include "json.hpp"

using namespace sio;

class SocketIO
{
private:
    std::string label;
    sio::client *client;
    nlohmann::json robotLocations;
private:

    void onStopStream(std::string const& name,message::ptr const& data,bool hasAck,message::list &ack_resp);
    void onStartStream(std::string const& name,message::ptr const& data,bool hasAck,message::list &ack_resp);
    void onSignal(std::string const& name,message::ptr const& data,bool hasAck,message::list &ack_resp);
    void onUpdateRobotLocation(std::string const& name,message::ptr const& data,bool hasAck,message::list &ack_resp);

    void OnConnected(std::string const& nsp);
    void OnClosed(client::close_reason const& reason);
    void OnFailed();
public:

    static SocketIO& get_instance() {
        static SocketIO instance;
        return instance;
    }
    SocketIO();
    void init(std::string _label);
    void emitSocket(std::string nameSocket, std::string data);
    void connectToServer();
    nlohmann::json getRobotLocations();
    ~ SocketIO() {
        delete client;
    }
};

#endif // SOCKETIO_H
