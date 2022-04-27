#ifndef DATAPOST_H
#define DATAPOST_H

#include "baseProduct.h"
#include "node.h"

using namespace std;
class dataPost : public baseProduct
{
public:
    dataPost();
    Q_INVOKABLE void initPoint() override;
public slots:
    Q_INVOKABLE void slotPostTopicDataChild(const void*,size_t,int ) override;
private:
    datax::Node _sender;
    static dataPost *P_dataPost;
};

#endif // DATAPOST_H
