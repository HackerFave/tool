#include "AnalogClock.h"
#include <QPainter>
#include <QTime>

AnalogClock::AnalogClock(QWidget *parent) : QWidget(parent)
{
    m_timerId = startTimer(50);
}


void AnalogClock::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timerId) {
        this->update();
    }
}

void AnalogClock::paintEvent(QPaintEvent *)
{
    static const QPoint hourHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -50)
    };
    static const QPoint minuteHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -75)
    };
    static const QPoint secondHand[3] = {
        QPoint(4, 8),
        QPoint(-4, 8),
        QPoint(0, -95)
    };

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(0, 0, width(), height(), QColor(0,0,0,0));
    painter.setBrush(QGradient(QGradient::FreshOasis));
    int size = qMin(width(), height())/2;
    painter.drawEllipse(QPointF(width()/2, height()/2),size,size);

    QColor hourColor(127, 0, 127);
    QColor minuteColor(0, 127, 127, 191);
    QColor secondColor(255, 0, 0, 191);

    QTime time = QTime::currentTime();

    painter.translate(width() / 2, height() / 2);

    int side = qMin(width(), height());
    painter.scale(side / 200.0, side / 200.0);

//    painter.setPen(Qt::NoPen);
//    painter.setBrush(QGradient(QGradient::YoungGrass));
    painter.setPen(Qt::NoPen);
    painter.setBrush(hourColor);

    painter.save();
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();

    painter.setPen(hourColor);
    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }


    painter.setPen(Qt::NoPen);
    painter.setBrush(minuteColor);

    painter.save();
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();

    painter.setPen(minuteColor);
    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }


    painter.setPen(Qt::NoPen);
    painter.setBrush(secondColor);

    painter.save();
    painter.rotate(6.0 * (time.second() + time.msec()/1000.0));
    painter.drawConvexPolygon(secondHand, 3);
    painter.restore();
    painter.end();
}
