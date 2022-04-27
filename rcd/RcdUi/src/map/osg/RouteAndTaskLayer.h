#ifndef ROUTEANDTASKLAYER_H
#define ROUTEANDTASKLAYER_H

#include "Layer.h"
#include <QList>
#include "Marker.h"

class RouteAndTaskLayer : public Layer
{
public:
	RouteAndTaskLayer();
	virtual ~RouteAndTaskLayer();

	void DrawTaskLane(osg::Vec3dArray* listInfo);
	void UpdateTaskVertexArray(osg::Vec3dArray* listInfo);
	void RedrawTaskLane();


	void UpdateRouteLane(int start, int end);

protected:
	void updateRouteArray(int start, int end);

	void redrawRouteLine(int start, int end);

protected:
	osg::Vec3dArray* m_pOriginArray;
	osg::Vec3Array* m_pVertexArray;
	osg::Vec3Array* m_pRouteArray;
	osg::Vec4 m_color;
	osg::Vec4 m_RouteColor;
	int m_layerHeight;
	int m_routeHeight;
	osg::ref_ptr<osg::Geometry> m_geometry;

	osg::ref_ptr<osg::Geometry> m_geomRoute;
	Marker* m_startMarker;
	Marker* m_endMarker;

	int m_start;
	int m_end;

	osg::PositionAttitudeTransform* m_text;

	
};

#endif