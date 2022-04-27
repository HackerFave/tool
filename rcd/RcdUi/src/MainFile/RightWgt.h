#ifndef RIGHTWGT_H
#define RIGHTWGT_H

#include <QMap>
#include <QWidget>
#include <QPixmap>
#include <QHBoxLayout>
#include "System.h"

namespace Ui {
class RightWgt;
}

// 右Widget背景

class RightWgt : public QWidget
{
    Q_OBJECT

public:
    explicit RightWgt(QWidget *parent = nullptr);
    ~RightWgt();

    void addShowWidget(int type ,QWidget *pWgt);
    void setLayoutMonitor();
    void setLayoutRemoteControl();

protected:
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *) override;

private:
    Ui::RightWgt    *ui;
    QHBoxLayout    *m_pHLayout;
    QMap<ScreenAreaType,QWidget *>   m_pCurrentWgt;

};

#endif // RIGHTWGT_H
