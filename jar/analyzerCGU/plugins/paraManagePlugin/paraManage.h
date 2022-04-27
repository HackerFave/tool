#ifndef PARAMANAGE_H
#define PARAMANAGE_H

#include <QWidget>
#include "paraManage_global.h"
#include "basewidget.h"
namespace Ui {
class paraManage;
}

#ifdef Q_OS_WIN
class  paraManage : public BaseWidget
#else
class PLUGIN_PARAMANAGE_EXPORT paraManage : public BaseWidget
#endif
{
    Q_OBJECT

public:
    explicit paraManage(QWidget *parent = nullptr,const void* node = nullptr);
    ~paraManage();
public slots:
    Q_INVOKABLE void slotRecvBackendData(const std::string&) override;//从主框架获取数据
    Q_INVOKABLE void slotPluginWidgetsPointHash(QHash<QString,QWidget*>) override;//插件界面列表
private:
    Ui::paraManage *ui;
    const void* _node = nullptr;
};

#endif // PARAMANAGE_H
