#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QTimerEvent>
#include <QPaintEvent>
#include <QTimer>

class Dashboard : public QWidget
{
    Q_OBJECT

public:

    enum DashboardType {
        Volmeter = 0x1,
        OilMeter ,
        OilPressureMeter ,
        CoolantTemperature ,
    };


    explicit Dashboard(QWidget *parent = nullptr);
    void     setParameter(DashboardType type,float minValue,float maxValue);
    void     setValue(float value);

private:

    float    calculateAngle();

protected:

    virtual void paintEvent(QPaintEvent *) override;

private slots:

    void timerHandle();

private:

    QTimer         m_timer;
    QPixmap        m_corollaImage;
    QPixmap        m_logoImage;
    QPixmap        m_needleImage;
    QString        m_name;
    float          m_currentValue;
    float          m_targetValue;
    float          m_moveSpeed;
    float          m_minValue;
    float          m_maxValue;

};

#endif // DASHBOARD_H
