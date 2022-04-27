#include "GridLayer.h"
#include <osg/Point>
#include "GisModel.h"
#include "EngineProxy.h"





GridLayer::GridLayer()
{
	this->addChild(DrawGrid(-5000.0, 5000.0, 100, -5000.0, 5000.0, 100));
}


GridLayer::~GridLayer()
{
}

void GridLayer::RedrawByUpdate(double lat, double lon, double gridSize, int gridNum)
{


	GisModel* model = EngineProxy::instance()->getGlobe();
	//model->computePointFromPosition()
	osg::Vec3d vertex = model->computeWGSPosFromPosition(osg::Vec3d(lon, lat, 0));
	//osg::Vec3d postion = model->computePositionFromPoint(vertex);
	osg::ref_ptr<osg::Vec3dArray> upDownList = new osg::Vec3dArray();
	osg::ref_ptr<osg::Vec3dArray> leftRightList = new osg::Vec3dArray();
	for (int i = gridNum / 2; i != 0; i--)
	{
		upDownList->push_back(model->computePositionFromWGSPos(osg::Vec3d(vertex.x(), vertex.y() + i * gridSize, vertex.z())));
		leftRightList->push_back(model->computePositionFromWGSPos(osg::Vec3d(vertex.x() - i * gridSize, vertex.y(), vertex.z())));
	}

	upDownList->push_back(osg::Vec3d(lon, lat, 0));
	leftRightList->push_back(osg::Vec3d(lon, lat, 0));

	for (int i = 1; i < gridNum / 2 + 1; i++)
	{
		upDownList->push_back(model->computePositionFromWGSPos(osg::Vec3d(vertex.x(), vertex.y() - i * gridSize, vertex.z())));
		leftRightList->push_back(model->computePositionFromWGSPos(osg::Vec3d(vertex.x() + i * gridSize, vertex.y(), vertex.z())));
	}
	int listSize = upDownList->size();

	double xMax = model->convertLatLongHeightToWorldPos(leftRightList->at(listSize - 1)).x();
	double xMin = model->convertLatLongHeightToWorldPos(leftRightList->at(0)).x();
	double yMax = model->convertLatLongHeightToWorldPos(upDownList->at(listSize - 1)).y();
	double yMin = model->convertLatLongHeightToWorldPos(upDownList->at(0)).y();

	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	
	double dx, dy;
	for (int i = 0; i < listSize; i++)
	{
		dx = model->convertLatLongHeightToWorldPos(leftRightList->at(i)).x();
		vertices->push_back(osg::Vec3(dx, yMin, -999));
		vertices->push_back(osg::Vec3(dx, yMax, -999));
		dy = model->convertLatLongHeightToWorldPos(upDownList->at(i)).y();
		vertices->push_back(osg::Vec3(xMin, dy, -999));
		vertices->push_back(osg::Vec3(xMax, dy, -999));
	}
	m_pGeometry->setVertexArray(vertices.get());
	osg::ref_ptr<osg::DrawArrays> lines = new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertices.get()->size());
	m_pGeometry->removePrimitiveSet(0, m_pGeometry->getNumPrimitiveSets());
	m_pGeometry->addPrimitiveSet(lines.get());
	m_pGeometry->dirtyGLObjects();

}

osg::Group* GridLayer::DrawGrid(double xMin, double xMax, int numCollumns, double yMin, double yMax, int numRows)
{
	osg::Group* group = new osg::Group();
	//mGridGroup->removeChildren(0, mGridGroup->getNumChildren());
	//Create geometry
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	m_pGeometry = new osg::Geometry();
	m_pGeometry->setDataVariance(osg::Object::DYNAMIC);
	//arrays for vertices
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	m_pGeometry->setVertexArray(vertices.get());
	//array  for colors
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.6f, 0.6f, 0.6f, 1.0f));
	m_pGeometry->setColorArray(colors.get());
	m_pGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	double step = fabs(xMax - xMin) / numCollumns;
	//create vertices
	for (double s = xMin; s <= xMax; s += step)
	{
		vertices->push_back(osg::Vec3(s, yMin, -999));
		vertices->push_back(osg::Vec3(s, yMax, -999));
	}


	step = fabs(yMax - yMin) / numRows;
	for (double s = yMin; s <= yMax; s += step)
	{
		vertices->push_back(osg::Vec3(xMin, s, -999));
		vertices->push_back(osg::Vec3(xMax, s, -999));

	}

	//lines
	osg::ref_ptr<osg::DrawArrays> lines = new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertices.get()->size());
	m_pGeometry->addPrimitiveSet(lines.get());
	m_pGeometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	m_pGeometry->getOrCreateStateSet()->setAttribute(new osg::Point(2.0f), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	m_pGeometry->setUseDisplayList(false);
	m_pGeometry->setUseVertexBufferObjects(true);
	geode->addDrawable(m_pGeometry.get());
	group->addChild(geode);
	return group;
}

