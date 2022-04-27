#ifndef SAVEGIFFORM_H
#define SAVEGIFFORM_H

#include <QWidget>
#include <QScopedPointer>
#include <mutex>
#include <QMap>
#include "movieplayer.h"
using namespace std;
namespace Ui {
class saveGifForm;
}
class saveGifForm : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int _screenRecord_L READ getScreenRecord_L WRITE setScreenRecord_L)
    Q_PROPERTY(int _screenRecord_M READ getScreenRecord_M WRITE setScreenRecord_M)
    Q_PROPERTY(int _screenRecord_R READ getScreenRecord_R WRITE setScreenRecord_R)

public:
    static saveGifForm *instance();
    bool getLeftScreenChecked() const;
    bool getMainScreenChecked() const;
    bool getRightScreenChecked() const;
    void leftStatus(QString str);
    void mainStatus(QString str);
    void rightStatus(QString str);

    void setScreenRecord_L(bool checked);
    void setScreenRecord_M(bool checked);
    void setScreenRecord_R(bool checked);
    bool getScreenRecord_L();
    bool getScreenRecord_M();
    bool getScreenRecord_R();
    void showForm();
private:
    explicit saveGifForm(QWidget *parent = nullptr);//构造函数
    saveGifForm(const saveGifForm &);//复制构造函数
    saveGifForm& operator=(const saveGifForm &);//赋值构造函数
    void init();
private slots:
    void slotUpdateChecked(bool);
    void slotUpdateLeftChecked(bool);
    void slotUpdateMainChecked(bool);
    void slotUpdateRightChecked(bool);
    void on_pushButton_ok_clicked(bool checked);

    void on_pushButton_cancel_clicked(bool checked);

signals:
    void signalGifForm(QMap<QString,bool>);//发送选中的屏幕录屏
private:
    static QScopedPointer<saveGifForm> P_saveGifForm;
    static mutex _mutex;
    Ui::saveGifForm *ui;
    bool _leftChecked;
    bool _mainChecked;
    bool _rightChecked;
    QMap<QString,bool> _screenMap;
    bool _screenRecord_L = false;
    bool _screenRecord_M = false;
    bool _screenRecord_R = false;
};

#endif // SAVEGIFFORM_H
