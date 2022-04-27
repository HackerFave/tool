#ifndef FIXSETDIGGER_H
#define FIXSETDIGGER_H

#include <QWidget>

namespace Ui {
class FixSetDigger;
}

class FixSetDigger : public QWidget
{
    Q_OBJECT

public:
    explicit FixSetDigger(QWidget *parent = 0);
    ~FixSetDigger();
    double getFixValue();
    double getLength();
    double getWidth();

    void setFixValue(double );
    void setLength(double);
    void setWidth(double);

private:
    Ui::FixSetDigger *ui;
};

#endif // FIXSETDIGGER_H
