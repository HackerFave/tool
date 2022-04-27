#include "jt808.h"
#include "log.h"

quint16 Jt808::serial = 0;
QMutex Jt808::serialMutex;

Jt808::Jt808()
{

}

char Jt808::sum(char *buf, int len)
{
    int i;
    char sum = 0;

    for(i=0; i<len; i++) {
        sum ^= buf[i];
    }
    return sum;
}

int Jt808::reverseRecv(char *buf, int len)
{

    int i;
    int j = 0;

    for(i = 0; i < len-2; i++) {
        if(buf[i] == JT808_REVERSE_BYTE1) {
            if(buf[i+1] == 0x01) {
                buf[j++] = JT808_REVERSE_BYTE1;
            } else if(buf[i+1] == 0x02) {
                buf[j++] = JT808_REVERSE_BYTE2;
            } else {
                return -1;
            }
            i++;
        } else {
            buf[j++] = buf[i];
        }
    }
    buf[j++] = 0x0D;
    buf[j++] = 0x0A;

    return j;
}

int Jt808::reverseSend(char *buf, int len)
{
    int i;
    int j = 0;
    char buf1[512];

    for(i = 0; i < len-2; i++) {
        if(buf[i] == JT808_REVERSE_BYTE1) {
            buf1[j++] = JT808_REVERSE_BYTE1;
            buf1[j++] = 0x01;
        } else if(buf[i] == JT808_REVERSE_BYTE2){
            buf1[j++] = JT808_REVERSE_BYTE1;
            buf1[j++] = 0x02;
        } else {
            buf1[j++] = buf[i];
        }
    }
    buf1[j++] = 0x0D;
    buf1[j++] = 0x0A;

    for(i=0; i<j; i++) {
        buf[i] = buf1[i];
    }

    return j;
}

int Jt808::decode(char *buf, int len, jt808MsgSt &msg)
{
    int ret;
    if((ret = reverseRecv(buf, len)) < 0) {
        return -1;
    }
    char s = sum(buf, ret-3);
    if(s != buf[ret-3]) {
        return -1;
    }

    memmove((char *)&msg.id, buf, 2);
    memmove((char *)&msg.bodyLen, &buf[2], 2);
    memmove((char *)&msg.serial, &buf[4], 2);
    if(msg.bodyLen > ret - 10 - 3)
    {
        LOG_DEBUG("bodyLen length get error");
        return -1;
    }

    memmove(msg.body, &buf[10], msg.bodyLen);
    return 0;
}

int Jt808::encode(char *buf, jt808MsgSt msg)
{
    union jt808HeadUN head;
    head.data.id = msg.id;
    head.data.attr = msg.bodyLen;
    {
        QMutexLocker locker(&serialMutex);
        head.data.num = serial++ ;
    }
    head.data.packageSub = 1;
    head.data.packageTotal = 1;
    memmove(buf, head.var8, sizeof(head));
    //body
    memmove(&buf[JT808_HEAD_LEN], msg.body, msg.bodyLen);
    //sum
    buf[JT808_HEAD_LEN+msg.bodyLen] = sum(buf, JT808_HEAD_LEN+msg.bodyLen);
    buf[JT808_HEAD_LEN+msg.bodyLen+1] = 0x0D;
    buf[JT808_HEAD_LEN+msg.bodyLen+2] = 0x0A;
    //reverse
    return reverseSend(buf, JT808_HEAD_LEN+msg.bodyLen+3);
}

