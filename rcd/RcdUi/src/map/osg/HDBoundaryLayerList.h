#ifndef HDBOUNDARYLAYERLIST_H
#define HDBOUNDARYLAYERLIST_H

#include "LayerList.h"

class HDBoundaryLayer;
class HDBoundaryLayerList : public LayerList
{
public:
	HDBoundaryLayerList();
	virtual ~HDBoundaryLayerList();

	virtual bool pick(int x, int y) { return false; }

	virtual void filterVertex(osg::Vec3d& centerPos) {};

	HDBoundaryLayer* GetBoundaryLayer(int id, int type);


};

#endif