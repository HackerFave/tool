#include "tr300rs.h"
#include "hidapi/hidapi.h"
#include "mydebug.h"
#include <QDebug>
#include <QThread>
#include <cstring>

Tr300rs::Tr300rs()
{
    libusb_init(&ctx);
    keyPagePressCnt.store(0);
}

Tr300rs::~Tr300rs()
{
    libusb_exit(ctx);
}

void Tr300rs::StartRecvRawData()
{
    bool foundTr300 = false;
    while (1)
    {
        if (foundTr300)
        {
            foundTr300 = ReadTr300RawData();
        }
        else
        {
            emit EmitStatus(DEV_LINKSTATUS_DISCONNECTED);
            MY_DEBUG() << "Not Found Tr300rs ,wait seconds";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            foundTr300 = InitTr300();
        }
    }
}

void Tr300rs::SetConfiguration(libusb_device_handle *handle)
{
    int result = 0;
    MY_DEBUG() << " Start Set Configuration to 1";
    result = libusb_set_configuration(handle, 1);
    MY_DEBUG() << " Set Configuration to 1,result:" << libusb_error_name(result);
}

void Tr300rs::SetInterface(libusb_device_handle *handle)
{
    int result = 0;
    MY_DEBUG() << " Start Set Interface to 0,alternateSetting:0";
    result = libusb_set_interface_alt_setting(handle, 0, 0);
    MY_DEBUG() << " Set Interface to 0,result:" << libusb_error_name(result);
}

void Tr300rs::SetGain(libusb_device_handle *handle, int gain)
{
    int result = 0;
    MY_DEBUG() << " Start Set Gain to " << gain << "%";
    uint8_t setCmd[64] = {0x00};
    setCmd[0] = 0x60;
    setCmd[1] = 0x02;
    setCmd[2] = (gain / 100.0) * 0xFF;
    result = libusb_interrupt_transfer(handle, DEFAULT_OUT_ENDPOINT, setCmd, sizeof(setCmd), NULL, 10);
    MY_DEBUG() << " SetGain,result:" << libusb_error_name(result);
}

void Tr300rs::SetAutoCenterByDevice(libusb_device_handle *handle)
{
    int result = 0;
    MY_DEBUG() << " Start Set AutoCenter By Device ";
    uint8_t setCmd[64] = {0x00};
    setCmd[0] = 0x60;
    setCmd[1] = 0x08;
    setCmd[2] = 0x04;
    //0x01 trun on setting by device.  0x00 trun off
    setCmd[3] = 0x01;
    result = libusb_interrupt_transfer(handle, DEFAULT_OUT_ENDPOINT, setCmd, sizeof(setCmd), NULL, 10);
    MY_DEBUG() << " Set AutoCenter By Device,result:" << libusb_error_name(result);
}

void Tr300rs::SetAutoCenter(libusb_device_handle *handle, int center)
{
    int result = 0;
    MY_DEBUG() << " Start Set AutoCenter to " << center << "%";
    uint8_t setCmd[64] = {0x00};
    setCmd[0] = 0x60;
    setCmd[1] = 0x08;
    setCmd[2] = 0x03;
    uint16_t centerCmd = (center / 100.0) * 0xFFFF;
    setCmd[3] = static_cast<uint8_t>(centerCmd & 0x00FF);
    setCmd[4] = static_cast<uint8_t>((centerCmd & 0xFF00) >> 8);
    result = libusb_interrupt_transfer(handle, DEFAULT_OUT_ENDPOINT, setCmd, sizeof(setCmd), NULL, 10);
    MY_DEBUG() << " Set AutoCenter,result:" << libusb_error_name(result);
}

void Tr300rs::SetRotationAngle(libusb_device_handle *handle, int angle)
{
    MY_DEBUG() << " Start Set RotationAngle to " << angle;
    if (angle < 40 || angle > 1080)
    {
        MY_DEBUG() << "angle overflow,must be >= 40 and <= 1080,setting is :" << angle;
        return;
    }
    int result = 0;
    uint8_t setCmd[64] = {0x00};
    setCmd[0] = 0x60;
    setCmd[1] = 0x08;
    setCmd[2] = 0x11;
    uint16_t angleCmd = (65535 / 1080.0) * angle;
    setCmd[3] = static_cast<uint8_t>(angleCmd & 0x00FF);
    setCmd[4] = static_cast<uint8_t>((angleCmd & 0xFF00) >> 8);
    result = libusb_interrupt_transfer(handle, DEFAULT_OUT_ENDPOINT, setCmd, sizeof(setCmd), NULL, 10);
    MY_DEBUG() << " Set RotationAngle,result:" << libusb_error_name(result);
}

bool Tr300rs::InitTr300()
{
    bool result = false;
    libusb_device **list = NULL;
    int cnt = libusb_get_device_list(ctx, &list);
    MY_DEBUG() << "Get " << cnt << " nums usb device";
    for (int i = 0; i < cnt; i++)
    {
        libusb_device *device = list[i];
        libusb_device_descriptor desc;
        int ret = libusb_get_device_descriptor(device, &desc);
        if ((desc.idVendor == TR300RS_OLD_VID) && (desc.idProduct == TR300RS_OLD_PID))
        {
            libusb_device_handle *handle;
            ret = libusb_open(device, &handle);
            qDebug("Open %04x:%04x Resutl:%s", TR300RS_OLD_VID, TR300RS_OLD_PID, libusb_error_name(ret));
            SwitchToB66E(handle);
            libusb_close(handle);
            break;
        }
        if ((desc.idVendor == TR300RS_VID) && (desc.idProduct == TR300RS_PID))
        {
            libusb_device_handle *handle;
            ret = libusb_open(device, &handle);
            qDebug("Open %04x:%04x Resutl:%s", TR300RS_VID, TR300RS_PID, libusb_error_name(ret));
            InitB66E(handle);
            libusb_close(handle);
            result = true;
            break;
        }
    }
    libusb_free_device_list(list, 1);
    return result;
}

bool Tr300rs::ReadTr300RawData()
{
    hid_device *hidDevice = NULL;
    hidDevice = hid_open(TR300RS_VID, TR300RS_PID, NULL);
    if (hidDevice == NULL)
    {
        MY_DEBUG() << "hid open Tr300 Failed:" << hid_error(hidDevice);
    }
    else
    {
        while (1)
        {
            unsigned char buf[128] = {0x00};
            int res = hid_read(hidDevice, buf, sizeof(buf));
            if (res > 0)
            {
                HandleRawData(buf, res);
                emit EmitStatus(DEV_LINKSTATUS_CONNECTED_DATA);
            }
            else
            {
                qCritical() << "Read Tr300rs error.";
                break;
            }
        }
    }
    hid_close(hidDevice);
    return false;
}

void Tr300rs::SwitchToB66E(libusb_device_handle *handle)
{
    int ret = libusb_control_transfer(handle, 0x41, 83, 0x0005, 0, NULL, 0, 10);
    MY_DEBUG() << "switch to b66e:" << libusb_error_name(ret);
}

void Tr300rs::InitB66E(libusb_device_handle *handle)
{
    int result = libusb_set_auto_detach_kernel_driver(handle, 1);
    MY_DEBUG() << "set auto detach kernel driver:" << libusb_error_name(result);

    int ret = libusb_claim_interface(handle, 0);
    MY_DEBUG() << " Claim interface Result:" << libusb_error_name(ret);
    SetInterface(handle);
    SetGain(handle, DEFAULT_GAIN_VALUE);
    SetAutoCenterByDevice(handle);
    SetAutoCenter(handle, DEFAULT_AUTOCENTER_DEVICE);
    SetRotationAngle(handle, DEFAULT_ROTATION_ANGLE);
    libusb_release_interface(handle, 0);
}

void Tr300rs::HandleRawData(const uint8_t *data, int len)
{
    Tr300rsSt tmp;
    std::memmove(&tmp, data, len);

    ControlData cacheControlData;
    std::memset(&cacheControlData, 0x00, sizeof(ControlData));

    cacheControlData.steeringWheelAngle = tmp.steeringWheelAngle;                    //方向盘
    cacheControlData.brakePedal = ((0x3FF - tmp.brake) / (1024.0)) * 10000;          //电制动
    cacheControlData.throttlePedal = ((0x3FF - tmp.accelerator) / (1024.0)) * 10000; //油门
    cacheControlData.hydraulicBrake = ((0x3FF - tmp.clutch) / (1024.0)) * 10000;     //机械制动
    MY_DEBUG() << "steeringWheelAngle:" << cacheControlData.steeringWheelAngle << " brakePedal:" << cacheControlData.brakePedal
               << " throttlePedal:" << cacheControlData.throttlePedal << " hydraulicBrake:" << cacheControlData.hydraulicBrake;
    QByteArray controlData(reinterpret_cast<const char *>(&cacheControlData), sizeof(cacheControlData));
    emit EmitControlData(controlData);
    //处理按键
    HandleTr300rsButton(tmp);
}

void Tr300rs::HandleTr300rsButton(Tr300rsSt &tmp)
{
    HandleDirButton(tmp);
    HandleEnterButton(tmp);
    HandleControlButton(tmp);
}

void Tr300rs::HandleDirButton(Tr300rsSt &tmp)
{
    static KeyButton dir = {Qt::Key_Any};
    bool unPressButton = false;
    switch (tmp.dirButton)
    {
    case 0x00:
        dir.key = Qt::Key_Up;
        MY_DEBUG() << "Key Up";
        break;
    case 0x02:
        dir.key = Qt::Key_Right;
        MY_DEBUG() << "Key Right";
        break;
    case 0x04:
        dir.key = Qt::Key_Down;
        MY_DEBUG() << "Key Down";
        break;
    case 0x06:
        dir.key = Qt::Key_Left;
        MY_DEBUG() << "Key Left";
        break;
    case 0x0f:
        unPressButton = true;
        break;
    default:
        break;
    }
    if ((dir.key != Qt::Key_Any) && unPressButton)
    {
        emit EmitButton(dir.key);
        dir.key = Qt::Key_Any;
        MY_DEBUG() << "Emit dir key Button.";
    }
}

void Tr300rs::HandleEnterButton(Tr300rsSt &tmp)
{
    static KeyButton dir = {Qt::Key_Any};
    bool unPressButton = false;
    switch (tmp.button)
    {
    case 0x40:
        //确认键
        dir.key = Qt::Key_Enter;
        MY_DEBUG() << "Key Enter";
        break;
    case 0x80:
        //返回键
        dir.key = Qt::Key_Escape;
        MY_DEBUG() << " Key Esc";
        break;
    default:
        unPressButton = true;
        break;
    }
    if ((dir.key != Qt::Key_Any) && unPressButton)
    {
        emit EmitButton(dir.key);
        dir.key = Qt::Key_Any;
        MY_DEBUG() << "Emit Enter/Esc key Button.";
    }
}

void Tr300rs::HandleControlButton(Tr300rsSt &tmp)
{
    static KeyButton dir = {Qt::Key_Any};
    bool unPressButton = false;
    switch (tmp.button)
    {
    case 0x01:
        dir.key = Qt::Key_PageUp;
        MY_DEBUG() << "key pageUp";
        break;
    case 0x02:
        dir.key = Qt::Key_PageDown;
        MY_DEBUG() << "Key PageDown";
        break;
    default:
        unPressButton = true;
        break;
    }
    if ((dir.key != Qt::Key_Any) && unPressButton)
    {
        if (keyPagePressCnt.fetch_add(1) == 0)
        {
            //first time
            lastPageTimeP = std::chrono::steady_clock::now();
            dir.key = Qt::Key_Any;
            return;
        }
        auto curTimeP = std::chrono::steady_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(curTimeP - lastPageTimeP);
        MY_DEBUG() << "diff time:" << diff.count();
        if (diff.count() >= BUTTON_PAGE_TIME_OUT_MS)
        {
            MY_DEBUG() << "Key page time out,cnt:" << keyPagePressCnt.load() << "Restart cnt";
            keyPagePressCnt.store(1);
        }
        else if (keyPagePressCnt.load() >= 3)
        {
            keyPagePressCnt.store(0);
            emit EmitButton(dir.key);
            MY_DEBUG() << "Emit PageUp/PageDown key Button.";
        }
        lastPageTimeP = curTimeP;
        dir.key = Qt::Key_Any;
    }
}
