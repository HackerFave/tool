#include "PageWgt.h"
#include <QPainter>
#include <QDebug>

#define  WIDTH          1920.0
#define  HEIGHT         100.0
#define  INTERVAL       150

PageWgt::PageWgt(QWidget *parent) : QWidget(parent)
{
    //connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerHandle()));
    //m_timer.start(1000);
}

void PageWgt::setMaxPage(quint8 value)
{
    if(value == 0){
        m_maxPage = 1;
    }else{
        m_maxPage = value;
    }
}

void PageWgt::setPage(quint8 value)
{
    if(value >= m_maxPage){
        m_currentPage = m_maxPage - 1;
    }else{
        m_currentPage = value;
    }
    this->update();
}

void PageWgt::timerHandle()
{
    static int i = 0;
    if(i >= m_maxPage){
        i = 0;
    }
    this->setPage(i);
    i++;
}

void PageWgt::paintEvent(QPaintEvent *)
{

    if(m_maxPage < 2){
        return;
    }

    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing);

    painter.translate(width()/2, height()/2);
    qreal side = qMin(width()/WIDTH, height()/HEIGHT);
    painter.scale(side , side);

    painter.setBrush(QColor(0x1f,0x74,0x9e));

    QPoint startPoint((m_maxPage - 1)*INTERVAL/-2,0);

    for(int i = 0;i < m_maxPage;i++){
        if(i == m_currentPage){
            painter.save();
            painter.setBrush(QColor(0x0,0xbc,0xff));
            painter.drawEllipse(startPoint,30,30);
            painter.restore();
            startPoint += QPoint(INTERVAL,0);
        }else{
            painter.drawEllipse(startPoint,22,22);
            startPoint += QPoint(INTERVAL,0);
        }
    }

    painter.end();
}
