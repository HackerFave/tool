
#ifndef _GISMODEL_H_
#define _GISMODEL_H_

//#include <osg/vec3d>
#include "ViewChangeListener.h"
#include <osg/Vec2i>
#include "proj/proj_api.h"
#include <osg/Vec3d>
class  GisModel : public ViewChangeListener
{
public:
	GisModel();
	virtual ~GisModel();


	virtual void convertLatLongHeightToWorldPos(double latitude, double longitude, double height,
		double& X, double& Y, double& Z) const = 0;

	virtual osg::Vec3d convertLatLongHeightToWorldPos(osg::Vec3d& pos) const = 0;

	virtual void convertWorldPosToLatLongHeight(double X, double Y, double Z,
		double& latitude, double& longitude, double& height) const = 0;

	virtual osg::Vec3d convertWorldPosToLatLongHeight(osg::Vec3d& vec) const = 0;

	virtual osg::Vec3d convertScreenXYToWorldPos(float x, float y) const = 0;

	virtual osg::Vec3d getCenterWorldPos(float x, float y) const = 0;

	virtual osg::Vec3d convertScreenXYToLatLongHeight(float x, float y) const = 0;

	virtual osg::Vec3d convertScreenXYNormalizedToLatLongHeight(float x, float y) const = 0;

	virtual bool viewChanged(const ViewChangeEvent& event);

	virtual osg::Vec3d geodeticToCartesian(double longitude, double latitude, double metersElevation) const;

	virtual osg::Vec3d computeWGSPosFromPosition(const osg::Vec3d& position) const;

	virtual void computePointFromPosition(double lon, double lat, double& x, double& y);

	virtual osg::Vec3d cartesianToGeodetic(const osg::Vec3d& cart) const;

	virtual osg::Vec3d computePositionFromWGSPos(const osg::Vec3d& point) const;

	virtual osg::Vec3d convertWorldPosToWGSPos(const osg::Vec3d& cart) const;

	virtual osg::Vec3d convertWGSPosToWorldPos(const osg::Vec3d& cart) const;

protected:
	osg::Vec4d m_sector;
	osg::Vec3d m_eyePos;
	double m_scaleRadio;
	osg::Vec2i m_view;

	projPJ pj_merc, pj_latlong;
};

#endif
