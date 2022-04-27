#ifndef TR300RS_H
#define TR300RS_H
#include "msg.h"
#include "zmq/zmqmsg.hpp"
#include <QObject>
#include <QTimer>
#include <atomic>
#include <libusb-1.0/libusb.h>
#include <zmq/zmqwrap.hpp>

#define TR300RS_OLD_VID 0x044F
#define TR300RS_OLD_PID 0xb65d

#define TR300RS_VID 0x044F
#define TR300RS_PID 0xB66E
#define DEFAULT_OUT_ENDPOINT 0x01

//力反馈总强度
#define DEFAULT_GAIN_VALUE 25
//自动居中方向盘设置
#define DEFAULT_AUTOCENTER_DEVICE 100
//方向盘角度
#define DEFAULT_ROTATION_ANGLE 1080

#define BUTTON_PAGE_TIME_OUT_MS 500

class Tr300rs : public QObject
{
    Q_OBJECT
public:
    Tr300rs();
    ~Tr300rs();
    void StartRecvRawData();

private:
    void SetConfiguration(libusb_device_handle *handle);
    void SetInterface(libusb_device_handle *handle);
    void SetGain(libusb_device_handle *handle, int gain = DEFAULT_GAIN_VALUE);
    void SetAutoCenterByDevice(libusb_device_handle *handle);
    void SetAutoCenter(libusb_device_handle *handle, int center = DEFAULT_AUTOCENTER_DEVICE);
    void SetRotationAngle(libusb_device_handle *handle, int angle = DEFAULT_ROTATION_ANGLE);

    bool InitTr300();
    bool ReadTr300RawData();
    void SwitchToB66E(libusb_device_handle *handle);
    void InitB66E(libusb_device_handle *handle);

    void HandleRawData(const uint8_t *data, int len);
    void HandleTr300rsButton(Tr300rsSt &tmp);
    inline void HandleDirButton(Tr300rsSt &tmp);
    inline void HandleEnterButton(Tr300rsSt &tmp);
    inline void HandleControlButton(Tr300rsSt &tmp);

signals:
    void EmitControlData(const QByteArray &data);
    void EmitButton(quint32 key);
    void EmitStatus(quint8 status);

private:
    libusb_context *ctx;
    std::atomic_int keyPagePressCnt;
    std::chrono::steady_clock::time_point lastPageTimeP;
};

#endif // TR300RS_H
