#ifndef MESSAGEBOXFORM_H
#define MESSAGEBOXFORM_H

#include <QWidget>

namespace Ui {
class messageBoxForm;
}

class messageBoxForm : public QWidget
{
    Q_OBJECT

public:
    explicit messageBoxForm(QWidget *parent = nullptr);
    ~messageBoxForm();

private:
    Ui::messageBoxForm *ui;
};

#endif // MESSAGEBOXFORM_H
