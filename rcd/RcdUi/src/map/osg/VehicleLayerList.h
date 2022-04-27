#ifndef VEHICLELAYERLIST_H
#define VEHICLELAYERLIST_H

#include "LayerList.h"

class VehicleLayer;
class VehicleLayerList : public LayerList
{
public:
	VehicleLayerList();
	virtual ~VehicleLayerList();

	virtual bool pick(int x, int y) { return false; }

	virtual void filterVertex(osg::Vec3d& centerPos) {};



};

#endif