#ifndef FAULTMINECHOICE_H
#define FAULTMINECHOICE_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class FaultMineChoice;
}

class FaultMineChoice : public QWidget
{
    Q_OBJECT

public:
    explicit FaultMineChoice(QWidget *parent = NULL);
    ~FaultMineChoice();

    void clearChoice();

public:
    Ui::FaultMineChoice *ui;
    QButtonGroup* buttonGroup;
};

#endif // FAULTCHOICE_H
