#include <QWebEngineSettings>
#include "WebEngine.h"

WebEngine::WebEngine(QWidget *parent) : QWebEngineView(parent)
{
//    QWebEngineSettings *webSetting = QWebEngineSettings::globalSettings();
//    webSetting->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
//    webSetting->setAttribute(QWebEngineSettings::PluginsEnabled, true);
//    webSetting->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
}

QWebEngineView *WebEngine::createWindow(QWebEnginePage::WebWindowType type)
{
    Q_UNUSED(type);
    return this;
}
