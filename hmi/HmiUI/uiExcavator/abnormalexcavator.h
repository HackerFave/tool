#ifndef ABNORMALEXCAVATOR_H
#define ABNORMALEXCAVATOR_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class AbnormalExcavator;
}

class AbnormalExcavator : public QWidget
{
    Q_OBJECT

public:
    explicit AbnormalExcavator(QWidget *parent = NULL);
    ~AbnormalExcavator();

    void clearChoice();

public:
    Ui::AbnormalExcavator *ui;
    QButtonGroup* buttonGroup;
};

#endif // ABNORMALEXCAVATOR_H
