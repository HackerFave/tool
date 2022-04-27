#ifndef DriveCameraCL_H
#define DriveCameraCL_H

#include "Interface/IController.h"

class DriveCameraCL : public IController
{
    Q_OBJECT
public:
    explicit DriveCameraCL(QObject *parent = nullptr);
    ~DriveCameraCL() override;

    void doInit() override;
    void doUnInit() override;
    bool intoAction() override;
    bool exitAction() override;
    void getFromAny(ModuleDir moduleDir,int cmd,QVariant data ) override;

    void getButton(int numb);

private:

};

#endif // DriveCameraCL_H
