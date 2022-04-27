#ifndef MATERCHOICE_H
#define MATERCHOICE_H

#include <QWidget>

#include <QButtonGroup>

namespace Ui {
class MaterChoice;
}

class MaterChoice : public QWidget
{
    Q_OBJECT

public:
    explicit MaterChoice(QWidget *parent = NULL);
    ~MaterChoice();

    void clearChoice();

public:
    Ui::MaterChoice *ui;
    QButtonGroup* buttonGroup;
};

#endif // MATERCHOICE_H
