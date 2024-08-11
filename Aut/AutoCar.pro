QT       += core gui
QT += webenginewidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG+=no_keywords
CONFIG+=c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/local/include/
INCLUDEPATH += $$PWD/opencv/include/opencv4
INCLUDEPATH += /usr/local/include/opencv4/
INCLUDEPATH += /usr/local/include/SDL2

LIBS += /usr/local/lib/libsioclient.a
LIBS += /Users/pancake/AppAutoCar/Aut/opencv/lib/libopencv_core.4.10.0.dylib
LIBS += /Users/pancake/AppAutoCar/Aut/opencv/lib/libopencv_videoio.4.10.0.dylib
LIBS += /Users/pancake/AppAutoCar/Aut/opencv/lib/libopencv_video.4.10.0.dylib
LIBS += /Users/pancake/AppAutoCar/Aut/opencv/lib/libopencv_imgproc.4.10.0.dylib
LIBS += -L/usr/local/lib -lSDL2


SOURCES += \
    customwebengine.cpp \
    # loadrobot.cpp \
    main.cpp \
    mainwindow.cpp \
    socketio.cpp \
    videoreader.cpp

HEADERS += \
    customwebengine.h \
    httplib.h \
    json.hpp \
    # loadrobot.h \
    mainwindow.h \
    socketio.h \
    static.h \
    videoreader.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resoure.qrc

DISTFILES += \
    websocketpp/CMakeLists.txt
