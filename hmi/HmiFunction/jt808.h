#ifndef JT808_H
#define JT808_H

#include <QObject>
#include <QByteArray>
#include <QMutex>
#include <QDebug>

#define JT808_HEAD_LEN              (sizeof(struct jt808HeadST))
#define JT808_REVERSE_BYTE1         0X0D
#define JT808_REVERSE_BYTE2         0X0A

#pragma pack(1)
struct jt808HeadST
{
    quint16 id;
    quint16 attr;
    quint16 num;
    quint16 packageTotal;
    quint16 packageSub;
};

typedef struct jt808MsgST
{
    quint16 id;
    quint16 serial;
    quint16 bodyLen;
    char body[1024];
} jt808MsgSt;
#pragma pack()
union jt808HeadUN
{
    struct jt808HeadST data;
    char var8[JT808_HEAD_LEN];
};


class Jt808
{
public:
    Jt808();
    static char sum(char *buf, int len);    
    static int reverseRecv(char *buf, int len);
    static int decode(char *buf, int len, jt808MsgSt &msg);
    static int reverseSend(char *buf, int len);
    static int encode(char *buf, jt808MsgSt msg);
private:
    static quint16 serial;
    static QMutex serialMutex;
};

#endif // JT808_H
