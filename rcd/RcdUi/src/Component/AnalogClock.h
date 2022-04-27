#ifndef AnalogClock_H
#define AnalogClock_H

#include <QWidget>
#include <QTimerEvent>
#include <QPaintEvent>

class AnalogClock : public QWidget
{
    Q_OBJECT
public:
    explicit AnalogClock(QWidget *parent = nullptr);

protected:

    virtual void timerEvent(QTimerEvent *) override;
    virtual void paintEvent(QPaintEvent *) override;


private:
    int m_timerId;

};

#endif // AnalogClock_H
