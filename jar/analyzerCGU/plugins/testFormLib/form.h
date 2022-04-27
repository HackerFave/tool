#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "form_global.h"
#include <QThread>
#include "thread/workThread.h"
#include "thread/testThread.h"
#include <QMetaType>
#include <QStandardItemModel>
//#include "sqlModule.h"
#include "basewidget.h"
#define INFODATA std::map<std::string,std::list<std::string>>
namespace Ui {
class Form;
}

#ifdef Q_OS_WIN
class /*FORM_EXPORT*/ Form : public BaseWidget
#else
class FORM_EXPORT Form : public BaseWidget
#endif

{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr,const void *node = nullptr);
    ~Form();
private slots:
    void slotDataInfo(std::map<std::string,std::list<std::string>>,std::list<std::string>);
    void slotFillTableView(std::map<std::string,std::list<std::string>>,std::list<std::string>);
    void slotTest(QString data);
    void OnlineTreeViewDoubleClick(const QModelIndex);
    Q_INVOKABLE void slotRecvBackendData(const std::string& data) override;
    Q_INVOKABLE void slotPluginWidgetsPointHash(QHash<QString, QWidget *>) override;
private:
    void initUi();
    Ui::Form *ui;
    QStandardItemModel* _treeview_model = nullptr;
    QStandardItemModel* _tableview_model = nullptr;
    workThread *_Pthread = nullptr;
    QThread *_q_thread = nullptr;
    testThread *_Pthread_1 = nullptr;
    QThread *_q_thread_1 = nullptr;
    QMap<QString,QStringList> _mapInfo;
    bool _isClick = false;
    const void *_node;
    //    SqlModule _sql;
};

#endif // FORM_H
