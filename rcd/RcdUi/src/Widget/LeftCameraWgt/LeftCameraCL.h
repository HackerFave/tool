#ifndef LeftCameraCL_H
#define LeftCameraCL_H

#include "Interface/IController.h"
#include "LeftCameraModel.h"

class LeftCameraCL : public IController
{
    Q_OBJECT
public:
    explicit LeftCameraCL(QObject *parent = nullptr);
    ~LeftCameraCL() override;

    void doInit() override;
    void doUnInit() override;
    bool intoAction() override;
    bool exitAction() override;
    void getFromAny(ModuleDir moduleDir,int cmd,QVariant data ) override;

    void getButton(int numb);
    void setModel(LeftCameraModel *model);

private:

    LeftCameraModel *m_pModel;
};

#endif // LeftCameraCL_H
