#ifndef UDPDEALCLASS_H
#define UDPDEALCLASS_H

#include <QObject>
#include <QString>
#include <QUdpSocket>
#include <QByteArray>

class UdpDealClass : public QObject
{
    Q_OBJECT
public:
    explicit UdpDealClass(QObject *parent = 0);
    void udpInit(QString laddr, short lport, QString daddr, short dport);
    int  udpSendMsg(char *buf, int len);
    virtual void udpRecvMsgDeal(QByteArray array) = 0;
private:
    QString laddr, daddr;//ip + 端口
    short lport, dport;
    QUdpSocket  *udp;

public slots:
    void udpRecvMsg();
	void udpInit();
    void udpExit();

};

#endif // UDPDEALCLASS_H
