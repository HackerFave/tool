#ifndef WORKSTATECHOICE_H
#define WORKSTATECHOICE_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class WorkStateChoice;
}

class WorkStateChoice : public QWidget
{
    Q_OBJECT

public:
    explicit WorkStateChoice(QWidget *parent = 0);
    ~WorkStateChoice();

    void clearChoice();

public:
    Ui::WorkStateChoice *ui;
    QButtonGroup* buttonGroup;
};

#endif // WORKSTATECHOICE_H
