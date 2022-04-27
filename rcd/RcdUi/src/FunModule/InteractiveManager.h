#ifndef InteractiveManager_H
#define InteractiveManager_H

#include <QMap>
#include "Interface/IController.h"

/* 交互管理模块 ： 所有交互信息先经过本模块处理，
 * 再发往当前可交互页面。同时管理跨进程的可交互页面的
 * 焦点切换。
 */

class InteractiveManager : public IController
{
    Q_OBJECT
public:
    explicit InteractiveManager(QObject *parent = nullptr);
    ~InteractiveManager() override;

    static InteractiveManager *GetInstance(QObject *parent = nullptr);

    void getFromAny(ModuleDir moduleDir,int cmd,QVariant data ) override;


private:

    static InteractiveManager       *s_pInstance;
    ModuleType                      m_focusModule;
    QMap<QString,ModuleType>        m_focusRelationship;
};

#define  pInteractiveIns  InteractiveManager::GetInstance()

#endif // InteractiveManager_H
