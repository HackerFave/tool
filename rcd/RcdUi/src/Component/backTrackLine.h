#ifndef BACKTRACKLINE_H
#define BACKTRACKLINE_H

#include <QWidget>
#include <QTimerEvent>
#include <QPaintEvent>
#include <QTimer>
#include <QPainter>
#include "frontPlayThread.h"

class backTrackLine : public QWidget
{
    Q_OBJECT
public:
    enum TrankLineModel{
        FrontLIne = 0,
        BackLIne
    };
    explicit backTrackLine(QWidget *parent = nullptr);
    void setAngleValue(double value);

signals:
protected:

    virtual void paintEvent(QPaintEvent *) override;


private slots:

    //  void timerHandle();

    void slotPoints(QList<QPointF>, QList<QPointF>);
    void on_timeout2();
private:
    QPainter m_painter;
//    QPainter m_painter_red;
//    QPainter m_painter_yellow;
//    QPainter m_painter_blue;

    QPen m_pen_red;
    QPen m_pen_yellow;
    QPen m_pen_blue;
    frontPlayThread * P_frontPlayThread = nullptr;
    QThread * P_thread = nullptr;
        QPointF m_left[11];
    QPointF m_left_red[3];
    QPointF m_left_yellow[4];
    QPointF m_left_blue[6];
        QPointF m_right[11];
    QPointF m_right_red[3];
    QPointF m_right_yellow[4];
    QPointF m_right_blue[6];
    bool m_bPtsSet = false;

    QTimer* m_timer2;

    // 前轮角度;
    double m_fai;
    TrankLineModel _id;
};

#endif // BACKTRACKLINE_H
