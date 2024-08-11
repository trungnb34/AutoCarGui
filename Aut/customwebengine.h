#ifndef CUSTOMWEBENGINE_H
#define CUSTOMWEBENGINE_H

#include <QWebEnginePage>
#include <iostream>
#include "json.hpp"

class CustomWebengine: public QWebEnginePage
{
    Q_OBJECT
private:
    nlohmann::json &locations;
public:
    explicit CustomWebengine(nlohmann::json &_locations, QObject *parent = nullptr);
protected:
    void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID) override;
};

#endif // CUSTOMWEBENGINE_H
