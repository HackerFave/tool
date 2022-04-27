#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QScrollBar>
#include <QFile>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    initCombox();
    _process.start("bash");
    _process.waitForStarted();
    connect(&_process,SIGNAL(readyReadStandardOutput()),this,SLOT(readBashStandardOutputInfo()));
    connect(&_process,SIGNAL(readyReadStandardError()),this,SLOT(readBashStandardErrorInfo()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_bashrun_clicked()
{
    QFile file(QApplication::applicationDirPath()+QString("/%1.sym").
               arg(ui->comboBox_bashcmd->currentText().simplified()));
    if(!file.exists()){

        if(!file.open(QIODevice::ReadWrite)){
            return;
        }

    }
    file.close();
    QString strCmd = QString("dump_syms %1 >%1.sym").arg(ui->comboBox_bashcmd->currentText().simplified());
    ui->textEdit_bashmsg->append("Linux:~$ "+strCmd);
    _process.write(strCmd.toLocal8Bit() + '\n');

    strCmd = QString("cat %1.sym").arg(ui->comboBox_bashcmd->currentText().simplified());
    _process.write(strCmd.toLocal8Bit() + '\n');

}

void MainWindow::readBashStandardOutputInfo()
{
    QByteArray cmdout = _process.readAllStandardOutput();
    if(!cmdout.isEmpty()){
        ui->textEdit_bashmsg->append(QString::fromLocal8Bit(cmdout));
    }
    QScrollBar* scroll = ui->textEdit_bashmsg->verticalScrollBar();
    scroll->setSliderPosition(scroll->maximum());
}

void MainWindow::readBashStandardErrorInfo()
{
    QByteArray cmdout = _process.readAllStandardError();
    if(!cmdout.isEmpty()){
        ui->textEdit_bashmsg->append(QString::fromLocal8Bit(cmdout));
    }
    QScrollBar* scroll = ui->textEdit_bashmsg->verticalScrollBar();
    scroll->setSliderPosition(scroll->maximum());
}

void MainWindow::init()
{
    qDebug()<<QApplication::applicationDirPath()<<"*****************";

    QDir path;
    path.setPath(QApplication::applicationDirPath()+"/symbols");
    path.removeRecursively();
    if(!path.exists()){
        if(path.mkpath(QApplication::applicationDirPath()+"/symbols")){
            qDebug()<<"***********初始化崩溃文件夹成功***************";
        }
    }


}
void MainWindow::initCombox()
{
    ui->comboBox_bashcmd->addItem("CGUMainFrame");
    bool bIsDir;
    QStringList filters;
    filters << "*.so*";
    QDir dir(QApplication::applicationDirPath());
    //    QFileInfoList fileinfolist = dir.entryInfoList(filters, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    //以时间排序
    QFileInfoList fileinfolist = dir.entryInfoList(filters, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Time);
    QFileInfoList::iterator iterator = fileinfolist.begin();
    while (iterator != fileinfolist.end())
    {
        bIsDir = iterator->isDir();
        if (bIsDir)
        {

        }else
        {
            ui->comboBox_bashcmd->addItem(iterator->fileName());
        }

        iterator++;
    }

}
void MainWindow::initFileList()
{
    _fileList.clear();
    bool bIsDir;
    QStringList filters;
    filters << "*.log" << "*.dmp" << "*.jpg";
    QDir dir(QApplication::applicationDirPath()+"/crashes");
    //    QFileInfoList fileinfolist = dir.entryInfoList(filters, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    //以时间排序
    QFileInfoList fileinfolist = dir.entryInfoList(filters, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Time);
    QFileInfoList::iterator iterator = fileinfolist.begin();
    while (iterator != fileinfolist.end())
    {
        bIsDir = iterator->isDir();
        if (bIsDir)
        {

        }else
        {
            _fileList << iterator->filePath();
        }

        iterator++;
    }

    for(int i=0;i<_fileList.size();i++){
        qDebug()<<_fileList.at(i)<<"**************************";
    }
}

void MainWindow::on_pushButton_errorlog_clicked()
{
    ///初始化崩溃文件列表
    initFileList();
    if(_fileList.isEmpty()){
        return;
    }
    QString strCmd = QString("minidump_stackwalk %1  ./symbols > error.log").arg(_fileList.at(0));
    _process.write(strCmd.toLocal8Bit() + '\n');
}

void MainWindow::on_pushButton_move_file_clicked()
{

    QString filename = QString("%1.sym").arg(ui->comboBox_bashcmd->currentText().simplified());
    qDebug()<<filename<<"JJJJJJJJJJJJJJJJJJJJJJJJJJJJJ";

    QFile file1;
    file1.setFileName(QApplication::applicationDirPath()+"/"+filename);
    QString symDirName;
    if(file1.open(QIODevice::ReadOnly)){
        while(!file1.atEnd()){
            QList<QByteArray> list = file1.readLine().split(' ');
            symDirName = list.at(list.size()-2);
            qDebug()<<symDirName<<list<<"JJJJJJJJJJJJJJJJJJJJJJJJJJJJJ";
            break;
        }
    }

    file1.close();

    QDir dirPath;
    QString filePath = QString(QApplication::applicationDirPath()+"/symbols/%1/%2")
            .arg(ui->comboBox_bashcmd->currentText().simplified())
            .arg(symDirName);

    dirPath.setPath(filePath);
    if(!dirPath.exists()){
        if(!dirPath.mkpath(filePath)){
            qDebug()<<"*********创建文件夹失败:"<<filePath;
            return;
        }
    }
    QFile file2;
    file2.setFileName(QApplication::applicationDirPath()+"/"+filename);
    qDebug()<<QApplication::applicationDirPath()+"/"+filename<<
              "*********创建文件夹:"<<filePath+QString("/%1").arg(filename);
    QFile::rename(QApplication::applicationDirPath()+"/"+filename,
                  filePath+QString("/%1").arg(filename));
}

void MainWindow::on_pushButton_read_log_clicked()
{
    QString strCmd = QString("cat error.log");
    _process.write(strCmd.toLocal8Bit() + '\n');
}

void MainWindow::on_pushButton_clear_clicked()
{
   ui->textEdit_bashmsg->clear();
}
