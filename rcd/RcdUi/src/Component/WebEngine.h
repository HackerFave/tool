#ifndef WEBENGINE_H
#define WEBENGINE_H

#include <QWebEngineView>

class WebEngine : public QWebEngineView
{
    Q_OBJECT
public:
    explicit WebEngine(QWidget* parent = Q_NULLPTR);

protected:

    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type);

};

#endif // WEBENGINE_H
