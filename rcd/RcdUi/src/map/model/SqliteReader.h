#ifndef SQLITEREADER_H
#define SQLITEREADER_H


#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class SqliteReader
{
public:
	SqliteReader(QString& filename);
	virtual ~SqliteReader();
	virtual bool open();
	virtual bool read();
	virtual bool close();

protected:

	bool loadHDLaneInfo();
	bool loadHDPolygonInfo();
	bool loadHDRoadInfo();


protected:
	QSqlDatabase m_Database;
	QSqlQuery m_sqlQuery;
	QString m_filename;
};



#endif // SQLITEREADER_H