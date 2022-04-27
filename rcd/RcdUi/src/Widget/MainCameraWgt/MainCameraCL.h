#ifndef MainCameraCL_H
#define MainCameraCL_H

#include "Interface/IController.h"
#include "MainCameraModel.h"

class MainCameraCL : public IController
{
    Q_OBJECT
public:
    explicit MainCameraCL(QObject *parent = nullptr);
    ~MainCameraCL() override;

    void doInit() override;
    void doUnInit() override;
    bool intoAction() override;
    bool exitAction() override;
    void getFromAny(ModuleDir moduleDir,int cmd,QVariant data ) override;

    void setModel(MainCameraModel *model);
private:

    void inputHandler(Qt::Key key);

    MainCameraModel *m_pModel;
    bool isRemoteApply;
    int _currentStatus = 0;
};

#endif // MainCameraCL_H
