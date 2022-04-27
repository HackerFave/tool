#ifndef SteerWheel_H
#define SteerWheel_H

#include <QWidget>
#include <QTimerEvent>
#include <QPaintEvent>
#include <QTimer>

class SteerWheel : public QWidget
{
    Q_OBJECT

public:

    explicit SteerWheel(QWidget *parent = nullptr);
    void     setParameter(float minValue,float maxValue);
    void     setValue(float value);

private:

    float    calculateAngle();

protected:

    virtual void paintEvent(QPaintEvent *) override;

private slots:

    void timerHandle();

private:

    QTimer         m_timer;
    QPixmap        m_steerWheelImage;

    float          m_currentValue;
    float          m_targetValue;
    float          m_moveSpeed;
    float          m_minValue;
    float          m_maxValue;

};

#endif // SteerWheel_H
