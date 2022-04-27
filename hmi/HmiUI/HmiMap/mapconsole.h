#ifndef MAPCONSOLE_H
#define MAPCONSOLE_H

#include <QWidget>

namespace Ui {
class mapConsole;
}

class mapConsole : public QWidget
{
    Q_OBJECT

public:
    explicit mapConsole(QWidget *parent = 0);
    ~mapConsole();

private slots:

    void on_mapMinusBtn_clicked();

    void on_mapPlusBtn_clicked();

public slots:
    void gpsStateChange(char flag);//修改GPS状态

signals:
    void zoomInSignal();
    void zoomOutSignal();

private:
    Ui::mapConsole *ui;
};

#endif // MAPCONSOLE_H
