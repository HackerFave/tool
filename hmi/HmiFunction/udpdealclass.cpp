#include "udpdealclass.h"
#include "log.h"
#include "paraparsing.h"
#include <QDebug>
UdpDealClass::UdpDealClass(QObject *parent) : QObject(parent)
{

}

void UdpDealClass::udpInit(QString laddr, short lport, QString daddr, short dport)
{
    this->laddr = laddr;
    this->lport = lport;
    this->daddr = daddr;
    this->dport = dport;

    LOG_DEBUG("udpInit laddr[%s], lport[%d], daddr[%s], dport[%d]", laddr.toLatin1().data(), lport, daddr.toLatin1().data(), dport);
	
    udp = new QUdpSocket();       //创建套接字
    udp->bind(QHostAddress(laddr), lport);
    connect(udp,SIGNAL(readyRead()),this,SLOT(udpRecvMsg()));
    qDebug()<<"初始化网络通信："<<laddr<<lport<<daddr<<dport<<"TC-----test";
}
void UdpDealClass::udpInit()
{
    udpInit(ParaParsing::instance()->paraSt.pub.localIp, ParaParsing::instance()->paraSt.pub.localPort,
                ParaParsing::instance()->paraSt.pub.remoteIp, ParaParsing::instance()->paraSt.pub.remotePort);
}

void UdpDealClass::udpExit()
{
    udp->close();
}

void UdpDealClass::udpRecvMsg()
{
    while (udp->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress sender;
        sender.setAddress(daddr);
        quint16 senderPort = dport;

        datagram.resize(udp->pendingDatagramSize());
        qint64 ret = udp->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);

        if(ret > 0) {
            LOG_DEBUG_HEX(datagram.data(), datagram.size(), "udp recv:");
            //qDebug()<<"接收报文数据："<<sender<<senderPort<<"TC-----test";
            udpRecvMsgDeal(datagram);
            //udpRecvMsgDeal(datagram);
        }

    }
}
/**
 * @brief UdpDealClass::sendMsg
 * @param buf
 * @param len
 * @return Return the number of bytes send on success;otherwise returns -1.
 */
int  UdpDealClass::udpSendMsg(char *buf, int len)
{
    static QHostAddress serverAddress = QHostAddress(daddr);
    qint64 ret = udp->writeDatagram(buf, len, serverAddress, dport);
    short id;
    memmove((char *)&id, buf, 2);   
    LOG_DEBUG_HEX(buf, len, "udp send id[%04x], array:", id);

    return ret;
}

