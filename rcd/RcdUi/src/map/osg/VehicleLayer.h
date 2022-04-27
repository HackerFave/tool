#ifndef VEHICLELAYER_H
#define VEHICLELAYER_H

#include "Layer.h"
#include <osg/Geometry>
#include "HDRoadLayer.h"


class PolyInfo;
class GisModel;

class VehicleLayer : public Layer
{
public:
	VehicleLayer(int type);
	virtual ~VehicleLayer();

	virtual bool pick(int x, int y) { return true; };

	virtual void filterVertex(osg::Vec3d& centerPos) {}

	void UpdateLatLon(double lat, double lon);

	void UpdateLatLon(osg::Vec3d& pos);

	void UpdateHeading(double fAngle);
protected:
	void createVehicle();

protected:
	osg::Geode* m_geode;
	int m_type;
};

#endif