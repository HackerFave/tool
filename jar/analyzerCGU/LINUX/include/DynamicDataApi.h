#ifndef DYNAMICDATASUBSCRIBER_DYNAMICDATAAPI_H
#define DYNAMICDATASUBSCRIBER_DYNAMICDATAAPI_H

#include "DynamicDataSubscriber.h"

class DynamicDataApi
{
public:
    using CallBackFunc = std::function<void(const std::string &)>;
     DynamicDataApi();
     virtual ~DynamicDataApi();

     void Init();

     void StartRun();

    void GetJsonString(std::string& json_string);
    void RegisterCallBack(const CallBackFunc& call_back_);

private:
    void Run();
    DynamicDataSubscriber dynamic_sub_;
    CallBackFunc call_back_;
};


#endif //DYNAMICDATASUBSCRIBER_DYNAMICDATAAPI_H
