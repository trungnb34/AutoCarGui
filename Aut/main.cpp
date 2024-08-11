#include "mainwindow.h"
#include "socketio.h"
#include <QApplication>
#include "loadrobot.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    SocketIO::get_instance();

    return a.exec();
}
