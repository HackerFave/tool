#include "sqlOperation.h"

sqlOperation::sqlOperation(QObject *parent)
    : QObject(parent)
{

}

sqlOperation::~sqlOperation()
{
    m_db.close();
    if (m_query != NULL)
    {
        delete m_query;
        m_query = NULL;
    }
}


bool sqlOperation::DatabaseIsOK()
{
    return true;
}

bool sqlOperation::Init(const QString & host, const QString & user, const QString & passwd,
                        const QString & dataname,const QString & type)
{
    QString sql = QString("CREATE DATABASE %1;").arg(dataname);
    QSqlQuery *ret;
    Query(sql, &ret);
    if (m_db.isValid())//存在连接直接退出
    {
        return false;
    }

    m_dbhost = host;
    m_dbuser = user;
    m_dbpasswd = passwd;
    m_dbdataname = dataname;
    //m_dbdataname = QCoreApplication::applicationDirPath() + '/' + dataname;

    m_db = QSqlDatabase::addDatabase("QMYSQL", type);
    m_db.setHostName(m_dbhost);							//数据库服务器IP
    m_db.setUserName(m_dbuser);							//数据库用户名
    m_db.setPassword(m_dbpasswd);						//数据库密码

    //QString str = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)}; FIL={MS Access};DBQ=%1;").arg(m_dbdataname);
    //m_db.setDatabaseName(str);
    m_db.setConnectOptions("MYSQL_OPT_RECONNECT = 1");  //add 设置mysql数据断开后自动重连 [WangPing 2020/3/20]
    m_db.setDatabaseName(m_dbdataname);
    if (!m_db.open()) {
        m_db.setConnectOptions();	//add 链接失败时清除设置 [WangPing 2020/3/20]
        QSqlError err = m_db.lastError();
        qDebug() << err.databaseText();
        return false;
    }
    //	qDebug() << QSqlDatabase::drivers();
    m_query = new QSqlQuery(m_db);
    return true;


}

bool sqlOperation::Query(const QString & sql, QSqlQuery ** ret)
{
    if (m_query == NULL)
        return false;
    bool isok = m_query->exec(sql);
    m_sql = sql;
    QString a = m_query->value(1).toString();
    *ret = m_query;
    if (isok == false) {
        return false;
    }
    return true;
}

bool sqlOperation::Add(QString table_name, std::vector<std::map<QString, QString>> data, QSqlQuery ** ret)
{
    if (m_query == NULL)
        return false;
    QString sql = "insert into " + table_name + "(";
    QString values = " values";
    QString valueData;
    for (std::vector<std::map<QString, QString>>::const_iterator iter = data.cbegin(); iter != data.cend(); iter++)
    {
        for (std::map<QString, QString>::const_iterator i = (*iter).cbegin(); i != (*iter).cend(); i++)
        {
            sql += i->first + ", ";
        }
        break;
    }
    for (std::vector<std::map<QString, QString>>::const_iterator iter = data.cbegin(); iter != data.cend(); iter++)
    {
        // cout << (*iter) << endl;
        QString value;
        value += "(";
        for (std::map<QString, QString>::const_iterator i = (*iter).cbegin(); i != (*iter).cend(); i++)
        {
            // sql += i->first + ", ";
            value += "'" + i->second + "', ";

        }
        value.chop(2);
        value += "),";
        valueData += value;
    }

    sql.chop(2);
    //values.chop(2);
    sql += ")";
    values += valueData;
    values.chop(1);
    sql += values;
    m_query->prepare(sql);
    bool is = Query(sql, ret);

    return is;
}
bool sqlOperation::Add(QString table_name, std::map<QString, QString> data, QSqlQuery ** ret)
{
    if (m_query == NULL)
        return false;
    QString sql = "insert into " + table_name + "(";
    QString values = " values(";
    for (std::map<QString, QString>::const_iterator i = data.cbegin(); i != data.cend(); i++)
    {
        sql += i->first + ", ";
        values += "'" + i->second + "', ";

    }
    sql.chop(2);
    values.chop(2);
    sql += ")";
    values += ")";
    sql += values;
    m_query->prepare(sql);
    bool is = Query(sql, ret);

    return is;
}

bool sqlOperation::Del(QString table_name, std::map<QString, QString> where, QSqlQuery ** ret)
{
    if (m_query == NULL)
        return false;
    QString sql = "delete from ";
    sql += table_name;
    sql += " where ";
    for (std::map<QString, QString>::const_iterator i = where.cbegin(); i != where.cend(); i++)
    {
        sql += i->first + "=";
        sql += "'" + i->second + "'and ";
    }
    sql.chop(4);
    m_query->prepare(sql);
    return Query(sql, ret);
}
//删除一条记录
bool sqlOperation::delRecord(QString table_name, const QString &key, const QString &value, QSqlQuery ** ret)
{
    QString sql = QString("delete from %1 where %2 = '%3'")
            .arg(table_name)
            .arg(key)
            .arg(value);
    m_query->prepare(sql);
    return Query(sql, ret);
}

bool sqlOperation::Updata(QString table_name, std::map<QString, QString> where, std::map<QString, QString> data, QSqlQuery ** ret)
{
    if (m_query == NULL)
        return false;
    QString sql = "update " + table_name + " set ";
    for (std::map<QString, QString>::const_iterator i = data.cbegin(); i != data.cend(); i++)
    {
        sql += i->first + "=";
        sql += "'" + i->second + "',";

    }
    sql.chop(1);
    sql += "where ";
    for (std::map<QString, QString>::const_iterator i = where.cbegin(); i != where.cend(); i++)
    {
        sql += i->first + "=";
        sql += "'" + i->second + "'and ";
    }
    sql.chop(4);
    return Query(sql, ret);
}

bool sqlOperation::Find(QString table_name, std::list<QString> key, std::list<std::list<QString>>* row, QSqlQuery ** ret)
{
    if (m_query == NULL)
        return false;
    QString sql = "SELECT ";
    int len = key.size();

    for (int i = 0; i < len; i++)
    {
        sql += key.front();
        sql += ",";
        key.pop_front();
    }
    sql.chop(1);
    sql += " FROM " + table_name;

    m_query->prepare(sql);
    if (Query(sql, ret))
    {
        while (m_query->next())
        {
            std::list<QString> j;
            for (int i = 0; i < len; i++)
            {
                j.push_back(m_query->value(i).toString());
            }
            row->push_back(j);
        }
        return true;
    }
    else
        return false;
}

void sqlOperation::CloseDB()
{
    m_db.close();
}
bool sqlOperation::Find(QString sql, std::list<std::list<QString> > *row, QSqlQuery ** ret, int len)
{
    m_query->prepare(sql);
    if (Query(sql, ret))
    {
        while (m_query->next())
        {
            std::list <QString> j;
            for (int i = 0; i < len; i++)
            {
                j.push_back(m_query->value(i).toString());

            }
            row->push_back(j);
        }
        return true;
    }
    else
        return false;
}



bool sqlOperation::Find(QString table_name, std::list<QString> key, std::map<QString, QString> where, std::list<std::list<QString> > *row, QSqlQuery ** ret)
{
    if (m_query == NULL)
        return false;
    QString sql = "SELECT ";
    int len = key.size();
    for (int i = 0; i < len; i++)
    {
        sql += key.front();
        sql += ",";
        key.pop_front();
    }
    sql.chop(1);
    sql += " FROM " + table_name;
    sql += " WHERE ";
    for (std::map<QString, QString>::const_iterator i = where.cbegin(); i != where.cend(); i++)
    {
        sql += i->first + " = '" + i->second + "'" + " AND ";
    }
    sql.chop(5);
    //qDebug()<<sql;
    m_query->prepare(sql);
    if (Query(sql, ret))
    {
        while (m_query->next())
        {
            std::list <QString> j;
            for (int i = 0; i < len; i++)
            {
                j.push_back(m_query->value(i).toString());

            }
            row->push_back(j);
        }
        return true;
    }
    else
        return false;
}
//判断该条记录是否存在
int sqlOperation::isExists(QString table_name, QString key, QString value, QSqlQuery ** ret)
{
    QString sql = QString("select * from %1 where %2 = '%3'")
            .arg(table_name)
            .arg(key)
            .arg(value);
    m_query->prepare(sql);
    if (Query(sql, ret))
    {
        if (!m_query->next()) {
            return 0;
        }
        else
        {
            return 1;
        }

    }
}
bool sqlOperation::findData(QString table_name, QMap<QString, QString> &where, const QStringList &keyList, QList<QVariantList> &row, QSqlQuery ** ret)
{
    // QMap<QString, QString> map = where;
    //if (where.count("record_ID") && where.value("record_ID").simplified().isEmpty()) {
    //  where["record_ID"] = _recordID;
    //}
    QString sql = QString("select * from %1 where ")
            .arg(table_name);
    int keySize = where.size();
    int num = 0;
    int keyNum = keyList.size();
    QMap<QString, QString>::iterator it;
    for (it = where.begin(); it != where.end(); it++) {
        num++;
        sql += it.key();
        sql += " = ";
        sql += "'" + it.value() + "'";
        if (num > keySize - 1) {
            break;
        }
        sql += " and ";
    }
    m_query->prepare(sql);
    QSqlRecord rec = m_query->record();
    //int keyNum = keyList.size();
    if (Query(sql, ret))
    {
        while (m_query->next())
        {
            QVariantList listChild;
            rec = m_query->record();

            /*QMap<QString, QString>::iterator itKey;
            for (itKey = where.begin(); itKey != where.end(); itKey++) {
                QString value = m_query->value(rec.indexOf(itKey.key().simplified())).toString();
                listChild.append(value);
            }*/
            for (int i = 0; i < keyNum; i++) {
                QString value = m_query->value(rec.indexOf(keyList.at(i).simplified())).toString();
                listChild.append(value);
            }
            row.append(listChild);
        }
        return true;
    }
    else
        return false;

}
bool sqlOperation::findData(QString table_name, QMap<QString, QString> &where, const QStringList &keyList, QList<QMap<QString, QString>>&row, QSqlQuery ** ret)
{
    // QMap<QString, QString> map = where;
    //if (where.count("record_ID") && where.value("record_ID").simplified().isEmpty()) {
    //  where["record_ID"] = _recordID;
    //}
    QString sql = QString("select * from %1 where ")
            .arg(table_name);
    int keySize = where.size();
    int num = 0;
    int keyNum = keyList.size();
    QMap<QString, QString>::iterator it;
    for (it = where.begin(); it != where.end(); it++) {
        num++;
        sql += it.key();
        sql += " = ";
        sql += "'" + it.value() + "'";
        if (num > keySize - 1) {
            break;
        }
        sql += " and ";
    }
    m_query->prepare(sql);
    QSqlRecord rec = m_query->record();
    //int keyNum = keyList.size();
    if (Query(sql, ret))
    {
        while (m_query->next())
        {
            QMap<QString, QString> mapChild;
            rec = m_query->record();

            /*QMap<QString, QString>::iterator itKey;
            for (itKey = where.begin(); itKey != where.end(); itKey++) {
                QString value = m_query->value(rec.indexOf(itKey.key().simplified())).toString();
                listChild.append(value);
            }*/
            for (int i = 0; i < keyNum; i++) {
                QString value = m_query->value(rec.indexOf(keyList.at(i).simplified())).toString();
                mapChild.insert(keyList.at(i), value);
            }
            row.append(mapChild);
        }
        return true;
    }
    else
        return false;
}
bool sqlOperation::findDataRecord(QString table_name, QMap<QString, QString> &where, const QStringList &keyList, std::vector<std::map<QString, QString>> &row, QSqlQuery ** ret)
{
    QString sql = QString("select * from %1 where ")
            .arg(table_name);
    int keySize = where.size();
    int num = 0;
    int keyNum = keyList.size();
    QMap<QString, QString>::iterator it;
    for (it = where.begin(); it != where.end(); it++) {
        num++;
        sql += it.key();
        sql += " = ";
        sql += "'" + it.value() + "'";
        if (num > keySize - 1) {
            break;
        }
        sql += " and ";
    }
    m_query->prepare(sql);
    QSqlRecord rec = m_query->record();
    //int keyNum = keyList.size();
    if (Query(sql, ret))
    {
        while (m_query->next())
        {
            std::map<QString, QString> mapChild;
            rec = m_query->record();

            /*QMap<QString, QString>::iterator itKey;
            for (itKey = where.begin(); itKey != where.end(); itKey++) {
                QString value = m_query->value(rec.indexOf(itKey.key().simplified())).toString();
                listChild.append(value);
            }*/
            for (int i = 0; i < keyNum; i++) {
                QString value = m_query->value(rec.indexOf(keyList.at(i).simplified())).toString();
                // mapChild.insert(keyList.at(i), value);
                mapChild[keyList.at(i)] = value;
            }
            row.push_back(mapChild);
        }
        return true;
    }
    else
        return false;
}
//查询最近days天数据
bool sqlOperation::findData_Date(QString table_name, const QStringList &keyList, QList<QVariantList> &row, int days, const QString &data_time_key, QSqlQuery ** ret)
{
    QString sql = QString("select * from %1 where DATE_SUB(CURDATE(), INTERVAL %2 DAY) <= date(%3)")
            .arg(table_name)
            .arg(days)
            .arg(data_time_key);
    m_query->prepare(sql);
    QSqlRecord rec = m_query->record();
    int keySize = keyList.size();
    if (Query(sql, ret))
    {
        while (m_query->next())
        {
            /*
            std::list <QString> j;
            for (int i = 0; i < 3; i++)
            {
                j.push_back(m_query->value(i).toString());

            }
            row->push_back(j);
            */
            QVariantList listChild;
            rec = m_query->record();
            for (int i = 0; i < keySize; i++) {
                QString value = m_query->value(rec.indexOf(keyList.at(i).simplified())).toString();
                listChild.append(value);
            }
            /* int snocol = rec.indexOf("id");
             int snamecol = rec.indexOf("data_time");
             int sclasscol = rec.indexOf("num");
             QString value1 = m_query->value(snocol).toString();
             QString value2 = m_query->value(snamecol).toString();
             QString value3 = m_query->value(sclasscol).toString();
             qDebug() << "sno:" << value1 << "\t" << "sname:" << value2 << "\t" << "sclass:" << value3;
             listChild<<value1<<value2<<value3;*/
            row.append(listChild);
        }
        return true;
    }
    else
        return false;
}
void sqlOperation::CreateTable(QString base)
{
    QString dataBase = QString("CREATE DATABASE %1;").arg(base);
    QString sql = "CREATE TABLE t1(id VARCHAR(100) not null primary key,name VARCHAR(100)); ";
}

QSqlDatabase sqlOperation::sql_create(const QString &connectName,const QString &DBname,
                                      const QString &userName,const QString &passWord)
{
    QSqlDatabase database;
    if (QSqlDatabase::contains(connectName))
    {
        database = QSqlDatabase::database(connectName);
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(QString("%1.db").arg(DBname));
        database.setUserName(userName);
        database.setPassword(passWord);
    }

    return database;
}

void sqlOperation::sql_close(QSqlDatabase DatabaseName)
{
    DatabaseName.close();
}
//创建表
QSqlQuery sqlOperation::sql_create_table(QString tableName,QMap<QString,QString> id)
{
    QSqlQuery sql_table;
    QString table_str =  "CREATE TABLE IF NOT EXISTS ";//test (id int primary key, date_time_record varchar(50),temp_record float)";

    table_str = table_str+ tableName+"(id int primary key,";
    QMap<QString,QString> ::iterator it;
    for(it = id.begin();it!=id.end();it++){
        table_str = table_str+it.key()+" "+it.value()+",";
    }
    table_str.chop(1);
    table_str = table_str +")";
    qDebug()<<table_str<<"::::::::::::::::::::::::::::::::::::::::::::";
    if(!sql_table.exec(table_str))
    {
        qDebug() << "Error: Fail to create table." << sql_table.lastError();
    }
    else
    {
        qDebug() << "Table created!";
    }
    return sql_table;
}
//插入数据
int sqlOperation::sql_insert_data(QSqlQuery sql_table, QString data_string)
{
    if(!sql_table.exec(data_string))
    {
        qDebug() << sql_table.lastError();
        return -1;
    }
    else
    {
        qDebug() << "inserted successfully!";
        return 0;
    }
}

int sqlOperation::sql_delete_data(QSqlQuery sql_table, QString delete_string, int id)
{
    QString delete_string_set = delete_string + QString::number(id);
    qDebug() << delete_string_set;
    if(!sql_table.exec(delete_string_set))
    {
        qDebug()<<sql_table.lastError();
        return -1;
    }
    else
    {
        qDebug()<<"deleted!";
        return 0;
    }
}

int sqlOperation::sql_clear_table(QSqlQuery sql_table, QString clear_string)
{
    if(!sql_table.exec(clear_string))
    {
        qDebug() << sql_table.lastError();
        return -1;
    }
    else
    {
        qDebug() << "table cleared";
        return 0;
    }
}

int sqlOperation::sql_select_maxid(QSqlQuery sql_table, QString tableName)
{
    int max_id = 0;
    QString str = QString("select max(id) from %1").arg(tableName);
    if(!sql_table.exec(str))
    {
        qDebug() << sql_table.lastError();
        return -1;
    }
    else
    {
        while(sql_table.next())
        {
            max_id = sql_table.value(0).toInt();
        }
        return max_id;
    }
}


QString sqlOperation::temp_read()
{
    QString temp;
    float temp_float;
    QByteArray temp_byte;
    QFile file("/sys/devices/virtual/thermal/thermal_zone0/temp");
    if(file.open(QIODevice::ReadOnly))
    {
        temp_byte = file.readLine();
        temp = temp_byte;
        temp_float = temp.toFloat();
        temp = QString("%1").arg(temp_float/1000);
        //qDebug() << temp;
    }
    file.close();
    return temp;
}
