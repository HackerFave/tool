#ifndef HDPOLYGONMANAGER_H
#define HDPOLYGONMANAGER_H
#include "PolyInfo.h"

class HDPolygonManager
{
public:
	static HDPolygonManager* instance();
	virtual ~HDPolygonManager();

	PolyInfo* addPolygonArray(int id, int type, osg::Vec3dArray* array);

	PolyInfo* addPolygonArray(int id, int type, osg::Vec3dArray* array, QString& name);

	void removePolyById(int type, int id);

	int generateNewIdByType(int type);

	QVector<PolyInfo*>& getPolyVector() { return m_polyVec; }
	void clear();
protected:
	HDPolygonManager();

protected:
	static HDPolygonManager* _manager;
	QVector<PolyInfo*> m_polyVec;
};


#endif