#ifndef _FLATMODEL_H_
#define _FLATMODEL_H_


#include "GisModel.h"

class FlatModel : public GisModel
{
public:
	FlatModel(double minlon = 109.988, double maxlon = 109.998, double minlat = 41.799, double maxlat = 41.804, double width = 1451, double height = 920);
	~FlatModel();


	virtual void convertLatLongHeightToWorldPos(double latitude, double longitude, double height,
		double& X, double& Y, double& Z) const;

	virtual osg::Vec3d convertLatLongHeightToWorldPos(osg::Vec3d& pos) const;

	virtual void convertWorldPosToLatLongHeight(double X, double Y, double Z,
		double& latitude, double& longitude, double& height) const;


	virtual osg::Vec3d convertWorldPosToLatLongHeight(osg::Vec3d& vec) const;

	virtual osg::Vec3d convertScreenXYToLatLongHeight(float x, float y) const;

	virtual osg::Vec3d convertScreenXYToWorldPos(float x, float y) const;

	virtual osg::Vec3d convertScreenXYNormalizedToLatLongHeight(float x, float y) const;

	virtual osg::Vec3d getCenterWorldPos(float x, float y) const;

protected:
	double m_minLon;
	double m_maxLon;
	double m_minLat;
	double m_maxLat;
	double m_screenWidth;
	double m_screenHeight;
	double m_scaleX;
	double m_scaleY;
};



#endif