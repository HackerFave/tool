#include "publicclass.h"
#include <QDir>
#include <QFile>
#include <QDebug>
publicClass* publicClass::P_publicClass = nullptr;
std::mutex publicClass::_mutex ;
publicClass::publicClass(QObject *parent) : QObject(parent)
{
    createFile(QCoreApplication::applicationDirPath()+"/"+"testdata.txt");
    QDir::setCurrent(QCoreApplication::applicationDirPath());
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
