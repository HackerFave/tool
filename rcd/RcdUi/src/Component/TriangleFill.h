#ifndef TriangleFill_H
#define TriangleFill_H

#include <QWidget>
#include <QTimerEvent>
#include <QPaintEvent>
#include <QTimer>

class TriangleFill : public QWidget
{
    Q_OBJECT

public:
    enum TriangleFillType {
        Throttle = 0x1,
        Brake ,
    };

    explicit TriangleFill(QWidget *parent = nullptr);
    void     setParameter(TriangleFillType type,float minValue,float maxValue);
    void     setValue(float value);

private:

    bool    calculate();

protected:

    virtual void paintEvent(QPaintEvent *) override;

private slots:

    void timerHandle();

private:

    QTimer              m_timer;
    QColor              m_valueColor;
    TriangleFillType    m_type;
    float               m_currentValue;
    float               m_targetValue;
    float               m_moveSpeed;
    float               m_minValue;
    float               m_maxValue;

};

#endif // TriangleFill_H
