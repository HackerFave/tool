#ifndef BASETABLEVIEWCPU_H
#define BASETABLEVIEWCPU_H

#include <QStandardItemModel>
#include <QHeaderView>
#include <QTableView>
#include <QPushButton>
#include "idl/systemInfo.h"
#include "idl/systemInfoPubSubTypes.h"
#include "idl/systemInfoTypeObject.h"
#include "idl/commonIdlInfo.h"
#include "idl/commonIdlInfoTypeObject.h"
#include <QObject>
#include <QSharedPointer>
using namespace tg_msgs::dds;

class baseTableViewCPU : public QTableView
{
    Q_OBJECT
public:
    baseTableViewCPU();
    void setCPUDataInfo(const std::vector<MsgProcessST> &vec);
private slots:
    void slotKillProc();
signals:
    void signalKillProc(MsgKILLProcess tmp);
private:
    void initTableView();
    moudleType getMoudle(const QString &flag){
        if(flag=="Unknown"){
            return moudleType::UnknownModule;
        }
        else if (flag=="CCU") {
            return moudleType::CCU;
        }
        else if (flag=="CGU") {

            return  moudleType::CGU;
        }
        if(flag=="CGU_TBOX"){
            return moudleType::CGU_TBOX;
        }
        else if (flag=="HMI") {
            return moudleType::HMI;
        }
        else if (flag=="ICU") {

            return  moudleType::ICU;
        }
    }
    QStandardItemModel *_modelCpu = nullptr;
};

#endif // BASETABLEVIEWCPU_H
