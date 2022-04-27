#include "screensaver.h"
#include "ui_screensaver.h"

ScreenSaver::ScreenSaver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenSaver)
{
    ui->setupUi(this);
}

ScreenSaver::~ScreenSaver()
{
    delete ui;
}

void ScreenSaver::mouseMoveEvent(QMouseEvent *)
{
    hal_set_lcd_status(0);
    close();
}
