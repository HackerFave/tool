#include "RightWgt.h"
#include "ui_RightWgt.h"
#include <QCloseEvent>
#include <QPainter>
#include <QDebug>

#include "Interface/WgtFactory.h"

RightWgt::RightWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RightWgt)
{
    ui->setupUi(this);
    this->setWindowTitle("RCD");
    //this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    m_pHLayout = new QHBoxLayout(this);
    m_pHLayout->setContentsMargins(10,10,10,10);
    m_pHLayout->setSpacing(0);

    m_pHLayout->addStretch(2);

    QWidget *pWgt = WgtFactory::getFunWidget(MD_AllInfo);
    m_pHLayout->addWidget(pWgt,60);
    m_pCurrentWgt[SA_RightScreenL] = pWgt;

    m_pHLayout->addStretch(2);

    pWgt = WgtFactory::getFunWidget(MD_WarnInfo);
    m_pHLayout->addWidget(pWgt,40);
    m_pCurrentWgt[SA_RightScreenR] = pWgt;

    m_pHLayout->addStretch(2);

    this->setLayout(m_pHLayout);

}

RightWgt::~RightWgt()
{
    delete ui;
}

void RightWgt::closeEvent(QCloseEvent *event)
{
    if(event->type() == QEvent::Close){
        QApplication::exit();
    }
}

void RightWgt::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setBrush(QColor(0,0x1d,0x33));
    painter.drawRect(event->rect().x()-2,event->rect().y()-2,
                     event->rect().width()+4,event->rect().height()+4);
    painter.end();
}

void RightWgt::addShowWidget(int type ,QWidget *pWgt)
{
    ScreenAreaType tType = static_cast<ScreenAreaType>(type);
    if(m_pCurrentWgt[tType] != pWgt) {
        m_pHLayout->replaceWidget(m_pCurrentWgt[tType],pWgt);
        pWgt->setParent(this);
        pWgt->show();
        m_pCurrentWgt[tType] = pWgt;
    }
    this->repaint();
}

void RightWgt::setLayoutMonitor()
{
    m_pHLayout->setStretch(1,60);
}

void RightWgt::setLayoutRemoteControl()
{
    m_pHLayout->setStretch(1,50);
}
