#ifndef LANEINFOMANAGER_H
#define LANEINFOMANAGER_H
#include "LaneInfo.h"
class  LaneInfoManager
{
public:
	static LaneInfoManager* instance();

	virtual ~LaneInfoManager();

	QList<LaneInfo*>& getLaneInfoList() { return m_laneInfoList; }
	void addLaneInfo(LaneInfo* laneInfo);

	void clearLaneInfo();

	int genertateID();

	void removeLaneById(int id);

	bool findNodeIdUsedByOther(int nodeId, int exceptLaneId);

protected:
	LaneInfoManager();



protected:
	static LaneInfoManager* _manager;
	QList<LaneInfo*> m_laneInfoList;
};



#endif // !LANEINFOMANAGER_H