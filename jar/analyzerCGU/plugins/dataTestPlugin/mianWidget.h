#ifndef MIANWIDGET_H
#define MIANWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class mianWidget; }
QT_END_NAMESPACE

class mianWidget : public QWidget
{
    Q_OBJECT

public:
    mianWidget(QWidget *parent = nullptr);
    ~mianWidget();

private slots:
    void on_pushButton_test_clicked();

    void on_pushButton_test_5_clicked();

    void on_pushButton_test_4_clicked();

    void on_pushButton_test_2_clicked();

    void on_pushButton_test_3_clicked();

private:
    Ui::mianWidget *ui;
};
#endif // MIANWIDGET_H
