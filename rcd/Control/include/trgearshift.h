#ifndef TRGEARSHIFT_H
#define TRGEARSHIFT_H

#include <QObject>

#define TR_GEARSHIFT_VID 0x044F
#define TR_GEARSHIFT_PID 0xB660

class TrGearShift : public QObject
{
    Q_OBJECT
public:
    explicit TrGearShift(QObject *parent = nullptr);
    ~TrGearShift();
    void StartRecvRawData();

private:
    void HandleGearShiftRawData(const uint8_t *data, int len);

signals:
    void EmitGearShift(quint8 gear);
    void EmitGearShiftStatus(quint8 _status);
};

#endif // TRGEARSHIFT_H
