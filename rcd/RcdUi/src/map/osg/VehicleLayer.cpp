#include "VehicleLayer.h"
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/Vec3>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include "EngineProxy.h"
#include "GisModel.h"
#include <QDebug>
using namespace std;


VehicleLayer::VehicleLayer(int type)
{
	m_type = type;
	createVehicle();
}

VehicleLayer::~VehicleLayer()
{
}

void VehicleLayer::UpdateLatLon(double lat, double lon)
{
	GisModel* model = EngineProxy::instance()->getGlobe();
	osg::Vec3d latlon(lon, lat, 0.0);
	this->setPosition(model->convertLatLongHeightToWorldPos(latlon));
}

void VehicleLayer::UpdateLatLon(osg::Vec3d& pos)
{
	GisModel* model = EngineProxy::instance()->getGlobe();
	this->setPosition(model->convertLatLongHeightToWorldPos(pos));
}

void VehicleLayer::UpdateHeading(double fAngle)
{
	this->setAttitude(osg::Quat(osg::DegreesToRadians(fAngle), osg::Vec3d(0.0, 0.0, -1.0)));
}

void VehicleLayer::createVehicle()
{
	QString files = EngineProxy::instance()->getResourceByType(m_type);
	osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile(files.toStdString()); ///Images/primitives.gif
	if (!image) return;


	osg::Geometry* polyGeom = new osg::Geometry();

	// note, anticlockwise ordering.
	osg::Vec3 myCoords[] =
	{
		osg::Vec3(-3.0f,6.0f,0.0f),
		osg::Vec3(-3.0f,-6.0f,0.0f),
		osg::Vec3(3.0f,-6.0f,0.0f),
		osg::Vec3(3.0f,6.0f,0.0f)
	};

	int numCoords = sizeof(myCoords) / sizeof(osg::Vec3);


	polyGeom->setVertexArray(new osg::Vec3Array(numCoords, myCoords));

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	polyGeom->setColorArray(colors, osg::Array::BIND_OVERALL);


	// Set the normal in the same way as the color.
	osg::Vec3Array* normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
	polyGeom->setNormalArray(normals, osg::Array::BIND_OVERALL);

	osg::Vec2 myTexCoords[] =
	{
		osg::Vec2(0,1),
		osg::Vec2(0,0),
		osg::Vec2(1,0),
		osg::Vec2(1,1)
	};

	int numTexCoords = sizeof(myTexCoords) / sizeof(osg::Vec2);

	// pass the created tex coord array to the points geometry object,
	// and use it to set texture unit 0.
	polyGeom->setTexCoordArray(0, new osg::Vec2Array(numTexCoords, myTexCoords));

	// we'll use indices and DrawElements to define the primitive this time.
	unsigned short myIndices[] =
	{
		0,
		1,
		3,
		2
	};

	int numIndices = sizeof(myIndices) / sizeof(unsigned short);

	polyGeom->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLE_STRIP, numIndices, myIndices));

	osg::StateSet* stateset = new osg::StateSet;

	osg::Texture2D* texture = new osg::Texture2D;
	texture->setImage(image);

	stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

	polyGeom->setStateSet(stateset);

	osg::Geode* geode = new osg::Geode();

	geode->addDrawable(polyGeom);

	geode->getOrCreateStateSet()->setMode(GL_LIGHTING,
		osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);

	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	geode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	geode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	addChild(geode);
}
