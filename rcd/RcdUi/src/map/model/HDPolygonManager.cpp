#include "HDPolygonManager.h"


HDPolygonManager* HDPolygonManager::_manager = 0;
HDPolygonManager::HDPolygonManager()
{
}


HDPolygonManager* HDPolygonManager::instance()
{
	if (!_manager)
	{
		_manager = new HDPolygonManager();
	}
	return _manager;
}

HDPolygonManager::~HDPolygonManager()
{
}

PolyInfo* HDPolygonManager::addPolygonArray(int id, int type, osg::Vec3dArray* array)
{
	PolyInfo* info = new PolyInfo();
	info->setInfos(id, type, array);
	m_polyVec.push_back(info);
	return info;
}

PolyInfo* HDPolygonManager::addPolygonArray(int id, int type, osg::Vec3dArray* array, QString& name)
{
	PolyInfo* info = new PolyInfo();
	info->setInfos(id, type, array);
	info->setName(name);
	m_polyVec.push_back(info);
	return info;
}

void HDPolygonManager::removePolyById(int type, int id)
{
	for (int i = 0; i < m_polyVec.size(); i++)
	{
		if (m_polyVec[i]->getID() == id && m_polyVec[i]->getType() == type)
		{
			m_polyVec.removeAt(i);
			return;
		}
	}

}

int HDPolygonManager::generateNewIdByType(int type)
{
	int id = -1;
	for (int i = 0; i < m_polyVec.size(); i++)
	{
		if (m_polyVec[i]->getType() == type)
		{
			id = m_polyVec[i]->getID();
		}
	}
	id++;
	return id;
}

void HDPolygonManager::clear()
{
	m_polyVec.clear();
}
