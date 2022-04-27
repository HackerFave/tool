#ifndef HDLANELAYERLIST_H
#define HDLANELAYERLIST_H

#include "LayerList.h"
#include "HDLaneLayer.h"
#include <QVector>

class HDLaneLayerList : public LayerList
{
public:
	HDLaneLayerList();
	virtual ~HDLaneLayerList();

	virtual bool pick(int x, int y) { return false; }

	virtual void filterVertex(osg::Vec3d& centerPos) {};

	QVector<HDLaneLayer*> findLaneLayerByRoadElementId(int id);
	QVector<HDLaneLayer*> findLaneLayerByBoundaryIdAndType(int id, int type);
	HDLaneLayer* findLaneLayerById(int id);

	int findLaneIdByPreNodeid(int nodeid);

	int findLaneIdBySucNodeid(int nodeid);

	QVector<HDLaneLayer*> findLaneLayerByPreNodeid(int id);
	QVector<HDLaneLayer*> findLaneLayerBySucNodeid(int id);

protected:
	QVector<HDLaneLayer*> findLaneLayer(int id, int type);
};

#endif