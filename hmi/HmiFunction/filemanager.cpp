#include "filemanager.h"
#include "paraparsing.h"
#include "log.h"

FileManager* FileManager::handle = NULL;
QMutex FileManager::classMutex;

FileManager::FileManager(QObject *parent) : QObject(parent)
{

}

FileManager* FileManager::instance()
{
    if(handle == NULL) {
        QMutexLocker locker(&classMutex);
        if(handle == NULL) {
            handle = new FileManager();
        }
    }
    return handle;
}

void FileManager::getMapMutex()
{
    mapFileMutex.lock();
}

bool FileManager::tryGetMapMutex(int milliseconds)
{
    return mapFileMutex.tryLock(milliseconds);
}

void FileManager::releaseMapMutex()
{
    mapFileMutex.unlock();
}

void FileManager::getTaskMutex()
{
    taskFileMutex.lock();
}

void FileManager::getPermissionMutex()
{
    permissionFileMutex.lock();
}

bool FileManager::tryGetTaskMutex(int milliseconds)
{
    return taskFileMutex.tryLock(milliseconds);
}

void FileManager::releaseTaskMutex()
{
    taskFileMutex.unlock();
}

void FileManager::releasePermissionMutex()
{
    permissionFileMutex.unlock();
}

/**
 * @brief FileManager::fileDownLoadDeal 下载文件对外接口
 * @param remotePath 远端文件绝对路径
 * @param remoteMd5 远端md5
 * @param localDir 本地文件目录
 * @param fileMaxNum 本地文件最大个数
 * @param localFilePath 本文件绝对路径
 * @return
 */
int FileManager::fileDownLoadDeal(QString remotePath, char *remoteMd5, QString localDir, int fileMaxNum, QString &localFilePath)
{
    QString fileName;
    fileGetName(remotePath, fileName);
    localFilePath = localDir+fileName;

    QDir fileDir;
    fileDir.setPath(localDir);
    if(!fileDir.exists())
    {
        fileDir.mkdir(localDir);
    }


    if(fileIsExist(localDir, fileName, remoteMd5) == true)
        return MapUpdateMode_MD5_SAME;

    fileRemove(localDir, fileMaxNum);

    return fileGetRemote(remotePath, remoteMd5, localFilePath);
}

/**
 * @brief FileManager::fileGetName 获取文件名称
 * @param remotePath 文件远端路径
 * @param fileName 文件名称
 */
void FileManager::fileGetName(QString remotePath, QString &fileName)
{
    QStringList nameSplit = remotePath.split("/");
    fileName = nameSplit.last();
    LOG_DEBUG("file download dir[%s], file name[%s]\n", remotePath.toStdString().data(), fileName.toStdString().data());
}

/**
 * @brief FileManager::fileRemove 删除文件
 * @param localDir 文件目录
 * @param fileMaxNum 文件最大个数
 */
void FileManager::fileRemove(QString localDir, int fileMaxNum)
{
    int total = 1;
    QDir dir(localDir);

    QStringList fileList = dir.entryList(QDir::Files/*QDir::NoDot | QDir::NoDotDot | QDir::NoDotAndDotDot*/, QDir::Time);

    foreach (QString var, fileList) {
        if(++total > fileMaxNum) {
            dir.remove(var);
            LOG_DEBUG("file remove, fileName[%s]\n", dir.absoluteFilePath(var).toStdString().data());
        }
    }
}
/**
 * @brief FileManager::dirRemove 删除文件夹
 * @param localDir 文件目录
 * @param fileMaxNum 保留文件夹最大个数
 */
void FileManager::dirRemove(QString localDir, int fileMaxNum)
{
    int total = 1;

    QDir dir(localDir);

    QStringList fileList = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time);

    foreach (QString var, fileList) {
        if(++total > fileMaxNum) {
            LOG_DEBUG("dir remove, fileName[%s]\n", dir.absoluteFilePath(var).toStdString().data());
            deleteDir(dir.absoluteFilePath(var));
        }
    }
}
/**
 * @brief FileManager::deleteDir 删除制定文件夹
 * @param dirName 文件目录
 */
bool FileManager::deleteDir(const QString &dirName)
{
    QDir directory(dirName);
    if (!directory.exists())
    {
        return true;
    }


    QString srcPath = QDir::toNativeSeparators(dirName);
    if (!srcPath.endsWith(QDir::separator()))
        srcPath += QDir::separator();


    QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    bool error = false;
    for (QStringList::size_type i=0; i != fileNames.size(); ++i)
    {
        QString filePath = srcPath + fileNames.at(i);
        QFileInfo fileInfo(filePath);
        if (fileInfo.isFile() || fileInfo.isSymLink())
        {
            QFile::setPermissions(filePath, QFile::WriteOwner);
            if (!QFile::remove(filePath))
            {
                //qDebug() << "remove file" << filePath << " faild!";
                LOG_DEBUG("remove file[%s] failed\n", filePath.toStdString().data());
                error = true;
            }
        }
        else if (fileInfo.isDir())
        {
            if (!deleteDir(filePath))
            {
                error = true;
            }
        }
    }


    if (!directory.rmdir(QDir::toNativeSeparators(directory.path())))
    {
        //qDebug() << "remove dir" << directory.path() << " faild!";
        LOG_DEBUG("remove dir[%s] failed\n", directory.path().toStdString().data());
        error = true;
    }
    return !error;
}
/**
 * @brief FileManager::fileIsExist 判断文件是否载本地存在
 * @param localDir 本地文件目录
 * @param fileName 文件名称
 * @param remoteMd5 文件远端md5
 * @param localFilePath 本文件绝对路径
 * @return
 */
bool FileManager::fileIsExist(QString localDir, QString fileName, char *remoteMd5)
{
    QDir dir(localDir);

    QStringList fileList = dir.entryList(QDir::AllEntries | QDir::NoDot | QDir::NoDotDot | QDir::NoDotAndDotDot, QDir::Time);
    foreach (QString var, fileList) {
        if(var != fileName)
            continue;

        //char resultMd5[33] ={0};
        //Compute_file_md5(var.toStdString().data(), resultMd5);
        //LOG_DEBUG("file download exist, fileName[%s], remote md5[%s], local md5[%s]\n",
        //          var.toStdString().data(), QByteArray(remoteMd5, 32).data(), resultMd5);

        //if(strncmp(remoteMd5, resultMd5, 32) == 0) {
        //    return true;
        //}

        QString resultMd5 = fileMd5(localDir+var);
        LOG_DEBUG("file download exist, fileName[%s], remote md5[%s], local md5[%s]\n",
                  var.toStdString().data(), QByteArray(remoteMd5, 32).data(), resultMd5.toStdString().data());

        QString remoteMd5Str = QString(QByteArray(remoteMd5, 32));
        if(remoteMd5Str == resultMd5) {
            LOG_DEBUG("md5 same, no need to download return true.")
            return true;
        }

        dir.remove(fileName);
        return false;
    }
    LOG_DEBUG("file download no exist, fileName[%s]\n", fileName.toStdString().data());
    return false;
}

/**
 * @brief FileManager::fileGetRemote 从远端目录下载文件
 * @param remotePath 远端文件绝对路径
 * @param remoteMd5 远端文件md5
 * @param localPath 文件本地绝对路径
 * @return
 */
int FileManager::fileGetRemote(QString remotePath, char *remoteMd5, QString localPath)
{
    QString downloadCmd(QString("tftp-client ") + ParaParsing::instance()->paraSt.pub.remoteIp +
                        QString(" -c get ") + remotePath + QString(" ") + localPath);
    LOG_DEBUG("file download cmd[%s]\n", downloadCmd.toStdString().data());

    QProcess shell;
    shell.execute(downloadCmd);

    char resultMd5[33] = {0};
    Compute_file_md5(localPath.toStdString().data(), resultMd5);
    LOG_DEBUG("file download, remote md5[%s], local md5[%s]\n", QByteArray(remoteMd5, 32).data(), resultMd5);
    if(strncmp(remoteMd5, resultMd5, 32) == 0) {
        return MapUpdateMode_NEW_FILE;
    }
    QString cmd(QString("remove ") + localPath);
    shell.execute(cmd);
    return MapUpdateMode_FASLE;
}

QStringList FileManager::anaysisTask(bool flag, QString dir)
{
    QStringList lpxList;

    if(flag)
    {
        QDir taskDir;
        taskDir.setPath(dir);
        QFile taskFile;
        QTextStream taskStream;

        QStringList tasklist = taskDir.entryList();
        for(int count = 0; count < tasklist.size(); count++)
        {
            if(tasklist.at(count).endsWith(".txt"))
            {
                taskFile.setFileName(QString("%1/%2").arg(dir).arg(tasklist.at(count)));
                break;
            }
        }

        if(!taskFile.fileName().isEmpty())
        {
            if(taskFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                taskStream.setDevice(&taskFile);
                QString lineContext = taskStream.readLine();
                QStringList contextList = lineContext.split("\t");
                QString last = contextList.last();
                if(last.endsWith(".lpx"))
                {
                    lpxList.append(QString("%1/%2").arg(dir).arg(last));
                }

                taskFile.close();
            }
        }
    }

    return lpxList;
}

QStringList FileManager::anaysisPermission(bool flag, QString dir)
{
    QStringList lpxList;

    if(flag)
    {
        QDir permissionDir;
        permissionDir.setPath(dir);
        QFile permissionFile;
        QTextStream permissionStream;

        QStringList permissionList = permissionDir.entryList();
        for(int count = 0; count < permissionList.size(); count++)
        {
            if(permissionList.at(count).endsWith(".txt"))
            {
                permissionFile.setFileName(QString("%1/%2").arg(dir).arg(permissionList.at(count)));
                break;
            }
        }

        if(!permissionFile.fileName().isEmpty())
        {
            if(permissionFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                permissionStream.setDevice(&permissionFile);
                QString lineContext = permissionStream.readLine();
                QStringList contextList = lineContext.split("\t");
                QString last = contextList.last();
                if(last.endsWith(".lpx"))
                {
                    lpxList.append(QString("%1/%2").arg(dir).arg(last));
                }

                permissionFile.close();
            }
        }
    }

    return lpxList;
}

QString FileManager::fileMd5(const QString &sourceFiledirPath)
{
    QFile sourceFile(sourceFiledirPath);
    qint64 fileSize = sourceFile.size();
    const qint64 bufferSize = 10240;

    if (sourceFile.open(QIODevice::ReadOnly)) {
        char buffer[bufferSize];
        int bytesRead;
        int readSize = qMin(fileSize, bufferSize);

        QCryptographicHash hash(QCryptographicHash::Md5);

        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0) {
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize = qMin(fileSize, bufferSize);
        }

        sourceFile.close();
        return QString(hash.result().toHex());
    }
    return QString();
}
