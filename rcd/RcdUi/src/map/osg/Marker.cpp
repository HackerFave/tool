#include "Marker.h"

#include <osg/Shape>
#include <osg/Point>
#include "PointShape.h"
#include <stdio.h>
#include <cstdio>
#include <iostream>
Marker::Marker(MarkerShape::ShapeType shapeType)
:osg::PositionAttitudeTransform()
{
	_Type = shapeType;
	_needText =true;
 	InitGeode();
}

//Marker::Marker(MarkerShape::ShapeType shapeType, const osg::Vec3d& pos)
//:osg::PositionAttitudeTransform()
//{
//	this->setPosition(pos);
//	_needText =true;
//	_Type = shapeType;
//	InitGeode();
//}

Marker::Marker(MarkerShape::ShapeType shapeType, const osg::Vec3d& pos)
{
/*	osg::Vec3d p = osg::Vec3d(pos.getLatitude().getDegrees(), pos.getLongitude().getDegrees(), pos.getElevation());
	Marker(shapeType, p)*/;
	_Type = shapeType;
	setPosition(pos);
	_needText =true;
	//_Type = shapeType;
	InitGeode();
}

Marker::Marker(MarkerShape::ShapeType shapeType, osg::Material* material)
:osg::PositionAttitudeTransform(),
_material(material)
{
	_Type = shapeType;
	_needText =true;
	InitGeode();
}
Marker::Marker(MarkerShape::ShapeType shapeType, const PositionAttitudeTransform& pat, osg::Material* material)
:osg::PositionAttitudeTransform(pat),
_material(material)
{
	_Type = shapeType;
	_needText =true;
	InitGeode();
}

void Marker::InitGeode()
{
	osg::Vec3 tempCenter = osg::Vec3(0,0,0);
	osg::Vec3 tempLenght = osg::Vec3(1,1,1);
	float tempheight = 1.0;
	osg::ref_ptr<MarkerShape> shape;

	switch(_Type)
	{
	case MarkerShape::Point:
		{
 			shape = new MarkerShape();
			break;
		}
	case MarkerShape::Sphere:
		{
			shape = new MarkerShape(new osg::Sphere());
			break;
		}
	case MarkerShape::Box:
		{
			shape = new MarkerShape(tempCenter, tempLenght);
			break;
		}
	case MarkerShape::Cylinder:
		{
			shape = new MarkerShape(MarkerShape::Cylinder,tempCenter, tempheight, tempheight);
			break;
		}
	case MarkerShape::Cone:
		{
			shape = new MarkerShape(MarkerShape::Cone,tempCenter, tempheight, tempheight);
			break;
		}
	case MarkerShape::Capsule:
		{
			shape = new MarkerShape(MarkerShape::Capsule,tempCenter, tempheight, tempheight);
			break;
		}
	default:
		{
			shape = new MarkerShape();
			break;
		}	
	}
	
	this->addChild(shape);
	if(_Type != MarkerShape::Point)
	{
		if (!_material)
		{
			_material = new osg::Material;
			_material->setAmbient(osg::Material::FRONT_AND_BACK,osg::Vec4(1,1,1,1));	// 环境光
			//_material->setDiffuse(osg::Material::Face::FRONT_AND_BACK,osg::Vec4(1,1,1,1));// 散射光
			_material->setSpecular(osg::Material::FRONT_AND_BACK,osg::Vec4(1,1,1,1));// 镜面光
			//_material->setEmission(osg::Material::Face::FRONT_AND_BACK,osg::Vec4(1,1,1,1));// 发射光
			_material->setShininess(osg::Material::FRONT_AND_BACK,80);// 光点大小
			_material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
		}
		_stateSet = new osg::StateSet;
		_stateSet->setAttribute(_material, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
		_stateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
		_stateSet->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON |osg::StateAttribute::PROTECTED);
		
		//PositionAttitudeTransform::setStateSet(_stateSet);
		this->setStateSet(_stateSet); 
	}
	else
	{
		_stateSet = new osg::StateSet;
		
		//osg::Point* po = new osg::Point(10);
		//_stateSet->setAttributeAndModes(po,osg::StateAttribute::ON);
		_stateSet->setMode(GL_LIGHTING,osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
		_stateSet->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
		//state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		//_stateSet->setAttributeAndModes(new osg::BlendFunc(),osg::StateAttribute::ON);
		osg::Point* ppoint = new osg::Point(10);
		_stateSet->setAttributeAndModes(ppoint,osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
		_stateSet->setRenderBinDetails(50000000,"RenderBin");
		setStateSet(_stateSet);
	}


	_isScaleSelf = true;
	_radius = tempheight;
	_isShowFullShape = false;


	//_text = NULL;
	//if (_needText)
	//{
	//	createText();
	//}
}

void Marker::setSize(float f_value)
{

	if (_Type != MarkerShape::Sphere || _Type != MarkerShape::Point)
	{
		return;
	}

	((MarkerShape*)getChild(0))->markerShapeSet(f_value);
	_radius = f_value;
}

void Marker::setSize(osg::Vec3& halfLengths)
{
	if (_Type != MarkerShape::Sphere)
	{
		return;
	}

	((MarkerShape*)getChild(0))->markerShapeSet(halfLengths);
	_radius = halfLengths.x() > halfLengths.y() ? halfLengths.x() : halfLengths.y();
	_radius = _radius > halfLengths.z() ? _radius : halfLengths.z();
}

void Marker::setSize(float radius, float height)
{
	if (_Type != MarkerShape::Sphere)
	{
		return;
	}

	((MarkerShape*)getChild(0))->markerShapeSet(radius, height);
	_radius = radius;
}

void Marker::setMaterial(osg::Material* material) 
{ 
	_material = _material;
	_stateSet->setAttribute(_material, osg::StateAttribute::ON);
}

void Marker::setColor(const osg::Vec4& color)
{
	((MarkerShape*)getChild(0))->setColor(color);
}

const osg::Vec4& Marker::getColor()
{
	return ((MarkerShape*)getChild(0))->getColor();
}

//void Marker::setPosition(const osg::Vec3d& pos)
//{
//	_position = pos;
//	//osg::Vec3d po(pos);
//	//po.z() += _radius;
//	//_position = po;
//	//osg::Vec3d position = BaseCommonMethod::comptuerPositionFromPoint(_position);
//	//_pos = GTGis::Position::fromDegrees(position.x(),position.y(),position.z());
//	PositionAttitudeTransform::setPosition(pos);
//}

void Marker::setScale(const osg::Vec3d& scale)
{
	if (_Type == MarkerShape::Point)
	{
		return;
	}
	osg::Vec3d pos = PositionAttitudeTransform::getPosition();
	//float radius = _radius;
	//radius *= scale.x();
	//pos.z() += radius;
	PositionAttitudeTransform::setPosition(pos);
	PositionAttitudeTransform::setScale(scale);
}

void Marker::setRatio(float ratio)
{
	((MarkerShape*)getChild(0))->setRatio(ratio);
}
float Marker::getRatio()
{
	return ((MarkerShape*)getChild(0))->getRatio();
}

//void GTScene::Marker::setPosition( const GTGis::Position& pos )
//{
//	_pos = pos;
//	if (_needText)
//	{
//		createText();
//	}
//	_position =BaseCommonMethod::computerPointFromPosition(_pos.getLatitude().getDegrees(), _pos.getLongitude().getDegrees(), _pos.getElevation());
//	PositionAttitudeTransform::setPosition(_position);
//}

void Marker::setPointNoUpdate( const osg::Vec3& pos )
{
	_position = pos;
	PositionAttitudeTransform::setPosition(pos);
}

//void GTScene::Marker::setPositionNoUpdate( const GTGis::Position& pos )
//{
//	_pos = pos;
//}

void Marker::setTextNeed( bool need )
{
	//if (need == _needText)
	//{
	//	return ;
	//}
	//
	//_needText = need;

	//if (_needText)
	//{
	//	createText();
	//}
	//else
	//{
	//	if (getChild(1) != NULL)
	//	{
	//		getChild(1)->setNodeMask(false);
	//	}
	//}

}

void Marker::setTextColor( const osg::Vec4& color )
{
	//_text->setColor(color);
}

const osg::Vec4& Marker::getTextColor()
{
	return osg::Vec4();
}

int Marker::getIntSize(int num)
{
	char temp[20];
//	sprintf_s(temp, "%d", num);
    sprintf(temp, "%d", num);
	std::string str(temp);
	return str.length();
}

void Marker::createText()
{
	//if (_text == NULL)
	//{
	//	_text = new EarthText();
	//	addChild(_text->getTextNode());
	//}
	//
	//if (_pos.isNaN())
	//{
	//	return;
	//}
	//else
	//{
	//	std::string strlat = BaseCommonMethod::ChangeValueToDagee(_pos.getLatitude().getDegrees(), true);
	//	std::string strlon = BaseCommonMethod::ChangeValueToDagee(_pos.getLongitude().getDegrees(), false);

	//	char ch[50];
	//	sprintf_s(ch,"%s:%s\n%s:%s","纬度",strlat.data(),"经度",strlon.data());
	//	
	//	std::string str = std::string(ch);
	//	str = encoding(LOCAL, UTF_8, str);
	//	_text->setTextString(str);
	//	getChild(1)->setNodeMask(true);
	//}
}


