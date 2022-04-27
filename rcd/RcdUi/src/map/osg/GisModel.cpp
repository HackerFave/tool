#include "GisModel.h"


GisModel::GisModel()
{
	if (!(pj_merc = pj_init_plus("+proj=merc +lon_0=0 +k=1 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs")))
		exit(1);
	if (!(pj_latlong = pj_init_plus("+proj=longlat +datum=WGS84 +no_defs")))
		exit(1);
}


GisModel::~GisModel()
{
}

bool GisModel::viewChanged(const ViewChangeEvent& event)
{
	m_eyePos = event.eyePos;
	m_scaleRadio = event.scaleRadio;
	m_sector = event.sector;
	m_view = event.view;
	return true;
}

osg::Vec3d GisModel::geodeticToCartesian(double longitude, double latitude, double metersElevation) const
{

	double _radiusEquator = 6378137.0;
	//double _eccentricitySquared = 0.00669437999013;

	//double cosLat = ::cos(osg::DegreesToRadians(latitude));
	//double sinLat = ::sin(osg::DegreesToRadians(latitude));

	//double cosLon = ::cos(osg::DegreesToRadians(longitude));
	//double sinLon = ::sin(osg::DegreesToRadians(longitude));

	//double rpm =
	//	_radiusEquator / ::sqrt(1.0 - _eccentricitySquared * sinLat * sinLat);

	//double x = (rpm + metersElevation) * cosLat * sinLon;
	//double y = (rpm * (1.0 - _eccentricitySquared) + metersElevation) * sinLat;
	//double z = (rpm + metersElevation) * cosLat * cosLon;
	//return  osg::Vec3d(z, x, y);

	return osg::Vec3d(_radiusEquator * osg::DegreesToRadians(longitude),
		_radiusEquator * osg::DegreesToRadians(latitude),
		metersElevation);


}

osg::Vec3d GisModel::computeWGSPosFromPosition(const osg::Vec3d& position) const
{
	//double lat = position.y();
	//double lon = position.x();
	//double abs = 0.00000001;
	////if(lat.getDegrees() > 90+abs || lat.getDegrees() < -90-abs || lon.getDegrees() > 180+abs || lon.getDegrees() < -180-abs)
	////	return GeoVec4d::NAN;
	//if (lat > 90 + abs) lat = 90;
	//if (lat < -90 - abs)lat = -90;
	//if (lon > 180 + abs) lon = 180;
	//if (lon < -180 - abs) lon = -180;
	//使用proj库进行计算之前的不用
	//osg::Vec3d old = this->geodeticToCartesian(position.x(), position.y(), position.z());
	double x, y;
	x = position.x();
	y = position.y();
	x *= DEG_TO_RAD;
	y *= DEG_TO_RAD;
	pj_transform(pj_latlong, pj_merc, 1, 1, &x, &y, NULL);

	return osg::Vec3d(x, y, position.z());
}

void GisModel::computePointFromPosition(double lon, double lat, double& x, double& y)
{
	double cgx = 0.0;
	double cgy = 0.0;
	cgx = lon;
	cgy = lat;
	cgx *= DEG_TO_RAD;
	cgy *= DEG_TO_RAD;

	pj_transform(pj_latlong, pj_merc, 1, 1, &cgx, &cgy, NULL);
	x = cgx;
	y = cgy;
}

osg::Vec3d GisModel::cartesianToGeodetic(const osg::Vec3d& cart) const
{
	double _radiusEquator = 6378137.0;
	//double _eccentricitySquared = 0.00669437999013;

	double x = osg::RadiansToDegrees(cart.x() / _radiusEquator);
	double y = osg::RadiansToDegrees(cart.y() / _radiusEquator);
	return osg::Vec3d(x, y, cart.z());
}

osg::Vec3d GisModel::computePositionFromWGSPos(const osg::Vec3d& point) const
{
	//使用proj库进行计算之前的不用
	//osg::Vec3d old = this->cartesianToGeodetic(point);

	double x, y;
	x = point.x();
	y = point.y();

	pj_transform(pj_merc, pj_latlong, 1, 1, &x, &y, NULL);

	x = x / DEG_TO_RAD;
	y = y / DEG_TO_RAD;


	return osg::Vec3d(x, y, point.z());
}

osg::Vec3d GisModel::convertWorldPosToWGSPos(const osg::Vec3d& cart) const
{
	osg::Vec3d pos = cart;
	pos = convertWorldPosToLatLongHeight(pos);
	return computeWGSPosFromPosition(pos);
}

osg::Vec3d GisModel::convertWGSPosToWorldPos(const osg::Vec3d& cart) const
{
	osg::Vec3d pos = cart;
	pos = computePositionFromWGSPos(pos);
	return convertLatLongHeightToWorldPos(pos);
}
