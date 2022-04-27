#ifndef CloudWebCL_H
#define CloudWebCL_H

#include "Interface/IController.h"
#include "CloudWebModel.h"

class CloudWebCL : public IController
{
    Q_OBJECT
public:
    explicit CloudWebCL(QObject *parent = nullptr);
    ~CloudWebCL() override;

    void doInit() override;
    void doUnInit() override;
    bool intoAction() override;
    bool exitAction() override;
    void getFromAny(ModuleDir moduleDir,int cmd,QVariant data ) override;

    void getButton(int numb);
    void setModel(CloudWebModel *model);

private:

    CloudWebModel *m_pModel;
};

#endif // CloudWebCL_H
