#ifndef GearWgt_H
#define GearWgt_H

#include <QWidget>
#include <QTimerEvent>
#include <QPaintEvent>
#include <QTimer>

class GearWgt : public QWidget
{
    Q_OBJECT

public:

    enum GearWgtType {
        Gear_D = 0x1,
        Gear_N = 0x0,
        Gear_R = -0x01
    };

    explicit GearWgt(QWidget *parent = nullptr);
    void     setValue(int value);

protected:

    virtual void paintEvent(QPaintEvent *) override;

private slots:

    void timerHandle();

private:

    QTimer         m_timer;
    QPixmap        m_gearWgtImage;
    GearWgtType    m_currentValue;

};

#endif // GearWgt_H
