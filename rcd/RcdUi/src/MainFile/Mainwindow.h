#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include "public/frmgifwidget.h"
#include "public/gifwidget.h"

class LeftWgt;
class MidWgt;
class RightWgt;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void switchLayout(int layoutType);
    void addShowWidget(int type ,QWidget *pWgt);
    void showAll();
    void showAllGif(QMap<QString,bool> map);

private:
    Ui::MainWindow *ui;
    LeftWgt         *m_pLeftWgt;
    MidWgt          *m_pMidWgt;
    RightWgt        *m_pRightWgt;
    GifWidget* p_GifWidget_0 = nullptr;//
    GifWidget* p_GifWidget_1 = nullptr;//
    GifWidget* p_GifWidget_2 = nullptr;//
};

#endif // MAINWINDOW_H
