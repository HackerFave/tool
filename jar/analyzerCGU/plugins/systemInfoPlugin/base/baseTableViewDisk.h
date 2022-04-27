#ifndef BASETABLEVIEWDISK_H
#define BASETABLEVIEWDISK_H

#include <QStandardItemModel>
#include <QHeaderView>
#include <QTableView>
#include "idl/systemInfo.h"
#include "idl/systemInfoPubSubTypes.h"
#include "idl/systemInfoTypeObject.h"
using namespace tg_msgs::dds;
class baseTableViewDisk : public QTableView
{
public:
    baseTableViewDisk();
    void setDiskDataInfo(const std::vector<MsgDiskStatST> &vec);
private:
    void initTableView();
    QStandardItemModel *_modelDisk = nullptr;
};

#endif // BASETABLEVIEWDISK_H
