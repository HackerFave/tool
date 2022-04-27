#ifndef SYSTEMINFOPLUGIN_H
#define SYSTEMINFOPLUGIN_H

#include <QWidget>
#include "systemInfoPlugin_global.h"
#include "basewidget.h"
#include "workThread.h"
#include "base/baseTableViewCPU.h"
#include "base/baseTableViewDisk.h"
using namespace tg_msgs::dds;
namespace Ui {
class systemInfoPlugin;
}

class SYSTEMINFOPLUGIN_EXPORT systemInfoPlugin : public BaseWidget
{
    Q_OBJECT

public:
    explicit systemInfoPlugin(QWidget *parent = nullptr,const void* node = nullptr);
    ~systemInfoPlugin();
private slots:
    Q_INVOKABLE void slotRecvBackendData(const std::string& data) override;
    Q_INVOKABLE void slotPluginWidgetsPointHash(QHash<QString, QWidget *>) override;
    Q_INVOKABLE void slotSetThreadStatus(bool) override;
    void on_pushButton_clear_clicked();
    void slotShellCMDResult(const void*);
    void slotDataDiskMonitor(const void*);
    void slotDataCPUMEMMonitor(const void*);
    void slotLineEditPubData();
    void on_pushButton_backend_data_clicked();
    void slotKillProc(MsgKILLProcess tmp);

private:
    void initComboBox();

    QString getMoudle(const moudleType &flag){
        switch (flag) {
        case moudleType::UnknownModule:{

            return "Unknown";
        }
        case moudleType::CCU:{

            return "CCU";
        }
        case moudleType::CGU:{

            return "CGU";
        }
        case moudleType::CGU_TBOX:{

            return "CGU_TBOX";
        }
        case moudleType::HMI:{

            return "HMI";
        }
        case moudleType::ICU:{

            return "ICU";
        }
        }
    }
    Ui::systemInfoPlugin *ui;
    const void * _node = nullptr;
    QThread *_workThread = nullptr;
    workThread * P_workThread = nullptr;
    QHash<QString,baseTableViewCPU *> _hashTableViewCPU;
    QHash<QString,baseTableViewDisk *> _hashTableViewDisk;
    std::shared_ptr<TGPub<MsgShellCMD>> _MsgShellCMD_Write;
    std::shared_ptr<TGPub<MsgKILLProcess>> _MsgKILLProcess_Write;
    std::shared_ptr<TGPub<MsgClearAll>> _MsgClearAll_Write;


};

#endif // SYSTEMINFOPLUGIN_H
