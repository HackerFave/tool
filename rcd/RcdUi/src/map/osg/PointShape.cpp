#include "PointShape.h"
#include <osg/Point>
PointShape::PointShape()
{
	//setColorBinding(osg::Geometry::BIND_OVERALL);
	setName("PointShape");
	//_firstColor = true;
	//_firstRadius = true;
	//_firstCenter = true;
	initPointShape();
}

PointShape::~PointShape()
{

}

//void PointShape::set(const osg::Vec3& center,float radius, const osg::Vec4& color)
//{
//	_center =center; 
//	_radius = radius;
//	_color = color;
//
//	osg::Point* ppoint = (osg::Point*)(getOrCreateStateSet()->getMode(osg::StateAttribute::POINT));
//	if (ppoint)
//	{
//		ppoint->setSize(radius);
//	}
//	else
//	{
//		ppoint = new osg::Point(radius);
//		getStateSet()->setAttributeAndModes(ppoint);
//	}
//	
//
//	osg::Vec4Array* array = (osg::Vec4Array*)getColorArray();
//	if (array->size() == 0)
//	{
//		array->push_back(_color);
//	}
//	else
//	{
//		array->at(0).set(color.x(), color.y(),color.z(),color.w());
//	}
//
//	osg::Vec3Array* vecarray = (osg::Vec3Array*)getVertexArray();
//	if (vecarray->size() == 0)
//	{
//		vecarray->push_back(_center);
//	}
//	else
//	{
//		vecarray->at(0).set(_center.x(), _center.y(),_center.z());
//	}
//
//
//}

void PointShape::setRadius( float radius )
{
	//_radius = radius;

	//
	//if (!_firstRadius)
	//{
	//	osg::Point* ppoint = (osg::Point*)(getStateSet()->getAttribute(osg::StateAttribute::POINT));
	//	ppoint->setSize(radius);
	//}
	//else
	//{
	//	osg::Point* ppoint = new osg::Point(radius);
	//	getOrCreateStateSet()->setAttributeAndModes(ppoint,osg::StateAttribute::ON);
	//	_firstRadius = false;
	//}
	

}

float PointShape::getRadius() const
{
	return _radius;
}

void PointShape::setCenter(const osg::Vec3& val )
{
	_center.set(val.x(), val.y(), val.z());

	
	osg::Vec3Array* vecarray;
	if (_firstCenter)
	{
		vecarray = new osg::Vec3Array();
		vecarray->push_back(_center);
		//vecarray->push_back(osg::Vec3(val.x(), val.y(), val.z() + 40.0));
		setVertexArray(vecarray);
		_firstCenter = false;
		dirtyDisplayList();
	}
	else
	{
		vecarray = (osg::Vec3Array*)getVertexArray();
		vecarray->at(0).set(val.x(), val.y(),val.z());
		dirtyDisplayList();
		//vecarray->at(1).set(val.x(), val.y(), val.z() + 40.0);
	}

}


const osg::Vec4& PointShape::getColor() const
{
	return _color;
}

void PointShape::setColor( const osg::Vec4& color )
{
	_color = color;


	
	
	if (_firstColor)
	{
		osg::Vec4Array* array = new osg::Vec4Array();
		array->push_back(_color);
		_firstColor = false;
		setColorArray(array);
		dirtyDisplayList();
	}
	else
	{
		osg::Vec4Array* array = (osg::Vec4Array*)getColorArray();
		array->at(0).set(color.x(), color.y(),color.z(),color.w());
		dirtyDisplayList();
	}
	
}
const osg::Vec3& PointShape::getCenter() const
{
	return _center;
}

void PointShape::initPointShape()
{
	//_firstRadius = 10.0;
	//osg::Point* ppoint = new osg::Point(_firstRadius);
	//getOrCreateStateSet()->setAttributeAndModes(ppoint,osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	//_firstRadius = false;

	osg::Vec3Array* vecarray = new osg::Vec3Array();
	vecarray->push_back(osg::Vec3(0.0,0.0,0.0));
	//vecarray->push_back(osg::Vec3(0.0,0.0,40.0));
	setVertexArray(vecarray);
	_firstCenter = false;

	osg::Vec4Array* array = new osg::Vec4Array();
	array->push_back(_color);
	_firstColor = false;
	
	setColorArray(array);
	setColorBinding(osg::Geometry::BIND_OVERALL);
	setDataVariance(osg::Geometry::DYNAMIC);
	setUseDisplayList(false);
	setUseVertexBufferObjects(true);
	this->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS,0,1));

}

