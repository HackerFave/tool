#include "SqliteReader.h"
#include <QDebug>
#include "HDPolygonManager.h"
#include "HDRoadManager.h"
#include <QJsonArray>
#include "LaneInfoManager.h"
#include <QByteArray>
#include <QJsonDocument>
#include "osg/Array"

SqliteReader::SqliteReader(QString& filename)
{
	m_filename = filename;
}

SqliteReader::~SqliteReader()
{
}

bool SqliteReader::open()
{
	m_Database = QSqlDatabase::addDatabase("QSQLITE");

	m_Database.setDatabaseName(m_filename);

	if (!m_Database.open())
	{
		qDebug() << "Error: Failed to connect database." << m_Database.lastError();
		return false;
	}
	return true;
}

bool SqliteReader::read()
{
	m_sqlQuery = QSqlQuery(m_Database);

	loadHDLaneInfo();
	loadHDPolygonInfo();
	loadHDRoadInfo();
	return true;
}

bool SqliteReader::close()
{
	m_Database.close();
	return true;
}


bool SqliteReader::loadHDLaneInfo()
{
	QString select_sql = "SELECT laneid, container_elementtype, \
		 container_elementid, pre_lanenodeid, suc_lanenodeid, trajectory, lanename, loadtype, \
		 joinedtype, joinedlaneid, joinedpointlat, joinedpointlon, length, noparking, parkcapacity,\
		 speedlimit, attributes  FROM HDMAP_LANES";
	if (!m_sqlQuery.exec(select_sql))
	{
		qDebug() << "Sqlite_Select_netarcs  Error:" << m_sqlQuery.lastError();

	}
	else
	{
		while (m_sqlQuery.next())
		{
			LaneInfo* info = new LaneInfo();
			info->setLaneId(m_sqlQuery.value(0).toInt());
			info->setElementType(m_sqlQuery.value(1).toInt());
			info->setElementId(m_sqlQuery.value(2).toInt());
			info->setPreNodeId(m_sqlQuery.value(3).toInt());
			info->setSucNodeId(m_sqlQuery.value(4).toInt());

			QString trailInfo = m_sqlQuery.value(5).toString();
			//info->readLaneListFromJson(jsonByteArray);
			info->readLaneTrainInfo(trailInfo);
			info->SetLaneName(m_sqlQuery.value(6).toString());
			info->SetLoadType(m_sqlQuery.value(7).toInt());
			info->SetJoinedType(m_sqlQuery.value(8).toInt());
			info->SetJoinedLaneid(m_sqlQuery.value(9).toInt());
			info->SetJoinedPointLat(m_sqlQuery.value(10).toDouble());
			info->SetJoinedPointLon(m_sqlQuery.value(11).toDouble());
			info->SetLength(m_sqlQuery.value(12).toInt());
			info->SetNoParking(m_sqlQuery.value(13).toBool());
			info->SetParkCapacity(m_sqlQuery.value(14).toInt());
			info->SetSpeedLimit(m_sqlQuery.value(15).toInt());
			info->SetAttributes(m_sqlQuery.value(16).toString());
			LaneInfoManager::instance()->addLaneInfo(info);
		}



	}
	return true;
}

bool SqliteReader::loadHDPolygonInfo()
{
	QString select_sql = "SELECT elementid, elementtype,boundary, elementname   FROM  HDMAP_ROADELEMENTS where elementtype <> 0 ";
	if (!m_sqlQuery.exec(select_sql))
	{
		qDebug() << "Sqlite_Select_netarcs  Error:" << m_sqlQuery.lastError();

	}
	else
	{
		QStringList list;
		QStringList valueList;
		osg::Vec3d pos;
		while (m_sqlQuery.next())
		{
			int id = m_sqlQuery.value(0).toInt();
			int type = m_sqlQuery.value(1).toInt();
			QString data = m_sqlQuery.value(2).toString();
			list = data.split("((");
			data = list[1];
			list = data.split("))");
			data = list[0];
			list = data.split(";");
			osg::Vec3dArray* array = new osg::Vec3dArray;
			for (int i = 0; i < list.size(); i++)
			{
				valueList = list[i].split(",");
				pos.x() = valueList[0].toDouble();
				pos.y() = valueList[1].toDouble();
				pos.z() = -50;
				array->push_back(pos);
			}
			//if (m_bLoadType)
			//{
			//	QString name = m_sqlQuery.value(4).toString();
			//	HDPolygonManager::instance()->addPolygonArray(id, type, array, name);
			//}
			//else
			//{
			QString name = m_sqlQuery.value(3).toString();
			HDPolygonManager::instance()->addPolygonArray(id, type, array, name);
			//}

		}

	}
	return true;
}

bool SqliteReader::loadHDRoadInfo()
{
	QString select_sql = "SELECT elementid, elementtype,boundary, elementname, attributes  FROM  HDMAP_ROADELEMENTS where elementtype == 0 ";
	if (!m_sqlQuery.exec(select_sql))
	{
		qDebug() << "Sqlite_Select_netarcs  Error:" << m_sqlQuery.lastError();

	}
	else
	{
		QStringList list;
		QString leftData, rightData;
		QStringList valueList;
		osg::Vec3d pos;
		QStringList leftList, rightList;
		while (m_sqlQuery.next())
		{
			int id = m_sqlQuery.value(0).toInt();
			//int type = m_sqlQuery.value(1).toInt();
			QString data = m_sqlQuery.value(2).toString();
			list = data.split("((");
			data = list[1];
			list = data.split("))");
			data = list[0];
			list = data.split("),(");
			leftData = list[0];
			rightData = list[1];

			leftList = leftData.split(";");
			//leftList.pop_back();
			rightList = rightData.split(";");
			//rightList.pop_back();
			if (leftList.size() < 10 || rightList.size() < 10)
			{
				continue;
			}
			osg::Vec3dArray* leftArray = new osg::Vec3dArray;
			osg::Vec3dArray* rightArray = new osg::Vec3dArray;

			for (int i = 0; i < leftList.size(); i++)
			{
				valueList = leftList[i].split(",");
				pos.x() = valueList[0].toDouble();
				pos.y() = valueList[1].toDouble();
				pos.z() = -50;
				leftArray->push_back(pos);
			}
			for (int i = 0; i < rightList.size(); i++)
			{
				valueList = rightList[i].split(",");
				pos.x() = valueList[0].toDouble();
				pos.y() = valueList[1].toDouble();
				pos.z() = -50;
				rightArray->push_back(pos);
			}

			QString name = m_sqlQuery.value(3).toString();
			QString attr = m_sqlQuery.value(4).toString();
			HDRoadManager::instance()->addOneRoad(id, leftArray, rightArray, name, attr);
		}

	}
	return true;
}
