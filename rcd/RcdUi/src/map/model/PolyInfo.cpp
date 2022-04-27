#include "PolyInfo.h"
#include "EngineProxy.h"
#include "GisModel.h"


void PolyInfo::setInfos(int id, int type, osg::Vec3dArray* array)
{
	m_id = id;
	m_type = type;
	m_array = array;
}

QString PolyInfo::getPolyInfo()
{
	QString info = QString("POLYGON((");
	double x = 0;
	double y = 0;
	for (int i = 0; i < m_array->size(); i++)
	{
		info.append(QString::number(m_array->at(i).x(), 'g', 15));
		info.append(" ");
		info.append(QString::number(m_array->at(i).y(), 'g', 14));
		info.append(",");
	}

	info.append(QString::number(m_array->at(0).x(), 'g', 15));
	info.append(" ");
	info.append(QString::number(m_array->at(0).y(), 'g', 14));
	info.append("))");
	return info;
}

QString PolyInfo::getMercInfo()
{
	QString info = QString("POLYGON((");
	double x = 0;
	double y = 0;
	GisModel* model = EngineProxy::instance()->getGlobe();
	for (int i = 0; i < m_array->size(); i++)
	{
		model->computePointFromPosition(m_array->at(i).x(), m_array->at(i).y(), x, y);
		info.append(QString::number(x, 'g', 20));
		info.append(" ");
		info.append(QString::number(y, 'g', 20));
		info.append(",");
	}

	model->computePointFromPosition(m_array->at(0).x(), m_array->at(0).y(), x, y);
	info.append(QString::number(x, 'g', 20));
	info.append(" ");
	info.append(QString::number(y, 'g', 20));
	info.append("))");
	return info;
}

QString PolyInfo::getPolyInfoByHD()
{
	QString info = QString("POLYGON((");
	for (int i = 0; i < m_array->size(); i++)
	{
		info.append(QString::number(m_array->at(i).x(), 'g', 15));
		info.append(",");
		info.append(QString::number(m_array->at(i).y(), 'g', 14));
		info.append(",");
		info.append(QString::number(0.0));
		info.append(";");
	}

	//info.remove(info.size() - 1, 1);
	info.append(QString::number(m_array->at(0).x(), 'g', 15));
	info.append(",");
	info.append(QString::number(m_array->at(0).y(), 'g', 14));
	info.append(",");
	info.append(QString::number(0.0));
	info.append("))");
	return info;
}

QString PolyInfo::getPolyInfoTmpByHD()
{
	QString info = QString("POLYGON((");
	double x = 0;
	double y = 0;
	GisModel* model = EngineProxy::instance()->getGlobe();
	for (int i = 0; i < m_array->size(); i++)
	{

		
		model->computePointFromPosition(m_array->at(i).x(), m_array->at(i).y(), x, y);
		info.append(QString::number(x, 'g', 20));
		info.append(",");
		info.append(QString::number(y, 'g', 20));
		info.append(";");
	}

	info.remove(info.size() - 1, 1);
	info.append("))");
	return info;
}

void PolyInfo::updatePosArray(osg::Vec3dArray* array)
{
	//m_array->clear();
	//for (int i = 0; i < array->size(); i++)
	//{
	//	m_array->push_back(array->at(i));
	//}
}

