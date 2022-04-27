#ifndef LEFTWGT_H
#define LEFTWGT_H

#include <QMap>
#include <QWidget>
#include <QPixmap>
#include <QGridLayout>
#include "System.h"

namespace Ui {
class LeftWgt;
}

// 左Widget背景
class LeftWgt : public QWidget
{
    Q_OBJECT

public:
    explicit LeftWgt(QWidget *parent = nullptr);
    ~LeftWgt();
    void addShowWidget(int type ,QWidget *pWgt);
    void setLayoutMonitor();
    void setLayoutRemoteControl();

protected:
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *) override;

private:

    Ui::LeftWgt    *ui;

    QGridLayout    *m_pGridLayout;
    QHBoxLayout    *m_pLogoLayout;
    QMap<ScreenAreaType,QWidget *>   m_pCurrentWgt;

};

#endif // LEFTWGT_H
