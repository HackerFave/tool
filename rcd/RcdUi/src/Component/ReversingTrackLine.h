#ifndef ReversingTrackLine_H
#define ReversingTrackLine_H

#include <QWidget>
#include <QTimerEvent>
#include <QPaintEvent>
#include <QTimer>
#include <QSettings>
class ReversingTrackLine : public QWidget
{
    Q_OBJECT

public:

    explicit ReversingTrackLine(QWidget *parent = nullptr);
    void     setParameter(float minValue,float maxValue);
    void     setValue(float value);
    void     setCurrentValue(float value);
private:

    bool    calculateRadius(float &,float &);

protected:

    virtual void paintEvent(QPaintEvent *) override;

private slots:

    void timerHandle();

private:

    QTimer         m_timer;//dingshiq
    float          m_currentValue;
    float          m_targetValue;
    float          m_moveSpeed;
    float          m_minValue;
    float          m_maxValue;
    QSettings      *_config = nullptr;
};

#endif // ReversingTrackLine_H
