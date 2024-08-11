#include "socketio.h"
#include <iostream>
#include <QString>
#include "static.h"


#ifdef WIN32
#define BIND_EVENT(IO,EV,FN) \
do{ \
        socket::event_listener_aux l = FN;\
        IO->on(EV,l);\
} while(0)

#else
#define BIND_EVENT(IO,EV,FN) \
IO->on(EV,FN)
#endif


SocketIO::SocketIO() {
    connectToServer();
}

void SocketIO::connectToServer() {

    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    using std::placeholders::_4;

    // std::string host = "http://localhost:5000";
    client = new sio::client();
    client->connect(URL);

    socket::ptr sock = client->socket();

    client->set_socket_open_listener(std::bind(&SocketIO::OnConnected,this,std::placeholders::_1));
    client->set_close_listener(std::bind(&SocketIO::OnClosed,this,_1));
    client->set_fail_listener(std::bind(&SocketIO::OnFailed,this));


    BIND_EVENT(sock,"stop_stream",std::bind(&SocketIO::onStopStream,this,_1,_2,_3,_4));
    BIND_EVENT(sock,"start_stream",std::bind(&SocketIO::onStartStream,this,_1,_2,_3,_4));
    BIND_EVENT(sock,"receive_signal",std::bind(&SocketIO::onSignal,this,_1,_2,_3,_4));
    BIND_EVENT(sock, "update_location_robot", std::bind(&SocketIO::onUpdateRobotLocation,this,_1,_2,_3,_4));
}


void SocketIO::onStopStream(std::string const& name,message::ptr const& data,bool hasAck,message::list &ack_resp)
{
    Q_UNUSED(name);
    Q_UNUSED(hasAck);
    Q_UNUSED(data);
    Q_UNUSED(ack_resp);
    std::cout << "VAO DAY stop stream" << std::endl;
}

void SocketIO::onUpdateRobotLocation(std::string const& name,message::ptr const& data,bool hasAck,message::list &ack_resp) {
    Q_UNUSED(ack_resp);
    Q_UNUSED(name);
    Q_UNUSED(hasAck);
    // std::cout << "hear" << std::endl;
    if(data->get_flag() == message::flag_object)
    {
        std::string robotID = data->get_map()["robot_id"]->get_string();
        std::string lng = std::to_string(data->get_map()["lat"]->get_double());
        std::string lat = std::to_string(data->get_map()["lng"]->get_double());
        if (robotLocations.contains(robotID)) {
            nlohmann::json dLocation;
            dLocation["location"] = {lat, lng};
            robotLocations[robotID] = dLocation;
        } else {
            nlohmann::json dLocation;
            dLocation["location"] = {lat, lng};
            robotLocations[robotID] = dLocation;
        }
    }
    else {
        std::cout << "error" << std::endl;
    }
}

nlohmann::json SocketIO::getRobotLocations() {
    nlohmann::json tmp = this->robotLocations;
    return tmp;
}

void SocketIO::onStartStream(std::string const& name,message::ptr const& data,bool hasAck,message::list &ack_resp)
{
    Q_UNUSED(ack_resp);
    Q_UNUSED(name);
    Q_UNUSED(hasAck);
    if(data->get_flag() == message::flag_object)
    {
        std::string msg = data->get_map()["stream"]->get_string();
        std::cout << "msg : " << msg << std::endl;
        STATIC_STREAM::get_instance().isStream = true;
        STATIC_STREAM::get_instance().url = msg;
        STATIC_STREAM::get_instance().restart = true;
    } else {
        std::cout << "error" << std::endl;
    }
}


void SocketIO::onSignal(std::string const& name,message::ptr const& data,bool hasAck,message::list &ack_resp)
{
    Q_UNUSED(name);
    Q_UNUSED(hasAck);
    Q_UNUSED(data);
    Q_UNUSED(ack_resp);
}

void SocketIO::OnConnected(std::string const& nsp)
{
    Q_UNUSED(nsp);
    std::cout << "Connected to the server" << std::endl;
}

void SocketIO::OnClosed(client::close_reason const& reason)
{
    // Q_EMIT RequestToggleInputs(false);
    Q_UNUSED(reason);
    std::cout << "Connection closed" << std::endl;
}

void SocketIO::OnFailed()
{
    // Q_EMIT RequestToggleInputs(false);
    std::cout << "Connection failed" << std::endl;
}



void SocketIO::init(std::string _label) {
    this->label = _label;
}


void SocketIO::emitSocket(std::string _nameSocket, std::string _data) {
    client->socket()->emit(_nameSocket, _data);
}
