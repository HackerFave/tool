#ifndef _QTSIMPLEMAP_H_
#define _QTSIMPLEMAP_H_

#include <QGridLayout>
#include "GraphicWindowQt.h"

#include <QWidget>
#include <QOpenGLContext>
#include <QWindow>
#include <QSurface>
#include <QDebug>
#include "EngineProxy.h"

#include <QtWidgets/QWidget>
#include <QSettings>
class QtSimpleMap : public QWidget
{
    Q_OBJECT

public:
    QtSimpleMap(QWidget *parent = Q_NULLPTR);

    ~QtSimpleMap() {
        killTimer(m_nTimerID);
    }


    virtual void paintEvent(QPaintEvent* event)
    {
        //_comViewer->frame();
    }
    virtual void timerEvent(QTimerEvent* event);

protected:

private:
    int               m_nTimerID;
    GraphicWindowQt* m_pGraphicsWindow;
    EngineProxy* m_pEngineProxy;
//    osg::ref_ptr<osg::GraphicsContext::Traits> _traits;
    QSettings      *_config = nullptr;
public slots:
    void onStartTimer();
};

#endif
