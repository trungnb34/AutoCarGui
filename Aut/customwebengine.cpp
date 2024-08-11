#include "customwebengine.h"

CustomWebengine::CustomWebengine(nlohmann::json &_locations, QObject *parent) : locations(_locations), QWebEnginePage(parent)
{

}


void CustomWebengine::javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID)
{
    Q_UNUSED(level);
    Q_UNUSED(sourceID);
    Q_UNUSED(lineNumber);
    try {
        nlohmann::json data = nlohmann::json::parse(message.toStdString());
        if (data.contains("CREATE")) {
            auto lnglat = data["CREATE"];
            float lat = lnglat["lat"];
            float lng = lnglat["lng"];
            if (locations.contains("locations")) {
                locations["locations"].push_back({lat, lng});
            } else {
                locations["locations"] = {{lat, lng}};
            }
        } else if (data.contains("DELETE")) {
            nlohmann::json temp = locations;
            int size = locations["locations"].size();
            locations["locations"].clear();
            for (int i = 0; i < size - 1; i++) {
                locations["locations"].push_back(temp["locations"][i]);
            }
        }
    } catch(...) {}
}
