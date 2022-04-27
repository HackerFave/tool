#include "CameraWidget.h"
#include <QApplication>
#include <QPainter>
#include <QResizeEvent>
#include <QTimer>
#include <QDebug>

#include "TGFrame.h"
#include "StreamThread.h"

CameraWidget::CameraWidget(QWidget *parent) : QWidget(parent),
    m_isPlay(false),
    m_isWaitingReconn(false)
{
    m_pFrame = new TGFrame(this);
    m_pFrame->move(0,0);
}

void CameraWidget::startPlay(QString &url)
{
    if(m_isPlay && m_playUrl == url){
        qDebug() << "m_isPlay :" << m_isPlay << "m_playUrl == url";
        return;
    }

    stopPlay();
    m_playUrl = url;

    if(url.isEmpty()){
        return;
    }
    m_isPlay = true;
    StreamThread *pStreaming = new StreamThread();
    QMetaObject::Connection conn(connect(pStreaming, SIGNAL(sglFinished()), this, SLOT(reconnect())));
    connect(pStreaming, SIGNAL(sglAsyncUpdateUi(QPixmap)), this, SLOT(asyncUpdateUi(QPixmap)),Qt::QueuedConnection);
    connect(this, SIGNAL(sglResize(int,int)), pStreaming, SLOT(outputResize(int,int)),Qt::DirectConnection);
    pStreaming->setParameter(url,conn,this->width(),this->height());
    pStreaming->startThread();
}

void CameraWidget::stopPlay()
{
    if(m_isPlay){
        m_isPlay = false;
        m_isWaitingReconn = false;
        emit sglResize(0,0); //发送窗口大小为0信号，播放停止。
        this->update();
        QApplication::sync();
    }
}

void CameraWidget::reconnect()
{
    qDebug() << "CameraWidget::reconnect()";
    m_isWaitingReconn = true;
    QTimer::singleShot(2000,Qt::VeryCoarseTimer,this,[this](){
        if(this->m_isWaitingReconn){
            this->m_isWaitingReconn = false;
            this->stopPlay();
            this->startPlay(m_playUrl);
        }
    });
}

void CameraWidget::setShadowType(int type)
{
    m_pFrame->setFrameType(static_cast<TGFrame::TGFrameType>(type));
}

void CameraWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(m_isPlay){
        int x = this->width() - m_videoImage.width();
        int y = this->height() - m_videoImage.height();
        x /= 2;
        y /= 2;

        painter.drawPixmap(QPoint(x,y),m_videoImage); //画出图像
    }else{
        m_videoImage.fill(Qt::transparent);
//        painter.setBrush(Qt::black);
//        painter.drawRect(0, 0, this->width(), this->height());
    }

    //painter.drawPixmap(0,0,this->width(),this->height(),m_bgImage,0,0,m_bgImage.width(),m_bgImage.height());

    painter.end();
}

void CameraWidget::resizeEvent(QResizeEvent *event)
{
    m_pFrame->resize(event->size());
    if(m_isPlay){
        emit sglResize(event->size().width(),event->size().height());
    }
}

void CameraWidget::asyncUpdateUi(QPixmap img)
{
    m_videoImage = img;
    this->update();
}
