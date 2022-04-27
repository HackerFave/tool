#ifndef _GRIDLAYER_H_
#define _GRIDLAYER_H_

#include "Layer.h"
#include "ViewChangeListener.h"
#include <osg/Geode>
#include <osg/Geometry>
class GridLayer : public Layer
{
public:
	GridLayer();
	virtual ~GridLayer();


	virtual bool pick(int x, int y) { return false; };

	virtual void filterVertex(osg::Vec3d& centerPos) {};



	void RedrawByUpdate(double lat, double lon, double gridSize = 100.0, int gridNum = 1000);
protected:
	osg::Group*  DrawGrid(double xMin, double xMax, int numCollumns, double yMin, double yMax, int numRows);

protected:
	osg::ref_ptr<osg::Geometry> m_pGeometry;
};



#endif