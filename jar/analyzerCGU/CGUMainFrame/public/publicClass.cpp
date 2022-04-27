#include "publicClass.h"
#include <QDir>
#include <QFile>
#include <QDebug>

publicClass* publicClass::P_publicClass = nullptr;
std::mutex publicClass::_mutex ;
publicClass::publicClass(QObject *parent) : QObject(parent)
{
    //    createFile(QCoreApplication::applicationDirPath()+"/"+"testdata.txt");
    //    QDir::setCurrent(QCoreApplication::applicationDirPath());
}
publicClass::~publicClass()
{
    delInstance();
}
QString  publicClass::getExePath()
{
      return QCoreApplication::applicationDirPath();
}

bool publicClass::createFile(const QString &filename)//创建文件
{
    QFile file(filename);
    if(file.exists()){
        return true;
    }
    if(file.open(QIODevice::ReadWrite)){
        file.close();
        return true;
    }
}
bool publicClass::createFileDir(const QString &dirName,C_DirMode flag)//创建文件夹
{
    QDir *folder = new QDir;
    bool exist = folder->exists(dirName);
    if(exist)
    {
        return true;
    }
    else
    {
        //创建文件夹
        switch (flag) {
        case C_DirMode::dir:{
            return folder->mkdir(dirName);
        }
        case C_DirMode::dirpath:{
            return folder->mkpath(dirName);
        }
        default:
            break;
        }
        return  false;
    }
}
bool publicClass::writeIntoFile(const QString &filename,const QByteArray& data)//写入文件
{
    QFile file(filename);
    if(!file.exists()){
        return false;
    }
    if(file.open(QIODevice::WriteOnly|QIODevice::Append)){
        QByteArray ndata = data +'\n';
        file.write(ndata);
        file.close();
        return  true;
    }

}
QByteArray publicClass::readFileData(QString filename)//读取文件数据
{
    QFile file(filename);
    if(!file.exists()){
        return QByteArray();
    }
    else {
        if(file.open(QIODevice::ReadOnly)){
            return  file.readAll();
        }
    }
    return QByteArray();
}
// QString >> QJson
QJsonObject publicClass::getJsonObjectFromString(const QString jsonString)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data());
    if( jsonDocument.isNull() ){
        qDebug()<< "===> please check the string "<< jsonString.toLocal8Bit().data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}
// QJson >> QString
QString publicClass::getStringFromJsonObject(const QJsonObject& jsonObject)
{
    return QString(QJsonDocument(jsonObject).toJson());
}


void publicClass::parseQJsonObject(QJsonObject data)
{

    QVariantMap map = data.toVariantMap();

    QVariantMap ::iterator it;
    for(it = map.begin();it!=map.begin();it++){
//        qDebug()<<it.key()
    }
}
QString publicClass::getValueBykey(QJsonObject obj, QString key)

{

    QString rst = "";
    QStringList strList = obj.keys();
    for (int i = 0; i < obj.size(); i++)
    {
        QJsonValue val = obj.value(strList.at(i));
        if ( /*strList.at(i) == key && */val.isString())
        {
            rst = val.toString();
        }

        else if (val.isObject())
        {
            rst = getValueBykey(val.toObject(), key);
        }
    }
    return rst;

}

