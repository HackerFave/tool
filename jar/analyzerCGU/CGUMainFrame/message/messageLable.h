#ifndef MESSAGELABLE_H
#define MESSAGELABLE_H

#include <QWidget>
#include <QTimer>
namespace Ui {
class messageLable;
}

class messageLable : public QWidget
{
    Q_OBJECT

public:
    explicit messageLable(QWidget *parent = nullptr);
    ~messageLable();
private slots:
    void slotMessage();
private:
    Ui::messageLable *ui;
    QTimer _timer;
};

#endif // MESSAGELABLE_H
