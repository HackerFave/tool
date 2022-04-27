#include "baseTableViewCPU.h"
#include <QtDebug>
baseTableViewCPU::baseTableViewCPU()
{
    initTableView();
}
void baseTableViewCPU::initTableView()
{
    _modelCpu = new QStandardItemModel;
    /*设置列字段名*/

    QStringList headers_cpu;
    headers_cpu << "进程名" << "进程pid"<<"CPU使用率(%)"<<"内存使用率(%)"<<"操作";
    _modelCpu->setHorizontalHeaderLabels(headers_cpu);
    this->setModel(_modelCpu);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->verticalHeader()->hide();

    /* 设置列宽在可视界面自适应宽度 */
    QHeaderView* headerView_cpu = this->horizontalHeader();
    headerView_cpu->setSectionResizeMode(QHeaderView::Stretch);
    this->setAlternatingRowColors(true);
}

void baseTableViewCPU::setCPUDataInfo(const std::vector<MsgProcessST> &vec)
{
    while (_modelCpu->rowCount() > 0)
    {
        _modelCpu->removeRow(0);

    }
    for(int i = 0;i<vec.size();i++){
        _modelCpu->setItem(i, 0, new QStandardItem(vec.at(i).process().c_str()));//进程名 如/home/work/cloudClient
        _modelCpu->setItem(i, 1, new QStandardItem(QString::number(vec.at(i).pid()))); //进程pid
        _modelCpu->setItem(i, 2, new QStandardItem(QString::number(vec.at(i).pCPUUsage()))); // CPU使用率 0.0-100.0
        _modelCpu->setItem(i, 3, new QStandardItem(QString::number(vec.at(i).pMEMUsage())));//内存使用率 0.0-100.0
        _modelCpu->item(i,0)->setTextAlignment(Qt::AlignCenter);
        _modelCpu->item(i,1)->setTextAlignment(Qt::AlignCenter);
        _modelCpu->item(i,2)->setTextAlignment(Qt::AlignCenter);
        _modelCpu->item(i,3)->setTextAlignment(Qt::AlignCenter);
        QPushButton *button = new QPushButton("kill");
        this->setIndexWidget(_modelCpu->index(i, 4), button);
        connect(button,SIGNAL(clicked()),this,SLOT(slotKillProc()));

    }
}
//kill 进程
void baseTableViewCPU::slotKillProc()
{
    MsgKILLProcess tmp;
    tmp.moudle(static_cast<moudleType>(getMoudle(this->objectName().simplified())));//模块
    tmp.pid(_modelCpu->item(this->currentIndex().row(),1)->text().toLong()); //pid
    tmp.name(_modelCpu->item(this->currentIndex().row(),0)->text().toStdString());//进程名
    emit signalKillProc(tmp);
    _modelCpu->removeRow(this->currentIndex().row());

}
