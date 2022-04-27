#ifndef WarnInfoCL_H
#define WarnInfoCL_H

#include "Interface/IController.h"
#include "WarnInfoModel.h"

class WarnInfoCL : public IController
{
    Q_OBJECT
public:
    explicit WarnInfoCL(QObject *parent = nullptr);
    ~WarnInfoCL() override;

    void doInit() override;
    void doUnInit() override;
    bool intoAction() override;
    bool exitAction() override;
    void getFromAny(ModuleDir moduleDir,int cmd,QVariant data ) override;

    void getButton(int numb);
    void setModel(WarnInfoModel *model);

private:

    WarnInfoModel *m_pModel;
};

#endif // WarnInfoCL_H
