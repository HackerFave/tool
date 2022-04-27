#ifndef AllInfoCL_H
#define AllInfoCL_H

#include "Interface/IController.h"
#include "AllInfoModel.h"

class AllInfoCL : public IController
{
    Q_OBJECT
public:
    explicit AllInfoCL(QObject *parent = nullptr);
    ~AllInfoCL() override;

    void doInit() override;
    void doUnInit() override;
    bool intoAction() override;
    bool exitAction() override;
    void getFromAny(ModuleDir moduleDir,int cmd,QVariant data ) override;

    void getButton(int numb);
    void setModel(AllInfoModel *model);

private:

    AllInfoModel *m_pModel;
};

#endif // AllInfoCL_H
