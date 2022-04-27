#include "baseTableViewDisk.h"

baseTableViewDisk::baseTableViewDisk()
{
    initTableView();
}
void baseTableViewDisk::initTableView()
{
    _modelDisk = new QStandardItemModel;
    /*设置列字段名*/

    QStringList headers_disk;
    headers_disk << "分区名" << "挂载路径"<<"总大小"<<"使用大小"<<"使用率";
    _modelDisk->setHorizontalHeaderLabels(headers_disk);
    this->setModel(_modelDisk);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->verticalHeader()->hide();
    /* 设置列宽在可视界面自适应宽度 */
    QHeaderView* headerView_disk = this->horizontalHeader();
    headerView_disk->setSectionResizeMode(QHeaderView::Stretch);
    this->setAlternatingRowColors(true);
}
void baseTableViewDisk::setDiskDataInfo(const std::vector<MsgDiskStatST> &vec)
{
    while (_modelDisk->rowCount() > 0)
    {
        _modelDisk->removeRow(0);

    }
    for(int i = 0;i<vec.size();i++){
        _modelDisk->setItem(i, 0, new QStandardItem(vec.at(i).fileSystem().c_str()));//分区名 如/dev/mmcblk3p1
        _modelDisk->setItem(i, 1, new QStandardItem(vec.at(i).mountedOn().c_str())); //挂载路径 如/home/work
        _modelDisk->setItem(i, 2, new QStandardItem(vec.at(i).size().c_str())); //总大小 如29G 847M
        _modelDisk->setItem(i, 3, new QStandardItem(vec.at(i).used().c_str()));//使用大小 如9.8G 4.0K
        _modelDisk->setItem(i, 4, new QStandardItem(vec.at(i).usage().c_str()));//使用率 如37%
        _modelDisk->item(i,0)->setTextAlignment(Qt::AlignCenter);
        _modelDisk->item(i,1)->setTextAlignment(Qt::AlignCenter);
        _modelDisk->item(i,2)->setTextAlignment(Qt::AlignCenter);
        _modelDisk->item(i,3)->setTextAlignment(Qt::AlignCenter);
        _modelDisk->item(i,4)->setTextAlignment(Qt::AlignCenter);
    }
}
