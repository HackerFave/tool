#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>

namespace Ui {
class KeyBoard;
}

class KeyBoard : public QWidget
{
    Q_OBJECT

public:
    explicit KeyBoard(QWidget *parent = NULL);
    ~KeyBoard();

    QString getContext();

private slots:
    void on_keyBoard1_clicked();

    void on_keyBoard2_clicked();

    void on_keyBoard3_clicked();

    void on_keyBoard4_clicked();

    void on_keyBoard5_clicked();

    void on_keyBoard6_clicked();

    void on_keyBoard7_clicked();

    void on_keyBoard8_clicked();

    void on_keyBoard9_clicked();

    void on_keyBoard0_clicked();

    void on_keyBoardDelete_clicked();

    void on_keyBoardClear_clicked();

private:
    Ui::KeyBoard *ui;

signals:
    void isEmpty();

};

#endif // KEYBOARD_H
