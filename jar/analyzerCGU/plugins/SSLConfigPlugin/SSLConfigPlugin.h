#ifndef SSLCONFIGPLUGIN_H
#define SSLCONFIGPLUGIN_H

#include <QWidget>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "basewidget.h"
#include "SSLConfigPlugin_global.h"
#include "idl/SignatureSSL.h"
#include "idl/SignatureSSLPubSubTypes.h"
#include "idl/SignatureSSLTypeObject.h"
#include "dds_wrapper/include/node.hpp"
#include <QFile>
#include <QFileDialog>
#include <QProcess>
#include <openssl/ssl.h>
#include <openssl/err.h>
using namespace tg_msgs::dds;
using namespace std;
namespace Ui {
class SSLConfigPlugin;
}

class SSLCONFIGPLUGIN_EXPORT SSLConfigPlugin : public BaseWidget
{
    Q_OBJECT

public:
    explicit SSLConfigPlugin(QWidget *parent = nullptr,const void* node = nullptr);
    ~SSLConfigPlugin();
    static void callBackDataFileImportStatus(const FileImportStatus &tmp);
public slots:
    Q_INVOKABLE void slotRecvBackendData(const std::string&) override;//从主框架获取数据
    Q_INVOKABLE void slotPluginWidgetsPointHash(QHash<QString,QWidget*>) override;//插件界面列表
private slots:
    void on_pushButton_upload_file_clicked();

    void on_pushButton_read_file_clicked();
    void slotReadyReadStandardOutputInfo();
    void slotReadyReadStandardErrorInfo();
private:
    void initPoint();
    void writeFileDataToBackend(QString filename);
    bool MakePemSSL(const char* cerFilePath, const char* savePemFilePath);

    Ui::SSLConfigPlugin *ui;
    static SSLConfigPlugin * P_SSLConfigPlugin;
    const void * _node = nullptr;
    std::shared_ptr<TGPub<FileCaStatus>> _FileCaStatus_Write;
    std::shared_ptr<TGPub<FileDataStream>> _FileDataStream_Write;
    std::shared_ptr<TGSub<FileImportStatus>> _FileImportStatus_Read;
//    QProcess *_process = nullptr;
    bool _isRemove = false;
};

#endif // SSLCONFIGPLUGIN_H
