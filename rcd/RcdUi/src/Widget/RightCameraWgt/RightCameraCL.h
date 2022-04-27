#ifndef RightCameraCL_H
#define RightCameraCL_H

#include "Interface/IController.h"
#include "RightCameraModel.h"

class RightCameraCL : public IController
{
    Q_OBJECT
public:
    explicit RightCameraCL(QObject *parent = nullptr);
    ~RightCameraCL() override;

    enum CLCmdType
    {
        CL_UpdateAll=0,
        CL_InputKey,
        CL_UpdateSelect,

        CL_Max
    };

    void doInit() override;
    void doUnInit() override;
    bool intoAction() override;
    bool exitAction() override;
    void getFromAny(ModuleDir moduleDir,int cmd,QVariant data ) override;

    void getButton(int numb);
    void setModel(RightCameraModel *model);

private:

    void inputHandler(Qt::Key key);

    RightCameraModel *m_pModel;
};

#endif // RightCameraCL_H
