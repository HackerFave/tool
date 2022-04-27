#ifndef RingDial_H
#define RingDial_H

#include <QWidget>
#include <QTimerEvent>
#include <QPaintEvent>
#include <QTimer>

class RingDial : public QWidget
{
    Q_OBJECT

public:

    enum RingDialType {
        Speedometer = 0x1,
        Tachometer ,
    };

    explicit RingDial(QWidget *parent = nullptr);
    void     setParameter(RingDialType type,float minValue,float maxValue);
    void     setValue(float value);

private:

    float    calculateAngle();

protected:

    virtual void paintEvent(QPaintEvent *) override;

private slots:

    void timerHandle();

private:

    QTimer         m_timer;
    RingDialType   m_type;
    float          m_currentValue;
    float          m_targetValue;
    float          m_moveSpeed;
    float          m_minValue;
    float          m_maxValue;

};

#endif // RingDial_H
