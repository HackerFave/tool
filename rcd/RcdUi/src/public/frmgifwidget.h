
#ifndef FRMGIFWIDGET_H
#define FRMGIFWIDGET_H

#include <QWidget>
#include <QTime>
class MoviePlayer;
class GifWidget;
class QTimer;
class QTime;
namespace Ui {
class frmGifWidget;
}

class frmGifWidget : public QWidget
{
    Q_OBJECT

public:
    explicit frmGifWidget(QWidget *parent = 0);
    ~frmGifWidget();

private slots:
    void on_pushButton_movie_clicked();

    void on_pushButton_togif_clicked(bool checked);
    void on_pushButton_test_clicked();
private:
    QString formatTime(int ms);
    void init();
signals:
    void signalRecord(int flag);
private:
    Ui::frmGifWidget *ui;
    MoviePlayer *_moviePlayer = nullptr;
    GifWidget * P_GifWidget = nullptr;
    //显示的时间
    unsigned int _num = 0;
    unsigned int _startTime = 0;
    unsigned int _endTime = 0;
    QTimer *pTimer;
    QTime baseTime;
    //显示的时间
    QString timeStr;
private slots:
    void updateDisplay();


};

#endif // FRMGIFWIDGET_H

