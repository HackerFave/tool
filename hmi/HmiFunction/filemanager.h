#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QMutex>
#include <QString>
#include <QCryptographicHash>
#include <QIODevice>
#include <QTextStream>

#include "systemcmd.h"
#include "../pub.h"

class FileManager : public QObject
{
    Q_OBJECT
public:    
    static FileManager* instance();
    void getMapMutex();
    bool tryGetMapMutex(int milliseconds = 0);
    void releaseMapMutex();
    void getTaskMutex();
    void getPermissionMutex();
    bool tryGetTaskMutex(int milliseconds = 0);
    void releaseTaskMutex();
    void releasePermissionMutex();

    int fileDownLoadDeal(QString remotePath, char *remoteMd5, QString localDir, int fileMaxNum, QString &localFilePath);
    void fileGetName(QString remotePath, QString &fileName);
    void fileRemove(QString localDir, int fileMaxNum);
    void dirRemove(QString localDir, int fileMaxNum);
    bool deleteDir(const QString &dirName);
    bool fileIsExist(QString localDir, QString fileName, char *remoteMd5);
    int fileGetRemote(QString remotePath, char *remoteMd5, QString localPath);


    QStringList anaysisTask(bool flag, QString dir);
    QStringList anaysisPermission(bool flag, QString dir);
    bool unTarMapFile(QString path);
    bool unTarTaskFile(QString path);
    QString fileMd5(const QString &sourceFiledirPath);

private:
    QMutex mapFileMutex;
    QMutex taskFileMutex;
    QMutex permissionFileMutex;

    static FileManager *handle;
    static QMutex classMutex;


private:
    explicit FileManager(QObject *parent = 0);

signals:

public slots:

};

#endif // FILEMANAGER_H
