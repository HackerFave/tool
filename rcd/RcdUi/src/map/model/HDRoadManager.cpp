#include "HDRoadManager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include "EngineProxy.h"
#include "GisModel.h"


HDRoadManager* HDRoadManager::_manager = 0;
HDRoadManager::HDRoadManager()
{
	m_lastID = -1;
}

HDRoadManager* HDRoadManager::instance()
{
	if (!_manager)
	{
		_manager = new HDRoadManager();
	}
	return _manager;
}

HDRoadManager::~HDRoadManager()
{
}

void HDRoadManager::addOneRoad(int id, osg::Vec3dArray* leftArray, osg::Vec3dArray* rightArray)
{
	RoadInfo* info = new RoadInfo();
	info->m_id = id;
	info->m_pLeftArray = leftArray;
	info->m_pRightArray = rightArray;
	m_roadVec.push_back(info);
	m_lastID = id;
}

void HDRoadManager::addOneRoad(int id, osg::Vec3dArray* leftArray, osg::Vec3dArray* rightArray, QString& name, QString& attr)
{
	RoadInfo* info = new RoadInfo();
	info->m_id = id;
	info->m_pLeftArray = leftArray;
	info->m_pRightArray = rightArray;
	m_roadVec.push_back(info);
	m_lastID = id;
	info->m_strName = name;
	info->setAttr(attr);
}

void HDRoadManager::addOneInfo(RoadInfo* info)
{
	m_roadVec.push_back(info);
}

void HDRoadManager::removeRoadById(int id)
{
	for (int i = 0; i < m_roadVec.size(); i++)
	{
		if (m_roadVec[i]->m_id == id)
		{
			m_roadVec.removeAt(i);
			return;
		}
	}
}

void HDRoadManager::clear()
{
	for (int i = 0; i < m_roadVec.size(); i++)
	{
		m_roadVec[i]->clear();
	}
	m_roadVec.clear();
}

int HDRoadManager::genertateID()
{
	int maxID = -1;
	for (int i = 0; i < m_roadVec.size(); i++)
	{
		if (m_roadVec[i]->m_id > maxID)
		{
			maxID = m_roadVec[i]->m_id;
		}
	}
	maxID++;
	return maxID;
}

int HDRoadManager::getLastAddID()
{
	return m_lastID;
}

RoadInfo::RoadInfo()
{
	m_leftType = 0;
	m_rightType = 0;
	m_nTwoSide = false;
	m_strName = QString();
}

QByteArray RoadInfo::getAttr()
{
	QJsonObject obj;
	obj.insert("LType", m_leftType);
	obj.insert("RType", m_rightType);
	obj.insert("TwoSide", m_nTwoSide);

	QJsonDocument doc(obj);
	QByteArray array = doc.toJson();
	return array;
}

void RoadInfo::setAttr(QString& attr)
{
	QJsonDocument jsonDocument = QJsonDocument::fromJson(attr.toUtf8());
	if (jsonDocument.isNull()) {
		qDebug() << "===> QJsonDocument£º" << attr.toUtf8();
		return;
	}
	QJsonObject jsonObject = jsonDocument.object();
	m_leftType = jsonObject["LType"].toInt();
	m_rightType = jsonObject["RType"].toInt();
	m_nTwoSide = jsonObject["TwoSide"].toBool();
}

void RoadInfo::clear()
{
	m_pLeftArray->clear();
	m_pLeftArray = nullptr;
	m_pRightArray->clear();
	m_pRightArray = nullptr;
}

QString RoadInfo::getRoadInfo()
{
	QString info;
	info.append("MULTILINE((");
	for (int i = 0; i < m_pLeftArray->size(); i++)
	{
		info.append(QString::number(m_pLeftArray->at(i).x(), 'g', 15));
		info.append(",");
		info.append(QString::number(m_pLeftArray->at(i).y(), 'g', 14));
		info.append(",");
		info.append(QString::number(0.0));
		//if (i != m_pLeftArray->size() - 1)
		//{
		//	info.append(";");
		//}
		info.append(";");
	}
	info.remove(info.size() - 1, 1);
	info.append("),(");
	for (int i = 0; i < m_pRightArray->size(); i++)
	{
		info.append(QString::number(m_pRightArray->at(i).x(), 'g', 15));
		info.append(",");
		info.append(QString::number(m_pRightArray->at(i).y(), 'g', 14));
		info.append(",");
		info.append(QString::number(0.0));
		//if (i != m_pRightArray->size() - 1)
		//{
		//	info.append(",");
		//}
		info.append(";");
	}
	info.remove(info.size() - 1, 1);
	info.append("))");
	return info;
}

QString RoadInfo::getRoadInfoTmp()
{
	QString info;
	GisModel* model = EngineProxy::instance()->getGlobe();
	info.append("MULTILINE((");
	double x = 0;
	double y = 0;
	for (int i = 0; i < m_pLeftArray->size(); i++)
	{
		model->computePointFromPosition(m_pLeftArray->at(i).x(), m_pLeftArray->at(i).y(), x, y);

		info.append(QString::number(x, 'g', 20));
		info.append(",");
		info.append(QString::number(y, 'g', 20));
		//if (i != m_pLeftArray->size() - 1)
		//{
		//	info.append(";");
		//}
		info.append(";");
	}
	info.remove(info.size() - 1, 1);
	info.append("),(");
	for (int i = 0; i < m_pRightArray->size(); i++)
	{
		model->computePointFromPosition(m_pRightArray->at(i).x(), m_pRightArray->at(i).y(), x, y);

		info.append(QString::number(x, 'g', 20));
		info.append(",");
		info.append(QString::number(y, 'g', 20));
		//if (i != m_pRightArray->size() - 1)
		//{
		//	info.append(",");
		//}
		info.append(";");
	}
	info.remove(info.size() - 1, 1);
	info.append("))");
	return info;
}

