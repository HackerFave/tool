#include "SSLConfigPlugin.h"
#include "ui_SSLConfigPlugin.h"
#include <QtDebug>
SSLConfigPlugin * SSLConfigPlugin::P_SSLConfigPlugin = nullptr;
SSLConfigPlugin::SSLConfigPlugin(QWidget *parent,const void *node) :
    BaseWidget(parent),
    _node(node),
    ui(new Ui::SSLConfigPlugin)
{
    ui->setupUi(this);
    initPoint();
    setAttribute(Qt::WA_DeleteOnClose);
    qDebug()<<"[ 插件：]"<<"SSLConfigPlugin";
    void * sslnode = const_cast<void*>(_node);
    //
    _FileImportStatus_Read =static_cast<Node*>(sslnode)->CreateReader<FileImportStatusPubSubType>(Topic_FileImportStatus);
    _FileImportStatus_Read->SetCallBack(callBackDataFileImportStatus);
    _FileDataStream_Write = static_cast<Node*>(sslnode)->CreateWriter<FileDataStreamPubSubType>(Topic_FileDataStream, QOS_PROFILE_EVENT_LOCAL);
    _FileCaStatus_Write = static_cast<Node*>(sslnode)->
            CreateWriter<FileCaStatusPubSubType>(Topic_CA_Status, QOS_PROFILE_EVENT_LOCAL);
    //    _process = new QProcess(0);

    //    connect(_process,SIGNAL(readyReadStandardOutput()),this,SLOT(slotReadyReadStandardOutputInfo()));
    //    connect(_process,SIGNAL(readyReadStandardError()),this,SLOT(slotReadyReadStandardErrorInfo()));
}

SSLConfigPlugin::~SSLConfigPlugin()
{
    qDebug()<<"【子界面正在析构：SSLConfigPlugin】";
    delete ui;
}
void SSLConfigPlugin::initPoint()
{
    P_SSLConfigPlugin = this;
}
Q_INVOKABLE void SSLConfigPlugin::slotRecvBackendData(const std::string& data)
{

}
Q_INVOKABLE void SSLConfigPlugin::slotPluginWidgetsPointHash(QHash<QString,QWidget*> widgetsHash)
{

}
void SSLConfigPlugin::on_pushButton_upload_file_clicked()
{
    //括号里的参数分别是：指定父类、标题、默认打开后显示的目录、右下角的文件过滤器。
    QString file_name = QFileDialog::getOpenFileName(NULL,"标题",".","*.txt *.cer *.pem *.crt");
    QFileInfo info(file_name);
    QString savefile = file_name;
    if(info.suffix()=="cer"){
        //        QString cmd = QString( "openssl x509 -inform der -in %1 -out %2CA_Backup.pem").arg(file_name).arg(QApplication::applicationDirPath()+"/");

        //        QStringList options;
        //        options << "-c" << cmd;
        //        _process->start("/bin/bash",options);
        //        file_name = QApplication::applicationDirPath()+"/CA_Backup.pem";
        savefile = QApplication::applicationDirPath()+"/CA_Backup.pem";
        MakePemSSL(file_name.toLocal8Bit(),savefile.toLocal8Bit());
        _isRemove = true;
    }
    //    else if (info.suffix()=="crt") {
    //        QString cmd = QString("openssl x509 -in %1 -out %2CA_Backup.pem -outform PEM").arg(file_name).arg(QApplication::applicationDirPath()+"/");
    //        QStringList options;
    //        options << "-c" << cmd;
    //        _process->start("/bin/bash",options);
    //        //        qDebug()<<"[cmd------>]"<<cmd.toLocal8Bit()<<"\n"<<"[filename----->]"<<file_name;
    //        file_name = QApplication::applicationDirPath()+"/CA_Backup.pem";

    //    }
    //    _process->waitForStarted();

    writeFileDataToBackend(/*file_name*/savefile);
}

void SSLConfigPlugin::on_pushButton_read_file_clicked()
{

}
void SSLConfigPlugin::callBackDataFileImportStatus(const FileImportStatus &tmp)
{
    if(tmp.imortStatus()){
        qDebug()<<"[backend---FileImportStatus:]"<<tmp.imortStatus()<<"[文件导入成功]";
        QFile file(QApplication::applicationDirPath()+"/CA_Backup.pem");
        if(P_SSLConfigPlugin->_isRemove){
            if(file.remove()){
                P_SSLConfigPlugin->_isRemove =false;
                qDebug()<<"[文件删除成功]";
                FileCaStatus data;
                data.CAStatus(true);
               P_SSLConfigPlugin->_FileCaStatus_Write->Write(data);
            }
            else {
                qDebug()<<"[error:]"<<"文件删除失败！";
                P_SSLConfigPlugin->_isRemove =false;
            }
        }
    }

}
void SSLConfigPlugin::slotReadyReadStandardOutputInfo()
{
    //    ui->textBrowser_out_put->setText(_process->readAllStandardOutput());
    //    qDebug()<<"[bash$ error:]"<<_process->readAllStandardOutput();
}
void SSLConfigPlugin::slotReadyReadStandardErrorInfo()
{
    //    ui->textBrowser_out_put->setText(_process->readAllStandardError());
    //    qDebug()<<"[bash$ output:]"<<_process->readAllStandardError();
}
void SSLConfigPlugin::writeFileDataToBackend(QString filename)
{
    FileDataStream tmp;
    QFile file(filename);
    qDebug()<<filename;
    if(file.exists()){
        qDebug()<<"文件创建成功---";
    }
    if(file.open(QIODevice::ReadOnly)){

        QByteArray data = file.readAll();
        char * dataArr = new char[data.size()];
        strcpy(dataArr,data);
        tmp.ca_data(dataArr);
        _FileDataStream_Write->Write(tmp);
        ui->textBrowser_out_put->setText(QString("filename---->%1").arg(filename)+"\n"+ tmp.ca_data().c_str());
        file.close();
        delete[] dataArr;
    }
    else {
        qDebug()<<"[error:]"<<file.errorString()<<file.error();
    }
}
bool SSLConfigPlugin::MakePemSSL(const char* cerFilePath, const char* savePemFilePath )
{
    int      ret = 0;
    X509 *x509 = NULL;
    FILE    *cerFile = NULL, *pemFile = NULL;

    cerFile = fopen(cerFilePath, "rb");
    if (cerFile == NULL) {
        fprintf(stderr, "MakeLocalPemSSL fopen cerFilePath err \n");
        goto free_all;
    }

    pemFile = fopen(savePemFilePath, "w+");
    if (pemFile == NULL) {
        fprintf(stderr, "MakeLocalPemSSL fopen savePemFilePath err \n");
        goto free_all;
    }

    x509 = d2i_X509_fp(cerFile, NULL);
    if (x509 == NULL) {
        fprintf(stderr, "MakeLocalPemSSL failed to parse to X509 from cerFile \n");
        goto free_all;
    }

    ret = PEM_write_X509(pemFile, x509);
    if (ret != 1) {
        fprintf(stderr, "MakeLocalPemSSL failed to PEM_write_X509 \n");
        goto free_all;
    }

free_all:
    if (cerFile) fclose(cerFile);
    if (pemFile) fclose(pemFile);

    return (ret == 1);
}
