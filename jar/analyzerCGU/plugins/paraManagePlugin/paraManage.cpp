#include "paraManage.h"
#include "ui_paraManage.h"

paraManage::paraManage(QWidget *parent,const void* node ) :
    BaseWidget(parent),
    _node(node),
    ui(new Ui::paraManage)
{
    ui->setupUi(this);
}

paraManage::~paraManage()
{
    delete ui;
}
Q_INVOKABLE void paraManage::slotRecvBackendData(const std::string& data)
{

}
Q_INVOKABLE void paraManage::slotPluginWidgetsPointHash(QHash<QString,QWidget*> widgetsHash)
{

}
