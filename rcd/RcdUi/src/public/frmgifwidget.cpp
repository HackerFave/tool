
#include "frmgifwidget.h"
#include "ui_frmgifwidget.h"
#include "gifwidget.h"
#include "movieplayer.h"
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QLabel>
#include<QScreen>
frmGifWidget::frmGifWidget(QWidget *parent) : QWidget(parent), ui(new Ui::frmGifWidget)
{
    ui->setupUi(this);
    _moviePlayer = new MoviePlayer;
    P_GifWidget = new GifWidget("");

    //默认显示
    init();
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    ui->label_time->setText(current_date);
}

frmGifWidget::~frmGifWidget()
{
    delete ui;
}
void frmGifWidget::on_pushButton_movie_clicked()
{
    _moviePlayer->showFullScreen();
}

void frmGifWidget::on_pushButton_togif_clicked(bool checked)
{
    if(checked){
        //获取当前时间
        this->baseTime = this->baseTime.currentTime();
        this->pTimer->start(1);
        ui->pushButton_togif->setText("关闭");
        P_GifWidget->setGifRect();
        P_GifWidget->slotRecord(0);
        return;
    }else {
        this->pTimer->stop();
        P_GifWidget->slotRecord(1);
        ui->pushButton_togif->setText("开启");
        _endTime = QDateTime::currentDateTime().time().msec();
        return;
    }

}

void frmGifWidget::on_pushButton_test_clicked()
{
    P_GifWidget->showFullScreen();
}
//ms转换成时间
QString frmGifWidget::formatTime(int ms)
{
    int ss = 1000;
    int mi = ss * 60;
    int hh = mi * 60;
    int dd = hh * 24;

    long day = ms / dd;
    long hour = (ms - day * dd) / hh;
    long minute = (ms - day * dd - hour * hh) / mi;
    long second = (ms - day * dd - hour * hh - minute * mi) / ss;
    long milliSecond = ms - day * dd - hour * hh - minute * mi - second * ss;

    QString hou = QString::number(hour,10);
    QString min = QString::number(minute,10);
    QString sec = QString::number(second,10);
    QString msec = QString::number(milliSecond,10);

    //qDebug() << "minute:" << min << "second" << sec << "ms" << msec <<endl;

    return hou + ":" + min + ":" + sec ;
}

//初始化
void frmGifWidget::init()
{
    //默认显示
    this->pTimer = new QTimer;
    //绑定定时器的信号
    connect(this->pTimer,SIGNAL(timeout()),this,SLOT(updateDisplay()));
}

//更新时间
void frmGifWidget::updateDisplay()
{
    /*
     * 1.点击开始后获取到当前的时间并且赋值给baseTime
     * 2.启动定时器,间隔1s
     * 3.槽函数中再次获取当前的时间currTime
     * 4.计算两个时间的差值t
     * 5.声明一个showTime对象给他加上t的差值
     * 6.格式化后设置显示
     */
    QTime currTime = QTime::currentTime();
    int t = this->baseTime.msecsTo(currTime);
    QTime showTime(0,0,0,0);
    showTime = showTime.addMSecs(t);
    this->timeStr = showTime.toString("hh:mm:ss:zzz");
    ui->label_time->setText(timeStr);
}
