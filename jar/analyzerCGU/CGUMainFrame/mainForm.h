#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include <QDir>
#include "backend/dataInfoThread.h"
#include "backend/initThread.h"
#include "backend/workThread.h"
#include "public/publicClass.h"
#include <QProcess>
#include <QDateTime>
#include "dds_wrapper/include/node.hpp"
#include "backend/workThread.h"
#include "message/outputForm.h"
QT_BEGIN_NAMESPACE
namespace Ui { class mainForm; }
QT_END_NAMESPACE
//#define PATH "../CGUMainFrame/plugins"
class mainForm : public QMainWindow
{
    Q_OBJECT

public:
    mainForm(QWidget *parent = nullptr);
    ~mainForm();
    void writeOutPut(const QString &str);
    static void logOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
private slots:
    void slotAction(bool);
    void slotActionOutput(bool);//输出打印界面
    void slotStartThread(void*);
    void slotBackendData(const std::string&);
    void slotCloseTab(const int& index);
    void slotPluginToBackendData(const std::string&);//发送数据到主框架
    void slotNewPlugin(const QString &);

private:
    void initUi();
    void initOtherForm();//初始化其他界面
    QHash<QString,QWidget*> _widgetHash;
    QHash<QString,QString> _pluginNamePath;
    QHash<QString,QWidget*> _testWidgets;
signals:
    void signalUploadBackendData(const std::string&);//推送后台数据到插件
    void signalPluginWidgetsPointHash(QHash<QString,QWidget*>);//插件界面列表
    void signalSetThreadStatus(bool);
    void signalNewPlugin(const QString &str);//重新初始化插件

private:
    Ui::mainForm *ui;
    static mainForm *P_mainForm;
    QThread * _thread = nullptr;
    dataInfoThread * P_dataInfoThread = nullptr;
    QThread *_threadInit = nullptr;
    initThread * P_initThread = nullptr;
    QProcess _process;
    const Node * _node = nullptr;//监控node
    QThread *_workThread = nullptr;
    workThread *P_workThread = nullptr;
    QAction * _actionOutputLog = nullptr;
    outputForm * _outputForm = nullptr;
};
#endif // MAINFORM_H
