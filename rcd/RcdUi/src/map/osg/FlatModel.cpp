#include "FlatModel.h"

FlatModel::FlatModel(double minlon /*= 107.0*/, double maxlon /*= 113.0*/, double minlat, double maxlat /*= 45.0*/, double width /*= 1420*/, double height /*= 691*/)
{
	m_minLat = minlat;
	m_maxLat = maxlat;
	m_minLon = minlon;
	m_maxLon = maxlon;
	m_screenHeight = height;
	m_screenWidth = width;
	m_scaleX = (m_maxLon - m_minLon) * 3600 / m_screenWidth;
	m_scaleY = (m_maxLat - m_minLat) * 3600 / m_screenHeight;
}

FlatModel::~FlatModel()
{
}

void FlatModel::convertLatLongHeightToWorldPos(double latitude, double longitude, double height, double& X, double& Y, double& Z) const
{
	X = (longitude - m_minLon) * 3600 / m_scaleX;
	Y = (latitude - m_minLat) * 3600 / m_scaleY;
	Z = 0;

}

osg::Vec3d FlatModel::convertLatLongHeightToWorldPos(osg::Vec3d& pos) const
{
	double x, y, z;
	convertLatLongHeightToWorldPos(pos.y(), pos.x(), pos.z(), x, y, z);
	return osg::Vec3d(x, y, z);
}

void FlatModel::convertWorldPosToLatLongHeight(double X, double Y, double Z, double& latitude, double& longitude, double& height) const
{
	latitude = Y * m_scaleY / 3600 + m_minLat;
	longitude = X * m_scaleX / 3600 + m_minLon;
	height = 0;
}

osg::Vec3d FlatModel::convertWorldPosToLatLongHeight(osg::Vec3d& vec) const
{
	double lon, lat, height;
	convertWorldPosToLatLongHeight(vec.x(), vec.y(), vec.z(), lat, lon, height);
	return osg::Vec3d(lon, lat, height);
}

osg::Vec3d FlatModel::convertScreenXYNormalizedToLatLongHeight(float x, float y) const
{
	double left = m_sector.x() + m_eyePos.x();
	double right = m_sector.y() + m_eyePos.x();
	double bottom = m_sector.z() + m_eyePos.y();
	double top = m_sector.w() + m_eyePos.y();



	double worldposx = left + (right - left) * (x + 1) / 2.0;
	double worldpoxy = top + (bottom - top) * (1 - y) / 2.0;

	double lat, lon, height;
	convertWorldPosToLatLongHeight(worldposx, worldpoxy, 0.0, lat, lon, height);


	return osg::Vec3d(lon, lat, height);
}

osg::Vec3d FlatModel::getCenterWorldPos(float x, float y) const
{
	return convertScreenXYToWorldPos(m_view.x() / 2, m_view.y() / 2);
}

osg::Vec3d FlatModel::convertScreenXYToWorldPos(float x, float y) const
{
	double left = m_sector.x() + m_eyePos.x();
	double right = m_sector.y() + m_eyePos.x();
	double bottom = m_sector.z() + m_eyePos.y();
	double top = m_sector.w() + m_eyePos.y();

	double xScale = x / (double)m_view.x();
	double yScale = y / (double)m_view.y();

	double worldposx = left + (right - left) * xScale;
	double worldposy = bottom + (top - bottom)  * yScale; // (1 - 

	return osg::Vec3d(worldposx, worldposy, 0.0);
}

osg::Vec3d FlatModel::convertScreenXYToLatLongHeight(float x, float y) const
{
	double left = m_sector.x() + m_eyePos.x();
	double right = m_sector.y() + m_eyePos.x();
	double bottom = m_sector.z() + m_eyePos.y();
	double top = m_sector.w() + m_eyePos.y();

	double xScale = x / (double)m_view.x();
	double yScale = y / (double)m_view.y();

	double worldposx = left + (right - left) * xScale;
	double worldposy = bottom + (top - bottom) * yScale; // (1 - 

	double lat, lon, height;
	convertWorldPosToLatLongHeight(worldposx, worldposy, 0.0, lat, lon, height);


	return osg::Vec3d(lon, lat, height);
}

