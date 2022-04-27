#include "QtSimpleMap.h"
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>
#include <QResizeEvent>
QtSimpleMap::QtSimpleMap(QWidget *parent)
    : QWidget(parent)
{
//    _config = new QSettings("rcdconfig.ini",QSettings::IniFormat);
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits(osg::DisplaySettings::instance().get());
    m_pGraphicsWindow = new GraphicWindowQt(traits.get());

    traits->width =parent->width()+300;
    traits->height =parent->height()+600;
    traits->doubleBuffer = true;
    traits->samples = 8;
    m_pGraphicsWindow = new GraphicWindowQt(traits.get());
    m_pGraphicsWindow->setMainWindow(this);
    QGridLayout* grid = new QGridLayout;
    grid->setMargin(0);
    grid->setSpacing(0);
    grid->addWidget(m_pGraphicsWindow->getGLWidget(), 0, 0);
    setLayout(grid);
    QTimer::singleShot(100, this, SLOT(onStartTimer()));//不要即可启动定时器，否则窗体还未创建，容易帧循环时出错
}

void QtSimpleMap::timerEvent(QTimerEvent* event)
{
    m_pEngineProxy->frame();
}

void QtSimpleMap::onStartTimer()
{

    m_pEngineProxy = EngineProxy::instance();

    m_pEngineProxy->init(m_pGraphicsWindow);

    m_pEngineProxy->ReadDbInfo(QString(qApp->applicationDirPath())+"/"+"HDMap.db");


    m_nTimerID = startTimer(100);
}

