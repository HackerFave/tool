#include "LaneInfo.h"
#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonObject"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "EngineProxy.h"
#include "GisModel.h"

LaneInfo::LaneInfo()
{
	m_laneName = QString("");
}


LaneInfo::~LaneInfo()
{
}

void LaneInfo::cpLaneInfo(LaneInfo& info)
{
	m_idLane = info.m_idLane;
	m_typeElement = info.m_typeElement;
	m_idElement = info.m_idElement;
	m_idPreNode = info.m_idPreNode;
	m_idSucNode = info.m_idSucNode;

	m_laneName = "";

	m_nLoadType = info.m_nLoadType;
	m_nJoinedType = info.m_nJoinedType;
	m_nJoinedLaneid = info.m_nJoinedLaneid;
	m_dJoinedPointLat = info.m_dJoinedPointLat;
	m_dJoinedPointLon = info.m_dJoinedPointLon;
	m_nLength = info.m_nLength;
	m_bNoParking = info.m_bNoParking;
	m_nParkCapacity = info.m_nParkCapacity;
	m_nSpeedLimit = info.m_nSpeedLimit;
	m_strAttributes = info.m_strAttributes;
}

void LaneInfo::readLaneListFromJson(QByteArray& info)
{
	//QJsonDocument jsonDocument = QJsonDocument::fromJson(info);
	//if (jsonDocument.isNull()) {
	//	qDebug() << "===> QJsonDocument：" << info;
	//	return;
	//}
	//QJsonObject jsonObject = jsonDocument.object();
	//QJsonArray jsonArray = jsonObject["infos"].toArray();
	//QJsonObject obj;
	//for (int i = 0; i < jsonArray.size(); i++)
	//{
	//	QJsonObject obj = jsonArray[i].toObject();
	//	TrialInfo info;
	//	info.readFromJson(i, obj);
	//	//m_LaneList.push_back(info);
	//}
}

void LaneInfo::readLaneTrainInfo(QString& info)
{
	QString data = info;
	QStringList list;
	list = data.split("((");
	data = list[1];
	list = data.split("))");
	data = list[0];
	list = data.split(";");
	//list.pop_back();
	for (int i = 0; i < list.size(); i++)
	{
		LaneTrailInfo trailInfo;
		trailInfo.ReadInfos(list[i]);
		trailInfo.setId(i + 1);
		m_LaneTrailList.push_back(trailInfo);
	}
}


void LaneInfo::readLaneTrainInfoFromRawData(QString& filename)
{
	m_LaneTrailList.clear();
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&file);
		QString readLine;
		QStringList list;
		LaneTrailInfo info;
		unsigned int pointId = 1;
		while (!in.atEnd())
		{
			readLine = in.readLine();
			list = readLine.split(",");
			if (list.size() == 11 || list.size() == 13)
			{
				info.setInfos(pointId, list);
				pointId++;
			}
			m_LaneTrailList.append(info);
		}
		file.close();
	}

}

QString LaneInfo::generateLaneTrainInfo()
{
	if (m_LaneTrailList.size() == 0)
	{
		return QString("");
	}
	QString info;
	info.append("TRAJECTORY((");
	for (int i = 0; i < m_LaneTrailList.size(); i++)
	{
		info.append(m_LaneTrailList[i].generateWriteInfo());
	}
	info.remove(info.size() - 1, 1);
	info.append("))");
	return info;
}

QString LaneInfo::generateLaneTmpInfo()
{
	if (m_LaneTrailList.size() == 0)
	{
		return QString("");
	}
	QString info;
	info.append("TRAJECTORY((");
	for (int i = 0; i < m_LaneTrailList.size(); i++)
	{
		info.append(m_LaneTrailList[i].generateTmpInfo());
	}
	info.remove(info.size() - 1, 1);
	info.append("))");
	return info;
}

QList<LaneTrailInfo>& LaneInfo::getLaneList()
{
	return m_LaneTrailList;
}


void LaneInfo::setLaneTrailSpeed(int id, double value)
{
	if (m_LaneTrailList.at(id - 1).pointID == id)
	{
		m_LaneTrailList[id - 1].Speed = value;
	}

	else
	{
		for (int i = 0; i < m_LaneTrailList.size(); i++)
		{
			if (m_LaneTrailList.at(i).pointID == id)
			{
				m_LaneTrailList[i].Speed = value;
			}
		}
	}
}

osg::Vec3d LaneInfo::getLaneTrailLatLonHeight(int id)
{
	if (m_LaneTrailList.at(id - 1).pointID == id)
	{
		return m_LaneTrailList[id - 1].getLonLatHeight();
	}

	else
	{
		for (int i = 0; i < m_LaneTrailList.size(); i++)
		{
			if (m_LaneTrailList.at(i).pointID == id)
			{
				return m_LaneTrailList[i].getLonLatHeight();
			}
		}
	}
	return osg::Vec3d();
}

void LaneInfo::setLaneList(QList<LaneTrailInfo>& list)
{
	m_LaneTrailList.clear();
	for (int i = 0; i < list.size(); i++)
	{
		m_LaneTrailList.push_back(list.at(i));
	}
}

void LaneInfo::updateTrailInfoHeight(QVector<double> mCurHeight)
{
	for (int i = 0; i < m_LaneTrailList.size(); i++)
	{
		m_LaneTrailList[i].Alt = mCurHeight[i];
	}
}

int LaneInfo::GetGear()
{
	QJsonObject message;
	int Gear = 1;
	QString attributes = m_strAttributes;
	if (attributes.isEmpty())
	{
		message.insert("Gear", 1);
	}
	else
	{
		QJsonParseError l_err;
		QJsonDocument l_doc = QJsonDocument::fromJson(attributes.toUtf8(), &l_err);
		if (l_err.error == QJsonParseError::NoError)
		{
			if (l_doc.isObject())
			{
				message = l_doc.object();
			}
		}
		if (message.keys().contains("Gear"))
		{
			Gear = message["Gear"].toInt();
		}
	}
	return Gear;
}

void LaneInfo::SetGear(int val)
{
	QJsonObject message;
	QString attributes = m_strAttributes;
	if (attributes.isEmpty())
	{
		message.insert("Gear", val);
	}
	else
	{
		QJsonParseError l_err;
		QJsonDocument l_doc = QJsonDocument::fromJson(attributes.toUtf8(), &l_err);
		if (l_err.error == QJsonParseError::NoError)
		{
			if (l_doc.isObject())
			{
				message = l_doc.object();
			}
		}
		if (message.keys().contains("Gear"))
		{
			message["Gear"] = val;
		}
		else
		{
			message.insert("Gear", val);
		}
	}

	m_strAttributes = QString(QJsonDocument(message).toJson(QJsonDocument::Compact));
}

int LaneInfo::GetWorkAndTest()
{
	QJsonObject message;
	int WorkAndTest = 1;
	QString attributes = m_strAttributes;
	if (attributes.isEmpty())
	{
		message.insert("WorkAndTest", 1);
	}
	else
	{
		QJsonParseError l_err;
		QJsonDocument l_doc = QJsonDocument::fromJson(attributes.toUtf8(), &l_err);
		if (l_err.error == QJsonParseError::NoError)
		{
			if (l_doc.isObject())
			{
				message = l_doc.object();
			}
		}
		if (message.keys().contains("WorkAndTest"))
		{
			WorkAndTest = message["WorkAndTest"].toInt();
		}
	}
	return WorkAndTest;
}

void LaneInfo::SetWorkAndTest(int val)
{
	QJsonObject message;
	QString attributes = m_strAttributes;
	if (attributes.isEmpty())
	{
		message.insert("WorkAndTest", val);
	}
	else
	{
		QJsonParseError l_err;
		QJsonDocument l_doc = QJsonDocument::fromJson(attributes.toUtf8(), &l_err);
		if (l_err.error == QJsonParseError::NoError)
		{
			if (l_doc.isObject())
			{
				message = l_doc.object();
			}
		}
		if (message.keys().contains("WorkAndTest"))
		{
			message["WorkAndTest"] = val;
		}
		else
		{
			message.insert("WorkAndTest", val);
		}
	}

	m_strAttributes = QString(QJsonDocument(message).toJson(QJsonDocument::Compact));
}

void LaneInfo::clearJoined()
{
	m_nJoinedLaneid = 0;
	m_dJoinedPointLat = 0.0f;
	m_dJoinedPointLon = 0.0f;
}

void LaneTrailInfo::ReadInfos(QString& data)
{
	QStringList list = data.split(",");
	Heading = list[3].toDouble();
	Lat = list[1].toDouble();
	Lon = list[0].toDouble();
	Alt = list[2].toDouble();
	Speed = list[4].toDouble();
	LDis = list[5].toDouble();
	RDis = list[6].toDouble();
	if (list.size() == 8)
	{
		CollectAlt = list[7].toDouble();
	}
	else
		CollectAlt = Alt;
}

void LaneTrailInfo::setId(unsigned int id)
{
	pointID = id;
}

void LaneTrailInfo::cpInfo(LaneTrailInfo& copyInfo)
{
	pointID = -1;
	Heading = copyInfo.Heading;
	Lat = copyInfo.Lat;
	Lon = copyInfo.Lon;
	Alt = copyInfo.Alt;
	Speed = copyInfo.Speed;
	LDis = copyInfo.LDis;
	RDis = copyInfo.RDis;
	CollectAlt = copyInfo.CollectAlt;
}

void LaneTrailInfo::setLonLatHeight(osg::Vec3d& vec)
{
	Lon = vec.x();
	Lat = vec.y();
	//暂时不更新alt;
	//Alt = vec.z();
}

void LaneTrailInfo::updateLonLat(double lon, double lat)
{
	Lon = lon;
	Lat = lat;
}

osg::Vec3d LaneTrailInfo::getLonLatHeight()
{
	return osg::Vec3d(Lon, Lat, Alt);
}

void LaneTrailInfo::setHeading(double heading)
{
	Heading = heading;
}

void LaneTrailInfo::setSpeed(double speed)
{
	Speed = speed;
}

QString LaneTrailInfo::generateWriteInfo()
{
	QString info;

	info.append(QString::number(Lon, 'g', 15));
	info.append(",");
	info.append(QString::number(Lat, 'g', 14));
	info.append(",");
	info.append(QString::number(Alt));
	info.append(",");
	info.append(QString::number(Heading));
	info.append(",");
	info.append(QString::number(Speed));
	info.append(",");
	info.append(QString::number(LDis));
	info.append(",");
	info.append(QString::number(RDis));
	info.append(",");
	info.append(QString::number(CollectAlt));
	info.append(";");
	return info;
}

QString LaneTrailInfo::generateTmpInfo()
{
	double x = 0;
	double y = 0;
	EngineProxy::instance()->getGlobe()->computePointFromPosition(Lon, Lat, x, y);
	QString info;
	info.append(QString::number(x, 'g', 20));
	info.append(",");
	info.append(QString::number(y, 'g', 20));
	info.append(",");
	info.append(QString::number(Alt, 'g', 15));
	info.append(";");
	return info;
}

void LaneTrailInfo::setInfos(unsigned int& pID, QStringList& list)
{
	pointID = pID;
	Heading = list[1].toDouble();
	Lat = list[2].toDouble();
	Lon = list[3].toDouble();
	Alt = list[4].toDouble();
	Speed = list[5].toDouble();
	LDis = 0.0;
	RDis = 0.0;
	CollectAlt = Alt;
}

QString LaneTrailInfo::getLpxString()
{
	QString info;
	info.append("$,");
	info.append(QString::number(Heading, 'g', 8));//1
	info.append(",");
	info.append(QString::number(Lat, 'g', 12));//2
	info.append(",");
	info.append(QString::number(Lon, 'g', 12));//3
	info.append(",");
	info.append(QString::number(Alt, 'g', 8));//4
	info.append(",");
	info.append(QString::number(Speed, 'g', 7));//5
	info.append(",");
	info.append(QString::number(0, 'g', 4));//6
	info.append(",");
	info.append(QString::number(0, 'g', 4));//7
	info.append(",");
	info.append(QString::number(0, 'g', 4));//8
	info.append(",");
	info.append("");//5
	info.append(",");
	info.append("");//5
	info.append(",");
	info.append("");//5
	info.append(",");

	info.append("*52");

	return info;
}

