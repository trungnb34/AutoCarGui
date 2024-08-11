#ifndef LOADROBOT_H
#define LOADROBOT_H

#include <QThread>
#include <QObject>
#include <iostream>
#include <vector>

class LoadRobot : public QObject
{
    Q_OBJECT
private:
    std::vector<std::string> &robots;
public:
    LoadRobot(std::vector<std::string> &_robots);
// protected:
//     void run() override;
};

#endif // LOADROBOT_H
