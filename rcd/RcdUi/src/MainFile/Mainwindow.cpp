#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include <QCloseEvent>
#include <QPainter>
#include <QDebug>
#include <QScreen>
#include <QSettings>
#include "System.h"
#include "LeftWgt.h"
#include "MidWgt.h"
#include "RightWgt.h"
#include "public/saveGifForm.h"
#define USEMAIN false

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("RCD");
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowTitleHint );

    // 布局选择
#if USEMAIN
    QHBoxLayout * pHLayout = new QHBoxLayout();
    pHLayout->setContentsMargins(0,0,0,0);
    pHLayout->setSpacing(0);

    m_pLeftWgt = new  LeftWgt();
    m_pMidWgt = new  MidWgt();
    m_pRightWgt = new  RightWgt();

    pHLayout->addWidget(m_pLeftWgt,1);
    pHLayout->addWidget(m_pMidWgt,1);
    pHLayout->addWidget(m_pRightWgt,1);

    ui->Background->setLayout(pHLayout);
#else
    int procType = System::getProcessType();
    if(procType & PT_RightProc){
        m_pRightWgt = new  RightWgt();
    }
    if(procType & PT_MidProc){
        m_pMidWgt = new  MidWgt();
    }
    if(procType & PT_LeftProc){
        m_pLeftWgt = new  LeftWgt();
    }

#endif

}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_pLeftWgt;
    delete m_pMidWgt;
    delete m_pRightWgt;
    if(p_GifWidget_0){
        delete p_GifWidget_0;
    }
    if(p_GifWidget_1){
        delete p_GifWidget_1;
    }
    if(p_GifWidget_2){
        delete p_GifWidget_2;
    }
}
//分屏展示
void MainWindow::showAll()
{

#if USEMAIN
    this->show();
#else
    QList<QScreen *> allScreen =  QGuiApplication::screens();
    QSettings config("rcdconfig.ini",QSettings::IniFormat);
    int index = 0;

    int procType = System::getProcessType();
    if(procType & PT_RightProc){
        index = config.value(QString("ScreenNum/Right")).toInt();
        if(index < allScreen.count()){
            m_pRightWgt->setGeometry(allScreen[index]->geometry());
        }
        m_pRightWgt->showFullScreen();
    }
    if(procType & PT_MidProc){
        index = config.value(QString("ScreenNum/Mid")).toInt();
        if(index < allScreen.count()){
            m_pMidWgt->setGeometry(allScreen[index]->geometry());
        }
        m_pMidWgt->showFullScreen();
    }
    if(procType & PT_LeftProc){
        index = config.value(QString("ScreenNum/Left")).toInt();
        if(index < allScreen.count()){
            m_pLeftWgt->setGeometry(allScreen[index]->geometry());
        }
        m_pLeftWgt->showFullScreen();
    }

#endif
}
void MainWindow::showAllGif(QMap<QString,bool> map)
{
    QList<QScreen *> allScreen =  QGuiApplication::screens();
    QSettings config("rcdconfig.ini",QSettings::IniFormat);
    int index = 0;
    int indexL,indexM,indexR = 0;
    if(!p_GifWidget_0){
        p_GifWidget_0 = new GifWidget("left_",this);
    }
    if(!p_GifWidget_1){
        p_GifWidget_1 = new GifWidget("main_",this);
    }
    if(!p_GifWidget_2){
        p_GifWidget_2 = new GifWidget("right_",this);
    }
    if(allScreen.size()>=3){
        indexL = 2;
        indexM = 0;
        indexR = 1;
    }else {
        indexL = 0;
        indexM = 0;
        indexR = 0;
    }
    if(map.value("left")){
        if(!saveGifForm::instance()->getScreenRecord_L()){
                if(index < allScreen.count()){
                    p_GifWidget_0->setGeometry(allScreen[indexL]->geometry());
                }
                p_GifWidget_0->setGifRect();
                p_GifWidget_0->slotRecord(0);
                saveGifForm::instance()->leftStatus("录制中……");
                saveGifForm::instance()->setScreenRecord_L(true);
        }

    }
    else {
        if(saveGifForm::instance()->getScreenRecord_L()){
            p_GifWidget_0->slotRecord(1);
            saveGifForm::instance()->setScreenRecord_L(false);
            saveGifForm::instance()->leftStatus("");
        }
    }
    if(map.value("main")){
        if(!saveGifForm::instance()->getScreenRecord_M()){
                if(index < allScreen.count()){
                    p_GifWidget_1->setGeometry(allScreen[indexM]->geometry());
                }
                p_GifWidget_1->setGifRect();
                p_GifWidget_1->slotRecord(0);
                saveGifForm::instance()->mainStatus("录制中……");
                saveGifForm::instance()->setScreenRecord_M(true);
        }
    }
    else {
        if(saveGifForm::instance()->getScreenRecord_M()){
            p_GifWidget_1->slotRecord(1);
            saveGifForm::instance()->setScreenRecord_M(false);
            saveGifForm::instance()->mainStatus("");
        }
    }
    if(map.value("right")){
        if(!saveGifForm::instance()->getScreenRecord_R()){
                if(index < allScreen.count()){
                    p_GifWidget_2->setGeometry(allScreen[indexR]->geometry());
                }
                p_GifWidget_2->setGifRect();
                p_GifWidget_2->slotRecord(0);
                saveGifForm::instance()->rightStatus("录制中……");
                saveGifForm::instance()->setScreenRecord_R(true);
           // }
        }
    }
    else {
        if(saveGifForm::instance()->getScreenRecord_R()){
            p_GifWidget_2->slotRecord(1);
            saveGifForm::instance()->setScreenRecord_R(false);
            saveGifForm::instance()->rightStatus("");
        }
    }
}
void MainWindow::switchLayout(int layoutType)
{
    int procType = System::getProcessType();
    LayoutType layout = static_cast<LayoutType>(layoutType);
    switch (layout){
    case LT_Monitor:{//监控时布局
        if(procType & PT_LeftProc){
            m_pLeftWgt->setLayoutMonitor();
        }
        if(procType & PT_RightProc){
            m_pRightWgt->setLayoutMonitor();
        }
        break;
    }
    case LT_RemoteControl:{//遥控时布局
        if(procType & PT_LeftProc){
            m_pLeftWgt->setLayoutRemoteControl();
        }
        if(procType & PT_RightProc){
            m_pRightWgt->setLayoutRemoteControl();
        }
        break;
    }
    default:
        break;
    }
}

void MainWindow::addShowWidget(int type ,QWidget *pWgt)
{
    ScreenAreaType tType = static_cast<ScreenAreaType>(type);
    int procType = System::getProcessType();
    switch (tType) {
    case SA_MainScreen:{
        if(procType & PT_MidProc){
            m_pMidWgt->addShowWidget(type,pWgt);
        }
    }break;
    case SA_RightScreenL:
    case SA_RightScreenR:{
        if(procType & PT_RightProc){
            m_pRightWgt->addShowWidget(type,pWgt);
        }
    }break;
    case SA_LeftScreenTopL:
    case SA_LeftScreenTopR:
    case SA_LeftScreenBottom:{
        if(procType & PT_LeftProc){
            m_pLeftWgt->addShowWidget(type,pWgt);
        }
    }break;

    default:break;
    }
}
