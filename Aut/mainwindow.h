#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
// #include <QWebEnginePage>
#include "customwebengine.h"
#include "httplib.h"
#include <QTimer>
#include "socketio.h"
#include <QObject>
#include "static.h"
#include "json.hpp"
#include "videoreader.h"
#include <QKeyEvent>
#include <opencv2/opencv.hpp>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void startUpApp();

private:
    Ui::MainWindow *ui;
    QWebEngineView *mWebView;
    CustomWebengine *mWebPage;

    QTimer *timerShowRobot;

    QTimer *timerSignal;

    QTimer *timerShowFrame;

    QTimer *timerShowRobotOnMap;

    QString lastRobotName;

    nlohmann::json locations;

    cv::Mat frame;
    std::mutex mutex;

    QThread *threadReadVideo;
    VideoReader *videoReader;

    std::vector<std::string> robotsShow;

private Q_SLOTS:
    void showRobot();
    void onShowSignal();
    void onDrawFrame();
    void onActivated(int index);  // Slot xử lý khi mục thay đổi
    void onManualChecked(bool checked);
    void onAutomaticChecked(bool checked);
    void onGoStopButtonClicked();
    void onGoAllClicked();
    void onStopAllClicked();
    void onShowRobotOnMap();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    // std::unique_ptr<client> clientIO;
};
#endif // MAINWINDOW_H
