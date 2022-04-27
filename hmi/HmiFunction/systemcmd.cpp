#include "systemcmd.h"

SystemCmd::SystemCmd(QObject *parent) : QObject(parent)
{

}

void SystemCmd::getMapCmd(QByteArray md5, QString cmd, QString name, QString mappath)
{
    qDebug("get the map database from gateway");
    qDebug()<<cmd;

    QProcess shell;
    shell.execute(cmd);


    QStringList namesplit = name.split("/");
    QString localname = namesplit.last();
    QString localfilepath;
    localfilepath.append(ParaParsing::instance()->paraSt.pub.mapPath+"/" + localname);
    char resultmd5[33];
    memset(resultmd5, 0 , 33);

    int ret = Compute_file_md5(localfilepath.toStdString().data(), resultmd5);

    if(ret < 0)//未完成校验
    {
        qDebug("compute the map md5 failed");
        emit getMapFile(false);
    }
    else//完成校验
    {
        if(!strncmp(md5.data(), resultmd5, 32))//校验一致
        {
            qDebug("get the database from gateway sucessful");

            QString untarcmd;
            QString renamecmd;

            untarcmd = QString("tar -zxvf "+ParaParsing::instance()->paraSt.pub.mapPath+"/*.tar.gz && rm *.tar.gz");
            system(untarcmd.toLatin1().data());
            renamecmd = QString("mv "+ParaParsing::instance()->paraSt.pub.mapPath+"/%1 "+
                                ParaParsing::instance()->paraSt.pub.mapPath+"/%2").arg(localname).arg(mappath);
            system(renamecmd.toLatin1().data());
            emit getMapFile(true);
        }
        else
        {
            qDebug("get the database from gateway false due to md5 error");
            emit getMapFile(false);
        }
    }
}

void SystemCmd::getTaskCmd(QByteArray md5, QString cmd, QString name)
{
    QDir taskDir;
    taskDir.setPath(ParaParsing::instance()->paraSt.pub.taskPath);

    if(taskDir.exists())//存在目录
    {
        QStringList dirList = taskDir.entryList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot | QDir::NoDotAndDotDot, QDir::Name);
        if(dirList.size() >= 3)
        {
            taskDir.rmdir(dirList.first());
        }
    }
    else//不存在创建目录
    {
        taskDir.mkdir(ParaParsing::instance()->paraSt.pub.taskPath);
    }

    QDateTime currenttime = QDateTime::currentDateTime();
    QString newDir = currenttime.toString("MM-dd_hh:mm:ss");

    QProcess shell;
    shell.execute(cmd);

    QStringList namesplit = name.split("/");
    QString localname = namesplit.last();
    QString localfilepath;
    localfilepath.append(QString(ParaParsing::instance()->paraSt.pub.taskPath+"/") + localname);
    char resultmd5[33];
    memset(resultmd5, 0 , 33);

    int ret = Compute_file_md5(localfilepath.toStdString().data(), resultmd5);
    if(ret < 0)//未完成校验
    {
        qDebug("compute the task md5 failed");
        emit getTaskFile(false, "");
    }
    else//完成校验
    {
        if(!strncmp(md5.data(), resultmd5, 32))//校验一致
        {
            qDebug("get the taskfile from gateway sucessful");

            QString untarcmd;

            untarcmd = QString("tar -zxvf "+ParaParsing::instance()->paraSt.pub.taskPath+"/*.tar.gz -C %1 && rm *.tar.gz").arg(newDir);
            system(untarcmd.toLatin1().data());

            emit getTaskFile(true, newDir);
        }
        else
        {
            qDebug("get the taskfile from gateway false due to md5 error");
            emit getTaskFile(false, "");
        }
    }
}

