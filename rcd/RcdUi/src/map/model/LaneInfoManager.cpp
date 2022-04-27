#include "LaneInfoManager.h"


LaneInfoManager* LaneInfoManager::_manager = 0;
LaneInfoManager::LaneInfoManager()
{
}


LaneInfoManager* LaneInfoManager::instance()
{
	if (!_manager)
	{
		_manager = new LaneInfoManager();
	}
	return _manager;
}

LaneInfoManager::~LaneInfoManager()
{
}

void LaneInfoManager::addLaneInfo(LaneInfo* laneInfo)
{
	m_laneInfoList.push_back(laneInfo);
}

void LaneInfoManager::clearLaneInfo()
{
	for (int i = 0; i < m_laneInfoList.size(); i++)
	{
		delete m_laneInfoList[i];
		m_laneInfoList[i] = nullptr;
	}
	m_laneInfoList.clear();
}

int LaneInfoManager::genertateID()
{
	int maxID = -1;
	for (int i = 0; i < m_laneInfoList.size(); i++)
	{
		if (m_laneInfoList[i]->getLaneId() > maxID)
		{
			maxID = m_laneInfoList[i]->getLaneId();
		}
	}
	maxID++;
	return maxID;
}

void LaneInfoManager::removeLaneById(int id)
{
	for (int i = 0; i < m_laneInfoList.size(); i++)
	{
		if (m_laneInfoList[i]->getLaneId() == id)
		{
			m_laneInfoList.removeAt(i);
			return;
		}
	}
}

bool LaneInfoManager::findNodeIdUsedByOther(int nodeId, int exceptLaneId)
{
	for (int i = 0; i < m_laneInfoList.size(); i++)
	{
		if (m_laneInfoList[i]->getPreNodeId() == nodeId ||
			m_laneInfoList[i]->getSucNodeId() == nodeId)
		{
			if (m_laneInfoList[i]->getLaneId() != exceptLaneId)
			{
				return true;
			}
		}
	}
	return false;
}

