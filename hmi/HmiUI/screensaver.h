#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include "System/common.h"
#include <QWidget>

namespace Ui {
class ScreenSaver;
}

class ScreenSaver : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenSaver(QWidget *parent = 0);
    ~ScreenSaver();
protected:
    void mouseMoveEvent(QMouseEvent *);

private:
    Ui::ScreenSaver *ui;
};

#endif // SCREENSAVER_H
