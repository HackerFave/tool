#include "backTrackLine.h"
#include <QPainter>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
backTrackLine::backTrackLine(QWidget *parent) : QWidget(parent)
{
    m_fai = 0.001f;
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();  //可用区域
    QRect screenRect = desktopWidget->screenGeometry();  //屏幕区域
    int w = screenRect.width();
    int h = screenRect.height();
    this->setGeometry(0,0,w,h);
    //connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerHandle()));
    //m_painter = QPainter(this);
    m_painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    //m_painter.setPen(QPen(QColor(0x3d, 0xfe, 0xfe), 5));
    m_pen_red = QPen(QColor(239, 41, 41), 5);
    m_pen_yellow = QPen(QColor(252, 233, 79), 5);
    m_pen_blue = QPen(QColor(114, 159, 207), 5);

    m_timer2 = new QTimer(this);
    connect(m_timer2, SIGNAL(timeout()), this, SLOT(on_timeout2()));


    setStyleSheet(QString::fromUtf8("background-color: rgb(237, 255, 208);"));

    P_frontPlayThread = new frontPlayThread(w, h,this->parent()->objectName());

    P_thread = new QThread;
    bool test = connect(P_thread, SIGNAL(started()), P_frontPlayThread, SLOT(slotCalculation()));

    test = connect(P_frontPlayThread, SIGNAL(signalPoints(QList<QPointF>, QList<QPointF>)),
                   this, SLOT(slotPoints(QList<QPointF>, QList<QPointF>)),Qt::BlockingQueuedConnection);
    P_frontPlayThread->moveToThread(P_thread);

    if (!P_thread->isRunning()) {
        P_thread->start();
    }
    setAngleValue(0.4);
    //    m_timer2->start(100);
}

void backTrackLine::paintEvent(QPaintEvent *)
{
    //return;
    if (!m_bPtsSet)
    {
        return;
    }


    m_painter.begin(this);

    m_painter.setPen(m_pen_red);
//    m_painter.drawPolyline(m_left_red, 3);
//    m_painter.drawPolyline(m_right_red, 3);


    m_painter.drawPolyline(m_left,11);
    m_painter.drawPolyline(m_right,11);

//    m_painter.setPen(m_pen_yellow);
//    m_painter.drawPolyline(m_left_yellow, 4);
//    m_painter.drawPolyline(m_right_yellow, 4);
//    m_painter.setPen(m_pen_blue);
//    m_painter.drawPolyline(m_left_blue, 6);
//    m_painter.drawPolyline(m_right_blue, 6);

    m_painter.end();

}

void backTrackLine::slotPoints(QList<QPointF> leftlist, QList<QPointF> rightlist)
{

//    std::copy(leftlist[0],leftlist[2],m_left_red);
//    std::copy(leftlist[2],leftlist[5],m_left_yellow);
//    std::copy(leftlist[5],leftlist[11],m_left_blue);
    for (int i = 0; i < 11; i++)
    {
        m_left[i]=leftlist.at(i);
        m_right[i] = rightlist.at(i);
//        if(i<=2){
//            m_left_red[i] = leftlist.at(i);
//            m_right_red[i] = rightlist.at(i);
//        }
//        else if(i<=5&&i>=2){
//            m_left_yellow[i] = leftlist.at(i);
//            m_right_yellow[i] = rightlist.at(i);
//        }
//        else if(i>=5) {
//            m_left_blue[i] = leftlist.at(i);
//            m_right_blue[i] = rightlist.at(i);
//        }
    }
    m_bPtsSet = true;
    this->update();
}

void backTrackLine::on_timeout2()
{
    m_fai++;
    if (m_fai > 40.0f)
    {
        m_fai = -40.0f;
    }
    setAngleValue(m_fai);
}

void backTrackLine::setAngleValue(double value)
{

    double fai = 0.001;
    if (value > -0.000001&&value < 0.000001) {
        fai = 0.0001;
    }
    else if (value < -40.0) {
        fai = -40.0;
    }
    else if (value > 40.0) {
        fai = 40.0;
    }
    else {
        fai = value;
    }
    P_frontPlayThread->slotm_fai(fai);
    //    if(!P_thread->isRunning()){
    //        P_thread->start();
    //    }
}
