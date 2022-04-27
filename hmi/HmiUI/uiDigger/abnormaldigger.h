#ifndef ABNORMALDIGGER_H
#define ABNORMALDIGGER_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class AbnormalDigger;
}

class AbnormalDigger : public QWidget
{
    Q_OBJECT

public:
    explicit AbnormalDigger(QWidget *parent = NULL);
    ~AbnormalDigger();

    void clearChoice();

public:
    Ui::AbnormalDigger *ui;
    QButtonGroup* buttonGroup;
};

#endif // ABNORMALDIGGER_H
