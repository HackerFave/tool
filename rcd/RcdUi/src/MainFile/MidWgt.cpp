#include "MidWgt.h"
#include "ui_MidWgt.h"
#include <QCloseEvent>
#include <QPainter>
#include <QDebug>

#include "Interface/WgtFactory.h"

MidWgt::MidWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MidWgt)
{
    ui->setupUi(this);
    this->setWindowTitle("RCD");
    //this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    m_pVLayout = new QVBoxLayout(this);
    m_pVLayout->setContentsMargins(0,0,0,0);
    m_pVLayout->setSpacing(0);

    QWidget *pWgt = WgtFactory::getFunWidget(MD_MainMonitor);
    m_pVLayout->addWidget(pWgt);

    m_pCurrentWgt[SA_MainScreen] = pWgt;

    this->setLayout(m_pVLayout);
}

MidWgt::~MidWgt()
{
    delete ui;
}

void MidWgt::closeEvent(QCloseEvent *event)
{
    if(event->type() == QEvent::Close){
        QApplication::exit();
    }
}

void MidWgt::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setBrush(QColor(0,0x1d,0x33));
    painter.drawRect(event->rect().x()-2,event->rect().y()-2,
                     event->rect().width()+4,event->rect().height()+4);

    //截取背景 填充显示
//    float pre = this->width()*1.0/this->height();
//    int templen = m_bgImage.width()/pre;
//    int imgX = 0,imgY = 0;
//    int imgW = m_bgImage.width(),imgH = m_bgImage.height();
//    if(templen <= m_bgImage.height()){
//        imgY = (m_bgImage.height() - templen)/2;
//        imgH = templen;
//    }else{
//        templen = m_bgImage.height()*pre;
//        imgX = (m_bgImage.width() - templen)/2;
//        imgW = templen;
//    }
    //painter.drawTiledPixmap(0,0,this->width(),this->height(),m_bgImage,imgX,imgY);
    //painter.drawPixmap(0,0,this->width(),this->height(),m_bgImage,imgX,imgY,imgW,imgH);
    painter.end();
}

void MidWgt::addShowWidget(int type ,QWidget *pWgt)
{
    ScreenAreaType tType = static_cast<ScreenAreaType>(type);
    if(m_pCurrentWgt[tType] != pWgt) {
        m_pVLayout->replaceWidget(m_pCurrentWgt[tType],pWgt);
        pWgt->setParent(this);
        pWgt->show();
        m_pCurrentWgt[tType] = pWgt;
    }
    this->repaint();
//    switch (tType) {
//        case MD_MainScreenArea:{

//            m_pVLayout->replaceWidget(m_pCurrentWgt[MD_MainScreenArea],pWgt);
//            pWgt->show();
//            m_pCurrentWgt[MD_MainScreenArea] = pWgt;

//        }break;
//        default:break;
//    }
}
