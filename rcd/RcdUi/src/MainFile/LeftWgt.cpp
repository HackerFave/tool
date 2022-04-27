#include "LeftWgt.h"
#include "ui_LeftWgt.h"
#include <QCloseEvent>
#include <QPainter>
#include <QDebug>

#include "Interface/WgtFactory.h"

LeftWgt::LeftWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LeftWgt)
{
    ui->setupUi(this);
    this->setWindowTitle("RCD");

    QWidget *pWgt = WgtFactory::getFunWidget(MD_CloudWeb);
    m_pCurrentWgt[SA_LeftScreenBottom] = pWgt;

    pWgt = WgtFactory::getFunWidget(MD_LeftCamera);
    m_pCurrentWgt[SA_LeftScreenTopR] = pWgt;

    pWgt = WgtFactory::getFunWidget(MD_DriveCamera);
    m_pCurrentWgt[SA_LeftScreenTopL] = pWgt;

    m_pLogoLayout = new QHBoxLayout();
    m_pLogoLayout->addWidget(ui->logoWgt,8);
    m_pLogoLayout->addStretch(2);

    m_pGridLayout = new QGridLayout(this);
    m_pGridLayout->setContentsMargins(50,30,50,30);
    m_pGridLayout->setHorizontalSpacing(30);
    m_pGridLayout->setVerticalSpacing(20);
    m_pGridLayout->setRowStretch(0, 8);
    m_pGridLayout->setRowStretch(1, 54);
    m_pGridLayout->setRowStretch(2, 36);
    m_pGridLayout->setColumnStretch(0, 7);
    m_pGridLayout->setColumnStretch(1, 8);

    this->setLayout(m_pGridLayout);
    this->setLayoutMonitor();
}

LeftWgt::~LeftWgt()
{
    delete ui;
}

void LeftWgt::closeEvent(QCloseEvent *event)
{
    if(event->type() == QEvent::Close){
        QApplication::exit();
    }
}

void LeftWgt::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setBrush(QColor(0,0x1d,0x33));
    painter.drawRect(event->rect().x()-2,event->rect().y()-2,
                     event->rect().width()+4,event->rect().height()+4);
//    int imgX = 0,imgY = 0;
//    int imgW = m_bgImage.width(),imgH = m_bgImage.height();
//    painter.drawPixmap(0,0,this->width(),this->height(),m_bgImage,imgX,imgY,imgW,imgH);
    painter.end();
}

void LeftWgt::addShowWidget(int type ,QWidget *pWgt)
{
    ScreenAreaType tType = static_cast<ScreenAreaType>(type);
    if(m_pCurrentWgt[tType] != pWgt) {
        m_pGridLayout->replaceWidget(m_pCurrentWgt[tType],pWgt);
        pWgt->setParent(this);
        pWgt->show();
        m_pCurrentWgt[tType] = pWgt;
    }
    this->repaint();
}

void LeftWgt::setLayoutMonitor()
{
    for(auto iter = m_pCurrentWgt.begin();iter != m_pCurrentWgt.end();iter++){
        iter.value()->setVisible(false);
    }
    while (m_pGridLayout->count() > 0) {
        m_pGridLayout->removeItem(m_pGridLayout->itemAt(0));
    }
    m_pGridLayout->addLayout(m_pLogoLayout, 0, 0, 1, 1);

    m_pGridLayout->addWidget(m_pCurrentWgt[SA_LeftScreenBottom], 1, 0, 2, 2);
    m_pCurrentWgt[SA_LeftScreenBottom]->setVisible(true);

}

void LeftWgt::setLayoutRemoteControl()
{
    for(auto iter = m_pCurrentWgt.begin();iter != m_pCurrentWgt.end();iter++){
        iter.value()->setVisible(false);
    }
    while (m_pGridLayout->count() > 0) {
        m_pGridLayout->removeItem(m_pGridLayout->itemAt(0));
    }

    m_pGridLayout->addLayout(m_pLogoLayout, 0, 0, 1, 1);

    m_pGridLayout->addWidget(m_pCurrentWgt[SA_LeftScreenBottom], 2, 0, 1, 2);
    m_pCurrentWgt[SA_LeftScreenBottom]->setVisible(true);

    m_pGridLayout->addWidget(m_pCurrentWgt[SA_LeftScreenTopR], 0, 1, 2, 1);
    m_pCurrentWgt[SA_LeftScreenTopR]->setVisible(true);

    m_pGridLayout->addWidget(m_pCurrentWgt[SA_LeftScreenTopL], 1, 0, 1, 1);
    m_pCurrentWgt[SA_LeftScreenTopL]->setVisible(true);
}

