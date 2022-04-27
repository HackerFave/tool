#include "HmiUI/uiMine/uimine.h"
#include "HmiUI/uiMineBaiYun/uiminebaiyun.h"
#include "HmiUI/uiCrush/uicrush.h"
#include "HmiUI/uiExcavator/uiexcavator.h"
#include "HmiUI/uiDigger/uidigger.h"
#include "HmiUI/uiCollector/uicollector.h"

#include "HmiFunction/paraparsing.h"

#include <QApplication>
#include <QTextCodec>

#include <QFile>
#include <QStyle>
#include <QMessageBox>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    QFile styleFile;
    styleFile.setFileName(":/style/resource/style.qss");
    if(styleFile.open(QIODevice::ReadOnly))
    {
        QString style = styleFile.readAll();
        a.setStyleSheet(style);
    }

//#if 1
    QWidget* w = NULL;
    ParaParsing* para = ParaParsing::instance();
    switch(para->paraSt.pub.vehicleType)
    {
        case 1://矿卡
        {
            w = new uiMine;
            break;
        }
        case 2://通用电铲
        case 9://永顺带4个参数的电铲
        {
            w = new uiExcavator;
            break;
        }
        case 3://推土机
        {
            w = new uiDigger;
            break;
        }
        case 4://采集车
        {
            w = new uiCollector;
            break;
        }
        case 7://停车场
        case 8://破碎站
        {
            w = new uiCrush;
            break;
        }
        case 10://白云矿卡
        {
            w = new uiMineBaiyun;
            break;
        }
        default:
        {
//            w = new uiCollector;
            QMessageBox::warning(NULL, QString::fromUtf8("警告"), QString("未知车辆类型"), QMessageBox::Ok);
            break;
        }
    }

    if(w != NULL)
        w->showFullScreen();
//        w->show();
//#endif
    return a.exec();
}
