#include "mainwindow.h"
#include "ui_mainwindow.h"
// #include <QDebug>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mWebView = new QWebEngineView();
    mWebPage = new CustomWebengine(locations);

    mWebView->setPage(mWebPage);

    QUrl url = QUrl("qrc:/map.html");

    mWebView->page()->load(url);

    ui->verticalLayout->addWidget(mWebView);

    timerShowRobot = new QTimer();
    connect(timerShowRobot, &QTimer::timeout, this, &MainWindow::showRobot);
    timerShowRobot->start(2000);

    timerSignal = new QTimer();
    connect(timerSignal, &QTimer::timeout, this, &MainWindow::onShowSignal);
    timerSignal->start(2000);

    timerShowFrame = new QTimer();
    connect(timerShowFrame, &QTimer::timeout, this, &MainWindow::onDrawFrame);
    timerShowFrame->start(100);



    connect(ui->cbRobot, QOverload<int>::of(&QComboBox::activated),
            this, &MainWindow::onActivated);
    connect(ui->manual, QOverload<bool>::of(&QRadioButton::clicked), this, &MainWindow::onManualChecked);
    connect(ui->automatic, QOverload<bool>::of(&QRadioButton::clicked), this, &MainWindow::onAutomaticChecked);
    connect(ui->goStop, &QPushButton::clicked, this, &MainWindow::onGoStopButtonClicked);
    connect(ui->goAll, &QPushButton::clicked, this, &MainWindow::onGoAllClicked);
    connect(ui->stopAll, &QPushButton::clicked, this, &MainWindow::onStopAllClicked);

    threadReadVideo = new QThread();
    videoReader = new VideoReader(frame, mutex);
    videoReader->moveToThread(threadReadVideo);
    connect(threadReadVideo, &QThread::started, videoReader, &VideoReader::read_source);
    threadReadVideo->start();

    connect(mWebView, &QWebEngineView::loadFinished, this, [&](bool ok) {
        if (ok) {
            timerShowRobotOnMap = new QTimer();
            connect(timerShowRobotOnMap, &QTimer::timeout, this, &MainWindow::onShowRobotOnMap);
            timerShowRobotOnMap->start(500);
        } else {
            qDebug() << "Page failed to load.";
        }
    });

    startUpApp();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_A) {
        std::cout << "AAAAAAAAAAAAAAAAA" << std::endl;
    } else if (event->key() == Qt::Key_S) {
        std::cout << "SSSSSSSSSSSSSSSSS" << std::endl;
    } else if (event->key() == Qt::Key_W) {
        std::cout << "WWWWWWWWWWWWWWWWW" << std::endl;
    } else if (event->key() == Qt::Key_D) {
        std::cout << "DDDDDDDDDDDDDDDDD" << std::endl;
    }
}
void MainWindow::onShowRobotOnMap() {
    nlohmann::json robots = SocketIO::get_instance().getRobotLocations();
    if (robots.empty()) {
        return;
    }
    // std::cout << "hear" << std::endl;
    for (auto& [key, value] : robots.items()) {
        std::string lat = value["location"][0];
        std::string lng = value["location"][1];
        auto it = std::find(robotsShow.begin(), robotsShow.end(), key);
        QString jsCode;
        if (it != robotsShow.end()) {
            jsCode = QString::fromStdString(key) + ".setLngLat([" + QString::fromStdString(lng) + ", "+ QString::fromStdString(lat) +"])";
            // std::cout << "111" << std::endl;
        } else {
            jsCode = "var "+ QString::fromStdString(key) + " = new maptilersdk.Popup({closeButton : false, closeOnClick: false})" \
                ".setLngLat(["+ QString::fromStdString(lng) +", "+ QString::fromStdString(lat) +"])" \
                ".setHTML('"+ QString::fromStdString(key) +"')" \
                ".setMaxWidth('300px')" \
                ".addTo(map);";
            robotsShow.push_back(std::string(key));
            std::cout << "222" << std::endl;
        }
        mWebPage->runJavaScript(jsCode);
    }
}

void MainWindow::onDrawFrame() {
    cv::Mat frameClone = frame.clone();
    if (frameClone.empty()) {
        return;
    }
    QImage qImg(frameClone.data, frameClone.cols, frameClone.rows, QImage::Format_RGB888);
    qImg = qImg.rgbSwapped();
    ui->stream->setPixmap(QPixmap::fromImage(qImg).scaled(ui->stream->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::onShowSignal() {
    int signal1 = QRandomGenerator::global()->bounded(0, 2);
    int signal2 = QRandomGenerator::global()->bounded(0, 2);
    int signal3 = QRandomGenerator::global()->bounded(0, 2);

    QString signal;

    if (signal1) {
        ui->Sig1->setStyleSheet("background-color: red");
        signal += "255,";
    } else {
        ui->Sig1->setStyleSheet("background-color: white");
        signal += "0,";
    }
    if (signal2) {
        ui->Sig2->setStyleSheet("background-color: red");
        signal += "255,";
    } else {
        ui->Sig2->setStyleSheet("background-color: white");
        signal += "0,";
    }
    if (signal3) {
        ui->Sig3->setStyleSheet("background-color: red");
        signal += "255,";
    } else {
        ui->Sig3->setStyleSheet("background-color: white");
        signal += "0,";
    }

    ui->rev->setText(signal + ",data rev");
    ui->rev->setStyleSheet("QLabel { qproperty-alignment: AlignCenter;color: black;background-color: white }");
    ui->send->setText(signal + ",data send");
    ui->send->setStyleSheet("QLabel { qproperty-alignment: AlignCenter;color: black;background-color: white }");
}

void MainWindow::showRobot()
{
    // std::cout << "aaa" << std::endl;
    ui->cbRobot->clear();
    httplib::Client client(URL);
    auto res = client.Get("/robots");
    if (res && res->status == 200) {
        nlohmann::json body = nlohmann::json::parse(res->body);
        auto listRobot = body["robots"];
        int index = 0;
        for (auto robot : listRobot) {
            std::string robotID = robot["id"];
            ui->cbRobot->addItem(QString::fromStdString(robotID));
            if (robotID == lastRobotName.toStdString()) {
                ui->cbRobot->setCurrentIndex(index);
            }
            index ++;
        }

    }
}

void MainWindow::onGoStopButtonClicked() {
    std::string currentRobotID = ui->cbRobot->currentText().toStdString();
    if (!currentRobotID.length()) {
        QMessageBox::critical(this, "Error", "Please, select the robot that you want to control !!!");
        return;
    }
    std::string buttonText = ui->goStop->text().toStdString();
    if (buttonText == GO) {
        ui->goStop->setText(STOP);
    } else {
        ui->goStop->setText(GO);
        return;
    }
    nlohmann::json dataLocations;
    dataLocations["robot_id"] = currentRobotID;
    // TODO trungnb send location
    locations["robot_id"] = currentRobotID;
    if (!locations.contains("locations")) {
        locations["locations"] = nlohmann::json::array();
    }
    // std::cout << "locations : " << locations.dump() << std::endl;
    SocketIO::get_instance().emitSocket(LOCATION_DIRECTION, locations.dump());

}


void MainWindow::onGoAllClicked() {
    std::string currentRobotID = ui->cbRobot->currentText().toStdString();
    // std::cout << "AAAA" << std::endl;
    nlohmann::json dataLocations;
    dataLocations["robot_id"] = currentRobotID;
    // TODO trungnb send location
    locations["robot_id"] = currentRobotID;
    if (!locations.contains("locations")) {
        locations["locations"] = nlohmann::json::array();
    }
    // std::cout << "locations : " << locations.dump() << std::endl;
    SocketIO::get_instance().emitSocket(LOCATION_DIRECTION, locations.dump());


    nlohmann::json data;
    data["type"] = GO;
    SocketIO::get_instance().emitSocket(AUTOMATIC_ALL_ROBOT, data.dump());
}

void MainWindow::onStopAllClicked() {
    // std::cout << "BBBB" << std::endl;
    nlohmann::json data;
    data["type"] = STOP;
    SocketIO::get_instance().emitSocket(AUTOMATIC_ALL_ROBOT, data.dump());
    ui->goStop->setText(GO);
}

void MainWindow::onManualChecked(bool checked) {
    Q_UNUSED(checked);
    ui->goStop->setText(GO);
    startUpApp();
    nlohmann::json statusRunAllCar; // = {"type" : MANUAL};
    statusRunAllCar["type"] = MANUAL;
    nlohmann::json autoAllRobot;
    autoAllRobot["type"] = STOP;
    SocketIO::get_instance().emitSocket(AUTOMATIC_ALL_ROBOT, autoAllRobot.dump());
    SocketIO::get_instance().emitSocket(STATUS_RUN_ALL_CAR, statusRunAllCar.dump());

}

void MainWindow::onAutomaticChecked(bool checked) {
    Q_UNUSED(checked);
    ui->goStop->setEnabled(true);
    ui->stopAll->setEnabled(true);
    ui->goAll->setEnabled(true);
    // SocketIOClient.get_instance().emit("status_run_all_car", {"type" : STATIC_VAR.AUTOMATIC})
    nlohmann::json body;
    body["type"] = AUTOMATIC;
    SocketIO::get_instance().emitSocket(STATUS_RUN_ALL_CAR, body.dump());
}

void MainWindow::startUpApp() {
    ui->goStop->setEnabled(false);
    ui->stopAll->setEnabled(false);
    ui->goAll->setEnabled(false);
    ui->manual->setChecked(true);
}


void MainWindow::onActivated(int index) {

    Q_UNUSED(index);
    QString currentText = ui->cbRobot->currentText();
    if (!currentText.length()) {
        return;
    }
    if (lastRobotName.length()) {
        nlohmann::json stopStream;
        stopStream["robot_id"] = this->lastRobotName.toStdString();
        SocketIO::get_instance().emitSocket(STOP_STREAM_API, stopStream.dump());
    }
    nlohmann::json startStream;
    startStream["robot_id"] = currentText.toStdString();
    this->lastRobotName = currentText;
    SocketIO::get_instance().emitSocket(START_STREAM_API, startStream.dump());
}


MainWindow::~MainWindow()
{
    threadReadVideo->quit();
    threadReadVideo->wait();
    delete ui;
    delete threadReadVideo;
    delete videoReader;
    delete timerShowRobot;
    delete timerSignal;
    delete timerShowFrame;
    delete timerShowRobotOnMap;
}
