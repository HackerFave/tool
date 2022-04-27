#ifndef STOPHEADINGCHOICE_H
#define STOPHEADINGCHOICE_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "vehicleitem.h"

namespace Ui {
class StopHeadingChoice;
}

class StopHeadingChoice : public QWidget
{
    Q_OBJECT

public:
    explicit StopHeadingChoice(QWidget *parent = NULL);
    ~StopHeadingChoice();

    qreal getFixHeading();
    void clearValue();

public slots:
    void itemFocusedSlots(qreal angle);

private slots:
    void on_upBtn_clicked();

    void on_downBtn_clicked();

    void on_doubleSpinBoxVehicleLen_valueChanged(double arg1);

    void on_doubleSpinBoxVehicleWidth_valueChanged(double arg1);

    void on_doubleSpinBoxStopLen_valueChanged(double arg1);

    void on_doubleSpinBoxStopWidth_valueChanged(double arg1);

private:
    Ui::StopHeadingChoice *ui;

    QGraphicsScene* vehicleScene;

    vehicleItem* itemSub90;
    vehicleItem* itemSub45;
    vehicleItem* itemAdd0;
    vehicleItem* itemAdd45;
    vehicleItem* itemAdd90;
    vehicleItem* itemExcavator;

    qreal value;
    double  stopWidth;
    double  stopLength;
    double  vehicleWidth;
    double  vehicleLength;
    double  stopCompAngle;
public:
    double getStopWidth();
    double getStopLength();
    double getVehicleLength();
    double getVehicleWidth();
    double getStopCompAngle();
    void setStopWidth(double value);
    void setStopLength(double value);
    void setVehicleLength(double value);
    void setVehicleWidth(double value);
    void setStopCompAngle(double value);
};

#endif // STOPHEADINGCHOICE_H
