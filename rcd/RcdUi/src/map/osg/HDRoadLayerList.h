#ifndef HDROADLAYERLIST_H
#define HDROADLAYERLIST_H

#include "LayerList.h"
#include "HDRoadLayer.h"

class  HDRoadLayerList : public LayerList
{
public:
	HDRoadLayerList();
	virtual ~HDRoadLayerList();

	virtual bool pick(int x, int y) { return false; }

	virtual void filterVertex(osg::Vec3d& centerPos) {};

	HDRoadLayer* getLayerById(int id);
};


#endif