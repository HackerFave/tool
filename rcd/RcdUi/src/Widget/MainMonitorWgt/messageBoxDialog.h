#ifndef MESSAGEBOXDIALOG_H
#define MESSAGEBOXDIALOG_H

#include <QDialog>

namespace Ui {
class messageBoxDialog;
}

class messageBoxDialog : public QDialog
{
    Q_OBJECT

public:
    explicit messageBoxDialog(QWidget *parent = nullptr);
    ~messageBoxDialog();

private:
    Ui::messageBoxDialog *ui;
};

#endif // MESSAGEBOXDIALOG_H
