#ifndef FAULTCHOICE_H
#define FAULTCHOICE_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class FaultChoice;
}

class FaultChoice : public QWidget
{
    Q_OBJECT

public:
    explicit FaultChoice(QWidget *parent = NULL);
    ~FaultChoice();

    void clearChoice();

public:
    Ui::FaultChoice *ui;
    QButtonGroup* buttonGroup;
};

#endif // FAULTCHOICE_H
