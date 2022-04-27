#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readBashStandardOutputInfo();
    void readBashStandardErrorInfo();
    void on_pushButton_bashrun_clicked();
    void on_pushButton_errorlog_clicked();

    void on_pushButton_move_file_clicked();

    void on_pushButton_read_log_clicked();

    void on_pushButton_clear_clicked();

private:
    void init();
    void initFileList();
    void initCombox();

private:
    Ui::MainWindow *ui;
    QProcess _process;
    QStringList _fileList;
};
#endif // MAINWINDOW_H
