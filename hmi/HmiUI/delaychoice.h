#ifndef DELAYCHOICE_H
#define DELAYCHOICE_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class DelayChoice;
}

class DelayChoice : public QWidget
{
    Q_OBJECT

public:
    explicit DelayChoice(QWidget *parent = NULL);
    ~DelayChoice();

    void clearChoice();

public:
    Ui::DelayChoice *ui;
    QButtonGroup* buttonGroup;
};

#endif // DELAYCHOICE_H
