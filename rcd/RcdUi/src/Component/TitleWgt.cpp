#include "TitleWgt.h"
#include <QPainter>
#include <QDebug>

TitleWgt::TitleWgt(QWidget *parent) : QWidget(parent)
{
    m_titleImage.load(":/resource/logo/titlelogo.png");
}

void TitleWgt::setTitleString(QString &str)
{
    m_titleStr = str;
}

void TitleWgt::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::LosslessImageRendering);

    painter.setBrush(QColor(0,0x1d,0x33));
    painter.drawRect(event->rect().x()-2,event->rect().y()-2,
                     event->rect().width()+4,event->rect().height()+4);
    //painter.translate(width()/2, height()/2);
    //qreal side = qMin(width()/WIDTH, height()/HEIGHT);
    qreal side = height()*1.0/m_titleImage.height();
    int newWidth = width()/side;
    painter.scale(side , side);

    painter.drawPixmap(QPoint(newWidth - m_titleImage.width(),0),m_titleImage);
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPixelSize(m_titleImage.height()/2);
    painter.setFont(font);
    painter.drawText(QPointF(0,m_titleImage.height()/2.0 + m_titleImage.height()/5.0),m_titleStr);

    painter.end();
}
