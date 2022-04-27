#include "saveGifForm.h"
#include "ui_saveGifForm.h"
#include <QMutex>
#include <QDesktopWidget>
#include <QDebug>
QScopedPointer<saveGifForm> saveGifForm::P_saveGifForm;
mutex saveGifForm::_mutex;
saveGifForm *saveGifForm::instance()
{
    if (P_saveGifForm.isNull()) {
        _mutex.lock();
        if (P_saveGifForm.isNull()) {
            P_saveGifForm.reset(new saveGifForm);
        }
        _mutex.unlock();
    }

    return P_saveGifForm.data();
}
saveGifForm::saveGifForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::saveGifForm)
{
    ui->setupUi(this);
    QDesktopWidget *desktop = QApplication::desktop();

    QRect deskRect = desktop->availableGeometry();

    int x = (deskRect.width() - this->width())/2;

    int y = (deskRect.height() - this->height())/2;

    this->move(x, y);
    this->setWindowTitle("屏幕录制");
    this->setAttribute(Qt::WA_ShowModal);
    connect(ui->checkBox_allselect,SIGNAL(toggled(bool)),this,SLOT(slotUpdateChecked(bool)));
    connect(ui->checkBox_leftScreen,SIGNAL(toggled(bool)),this,SLOT(slotUpdateLeftChecked(bool)));
    connect(ui->checkBox_mainScreen,SIGNAL(toggled(bool)),this,SLOT(slotUpdateMainChecked(bool)));
    connect(ui->checkBox_rightScreen,SIGNAL(toggled(bool)),this,SLOT(slotUpdateRightChecked(bool)));


}
void saveGifForm::init()
{
    //ui->groupBox_screenRecord->setCheckable();
}
void saveGifForm::slotUpdateChecked(bool checked)
{
    if(ui->checkBox_leftScreen->isVisible()){
        ui->checkBox_leftScreen->setChecked(checked);
    }
    if(ui->checkBox_mainScreen->isVisible()){
        ui->checkBox_mainScreen->setChecked(checked);
    }
    if(ui->checkBox_rightScreen->isVisible()){
        ui->checkBox_rightScreen->setChecked(checked);
    }
}
void saveGifForm::slotUpdateLeftChecked(bool checked)
{
    _leftChecked = checked;
    _screenMap.insert("left",_leftChecked);
}
void saveGifForm::slotUpdateMainChecked(bool checked)
{
    _mainChecked = checked;
    _screenMap.insert("main",_mainChecked);
}
void saveGifForm::slotUpdateRightChecked(bool checked)
{
    _rightChecked = checked;
    _screenMap.insert("right",_rightChecked);
}

void saveGifForm::on_pushButton_ok_clicked(bool checked)
{
    this->close();
    emit signalGifForm(_screenMap);
}

void saveGifForm::on_pushButton_cancel_clicked(bool checked)
{
    this->close();
}
bool saveGifForm::getLeftScreenChecked() const
{
    return _leftChecked;
}
bool saveGifForm::getMainScreenChecked() const
{
    return _mainChecked;
}
bool saveGifForm::getRightScreenChecked() const
{
    return _rightChecked;
}

void saveGifForm::leftStatus(QString str)
{
    ui->label_left->setText(str);
}
void saveGifForm::mainStatus(QString str)
{
    ui->label_main->setText(str);
}
void saveGifForm::rightStatus(QString str)
{
    ui->label_right->setText(str);
}
void saveGifForm::setScreenRecord_L(bool checked)
{
    _screenRecord_L = checked;
}
void saveGifForm::setScreenRecord_M(bool checked)
{
    _screenRecord_M = checked;
}
void saveGifForm::setScreenRecord_R(bool checked)
{
    _screenRecord_R = checked;
}
bool saveGifForm::getScreenRecord_L()
{
    return _screenRecord_L;
}
bool saveGifForm::getScreenRecord_M()
{
    return _screenRecord_M;
}
bool saveGifForm::getScreenRecord_R()
{
    return _screenRecord_R;
}
void saveGifForm::showForm()
{
    QList<QScreen *> allScreen =  QGuiApplication::screens();
    if(allScreen.size()<3){
        ui->label_left->setVisible(false);
        ui->checkBox_leftScreen->setVisible(false);
        ui->label_right->setVisible(false);
        ui->checkBox_rightScreen->setVisible(false);
    }else if(allScreen.size()>=3) {
        ui->label_left->setVisible(true);
        ui->checkBox_leftScreen->setVisible(true);
        ui->label_right->setVisible(true);
        ui->checkBox_rightScreen->setVisible(true);
    }
    this->show();
}
