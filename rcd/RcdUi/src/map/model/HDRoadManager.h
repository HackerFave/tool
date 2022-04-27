#ifndef HDROADMANAGER_H
#define HDROADMANAGER_H
#include <osg/Array>
#include <QVector>


class RoadInfo
{
public:
	RoadInfo();
	int m_id;
	osg::Vec3dArray* m_pLeftArray;
	osg::Vec3dArray* m_pRightArray;
	int m_leftType;
	int m_rightType;
	bool m_nTwoSide;
	QString m_strName;

	QByteArray getAttr();
	void setAttr(QString& attr);

	void clear();
	QString getRoadInfo();
	QString getRoadInfoTmp();
};

class HDRoadManager
{
public:
	static HDRoadManager* instance();
	~HDRoadManager();

	void addOneRoad(int id, osg::Vec3dArray* leftArray, osg::Vec3dArray* rightArray);

	void addOneRoad(int id, osg::Vec3dArray* leftArray, osg::Vec3dArray* rightArray, QString& name, QString& attr);

	void addOneInfo(RoadInfo* info);

	QVector<RoadInfo*>& getRoadVector() { return m_roadVec; }

	void removeRoadById(int id);

	void clear();

	int genertateID();

	int getLastAddID();

protected:
	HDRoadManager();

protected:
	QVector<RoadInfo*> m_roadVec;
	static HDRoadManager* _manager;
	int m_lastID;
};

#endif