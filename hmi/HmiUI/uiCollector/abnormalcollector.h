#ifndef ABNORMALCOLLECTOR_H
#define ABNORMALCOLLECTOR_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class AbnormalCollector;
}

class AbnormalCollector : public QWidget
{
    Q_OBJECT

public:
    explicit AbnormalCollector(QWidget *parent = NULL);
    ~AbnormalCollector();

    void clearChoice();

public:
    Ui::AbnormalCollector *ui;
    QButtonGroup* buttonGroup;
};

#endif // ABNORMALDIGGER_H
