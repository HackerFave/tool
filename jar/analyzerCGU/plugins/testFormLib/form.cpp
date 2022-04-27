#include "form.h"
#include "ui_form.h"
#include <QDebug>
#include <QHeaderView>
Form::Form(QWidget *parent,const void *node) :
    BaseWidget(parent),
    _node(node),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    qRegisterMetaType<INFODATA>("INFODATA&");
    initUi();
    qDebug()<<"[ 插件：]"<<"CGU_Widget";
    /***********数据回馈轮寻机制  **********/
    _Pthread = new workThread;
    _q_thread = new QThread;
    _Pthread->moveToThread(_q_thread);
    connect(_q_thread,SIGNAL(finished()),_Pthread,SLOT(deleteLater()));
    connect(_q_thread,SIGNAL(started()),_Pthread,SLOT(slotPostWork()));


    _Pthread_1 = new testThread;
    _q_thread_1 = new QThread;
    _Pthread_1->moveToThread(_q_thread_1);
    connect(_q_thread_1,SIGNAL(finished()),_Pthread_1,SLOT(deleteLater()));
    connect(_q_thread_1,SIGNAL(started()),_Pthread_1,SLOT(slotPostWork()));

    connect(_Pthread_1,SIGNAL(signalDataInfo(std::map<std::string,std::list<std::string>>,std::list<std::string>)),
            this,SLOT(slotDataInfo(std::map<std::string,std::list<std::string>>,std::list<std::string>)),Qt::DirectConnection);

    connect(_Pthread_1,SIGNAL(signalDataInfo(std::map<std::string,std::list<std::string>>,std::list<std::string>)),
            this,SLOT(slotFillTableView(std::map<std::string,std::list<std::string>>,std::list<std::string>)),Qt::DirectConnection);

    connect(_Pthread_1,SIGNAL(signalTest(QString)),this,SLOT(slotTest(QString)));
    setAttribute(Qt::WA_DeleteOnClose);
    //    _q_thread->start();
    //    _q_thread_1->start();

    //    QBreakpadInstance.setDumpPath("crashes"); // 设置生成dump文件路径
    //    QSqlDatabase db= _sql.getSqlInstance()->sql_create("dbcon","data","tc","123456");
    ////    QSqlDatabase db1= _sql.getSqlInstance()->sql_create("dbcon1","data22","tc","123456");
    //    if(!db.open())
    //    {
    //        qDebug() << "Error: Failed to connect database." << db.lastError();
    //    }
    //    else {
    //        QMap<QString,QString> data;
    //        data["id1"] = "varchar(50)";
    //        data["id2"] = "float";
    //        data["id3"] = "bool";
    //        QSqlQuery sqlTable=  _sql.getSqlInstance()->sql_create_table("test",data);
    //        QSqlQuery sqlTable11=  _sql.getSqlInstance()->sql_create_table("test222",data);


    //    }
    //     *((int*)0) = 10;

}

Form::~Form()
{
    if(_q_thread->isRunning())
    {
        _Pthread->stopWork();  //关闭线程槽函数
        _q_thread->quit();            //退出事件循环
        _q_thread->wait();            //释放线程槽函数资源
    }
    if(_q_thread_1->isRunning())
    {
        _Pthread_1->stopWork();  //关闭线程槽函数
        _q_thread_1->quit();            //退出事件循环
        _q_thread_1->wait();            //释放线程槽函数资源
    }
    delete ui;
}

void Form::initUi()
{
    //1，构造Model，这里示例具有3层关系的model构造过程
    _treeview_model = new QStandardItemModel(ui->treeView_node_info);
    _treeview_model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("信息") << QStringLiteral("名称"));     //设置列头
    ui->treeView_node_info->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView_node_info->setModel(_treeview_model);
    connect(ui->treeView_node_info,SIGNAL(doubleClicked(const QModelIndex)),
            this,SLOT(OnlineTreeViewDoubleClick(const QModelIndex)));

    _tableview_model = new QStandardItemModel;
    ui->tableView_data->setModel(_tableview_model);
    /* 设置列宽在可视界面自适应宽度 *//*
    ui->tableView_data->horizontalHeader()->setResizeMode(QHeaderView::Stretch);*/
    //    ui->tableView_data->horizontalHeader()->setResizeContentsPrecision(QHeaderView::Interactive);
    ui->tableView_data->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    /* 行颜色交替显示 */
    ui->tableView_data->setAlternatingRowColors(true);
    /* 不允许在图形界面修改内容 */
    ui->tableView_data->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //    ui->tableView_data->setColumnWidth(0, 200);//设置固定宽度
}

void Form::slotDataInfo(std::map<std::string,std::list<std::string>> data,std::list<std::string> info)
{


    if(info.size()!= _treeview_model->rowCount()){
        _treeview_model->clear();
        for(auto it :info){
            //一级节点，加入mModel
            QList<QStandardItem*> items1;
            QStandardItem* item_column_1_1 = new QStandardItem(it.data());
            //                        QStandardItem* item_column_1_2 = new QStandardItem(QStringLiteral(""));
            items1.append(item_column_1_1);
            //                        items1.append(item_column_1_2);
            _treeview_model->appendRow(items1);


            for(auto itmap:data){
                //二级节点,加入第1个一级节点
                QList<QStandardItem*> items2;
                QStandardItem* item_column_2_1 = new QStandardItem(itmap.first.c_str());
                //                                QStandardItem* item_column_2_2 = new QStandardItem(QStringLiteral("二级节点"));
                items2.append(item_column_2_1);
                //                items2.append(item_column_2_2);
                item_column_1_1->appendRow(items2);
                QStringList list;
                for(auto it_child:itmap.second){
                    QString str = it_child.data();
                    QList<QStandardItem*> items3;
                    QStandardItem* item_column_3_1 = new QStandardItem(str.split(":").at(0));
                    items3.append(item_column_3_1);
                    item_column_2_1->appendRow(items3);
                    list.push_back(str.split(":").at(0));
                }
                _mapInfo[itmap.first.c_str()]= list;
            }
        }

    }
}
void Form::slotTest(QString data)
{
}
void Form::OnlineTreeViewDoubleClick(const QModelIndex index)
{

    if(!_mapInfo.contains(index.data().toString())){

        return;
    }
    _isClick = true;
    _treeview_model->item(index.row(),index.column());

    /* 设置列数 */
    _tableview_model->setColumnCount(_mapInfo.value(index.data().toString()).size());
    QStringList listData = _mapInfo.value(index.data().toString());
    for(int i=0;i<listData.size();i++){
        _tableview_model->setHeaderData(i, Qt::Horizontal, listData.at(i));

    }
}
void Form::slotFillTableView(std::map<std::string,std::list<std::string>> data,std::list<std::string> info)
{
    if(!_isClick){
        return;
    }
    for(auto it:data){
        QList<QStandardItem*> list;
        for(auto it_child:it.second){
            QString str = it_child.data();
            QStandardItem *item =new QStandardItem(str.split(":").at(1));

            list.append(item);
        }
        _tableview_model->appendRow(list);
    }

}
void Form::slotRecvBackendData(const std::string& data)
{
    qDebug()<<QString("[%1获取主框架数据:]").arg(this->objectName());
}
void Form::slotPluginWidgetsPointHash(QHash<QString, QWidget *> hash)
{
    QHash<QString,QWidget*>::iterator it;
    for (it = hash.begin();it!=hash.end();it++) {
        emit signalPluginToBackendData("我在测试**************");
    }
}

