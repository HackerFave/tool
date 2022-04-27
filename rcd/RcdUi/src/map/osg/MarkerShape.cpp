#include "MarkerShape.h"
#include "PointShape.h"
#include <osgText/Text>
//#include <osgUtil/CullVisitor>



//class PointGeodeCallBack : public osg::NodeCallback
//{
//public:
//	PointGeodeCallBack(){}
//
//	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
//	{
//		osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
//		if (cv)
//		{
//			osg::CullStack::CullingStack &proj = cv->getProjectionCullingStack();
//
//			if (!proj.empty())
//			{
//				osg::CullingSet& set = proj.back();
//				osg::CullingSet::Mask cullinosgtMask = set.getCullingMask();
//
//				set.setCullingMask(cullinosgtMask & ~osg::CullSettings::SMALL_FEATURE_CULLING);
//				traverse(node, nv);
//				set.setCullingMask(cullinosgtMask);
//			}
//			else
//				traverse(node, nv);
//
//		}
//		else
//			traverse(node, nv);
//
//	}
//
//};

MarkerShape::MarkerShape(osg::Shape* shape)
{
	_shape = shape;

	std::string strType = shape->className();


	if (strType.compare("Sphere"))
	{
		_shapeType = MarkerShape::Sphere;
		_center = ((osg::Sphere*)(_shape.get()))->getCenter();
	} 
	else if (strType.compare("Box"))
    {
		_shapeType = MarkerShape::Box;
		_center = ((osg::Box*)(_shape.get()))->getCenter();
	}
	else if (strType.compare("Cylinder"))
	{
		_shapeType = MarkerShape::Cylinder;
		_center = ((osg::Cylinder*)(_shape.get()))->getCenter();
	}
	else if (strType.compare("Cone"))
	{
		_shapeType = MarkerShape::Cone;
		_center = ((osg::Cone*)(_shape.get()))->getCenter();
	}
	else if (strType.compare("Capsule"))
	{
		_shapeType = MarkerShape::Capsule;
		_center = ((osg::Capsule*)(_shape.get()))->getCenter();
	}
	else
	{
		_shape = NULL;
	}

	InitDrawable();
}

MarkerShape::MarkerShape(/*球*/ const osg::Vec3& center,float radius)
{
	_shapeType = MarkerShape::Point;
	//_shape = new osg::Sphere(center, radius);
	
	_center = center;
 	InitDrawable();

	
}

MarkerShape::MarkerShape(/*立方体*/ const osg::Vec3& center,const osg::Vec3& halfLengths)
{
	_shapeType = MarkerShape::Box;
	_shape = new osg::Box(center, halfLengths.x(), halfLengths.y(), halfLengths.z());

	_center = center;
	InitDrawable();
}

MarkerShape::MarkerShape(/*圆柱、圆锥、太空舱*/ ShapeType shapeType, const osg::Vec3& center,float radius, float height)
{
	_shapeType = shapeType;

	switch(_shapeType)
	{
		case MarkerShape::Point:
			{
				_center = center;
				_ratio = radius;
			}
		case MarkerShape::Sphere:
			{
				//_shape = NULL;
				//break;
			}
		case MarkerShape::Box:
			{
				_shape = NULL;
				break;
			}
		case MarkerShape::Cylinder:
			{
				_shape = new osg::Cylinder(center, radius, height);
				break;
			}
		case MarkerShape::Cone:
			{
				_shape = new osg::Cone(center, radius, height);
				break;
			}
		case MarkerShape::Capsule:
			{
				_shape = new osg::Capsule(center, radius, height);
				break;
			}
		default:
			{
				_shape = NULL;
				break;
			}	
	}

	InitDrawable();
}

bool MarkerShape::markerShapeSet(float radius)
{
	//if (_shapeType != MarkerShape::Sphere || _shapeType != MarkerShape::Point || !_shape)
	//{
	//	return false;
	//}
	if (_shapeType == MarkerShape::Sphere && _shape)
	{
		((osg::Sphere*)(_shape.get()))->set(_center, radius);
	}
	//else
	//{
	//	_pointShape->setRadius(radius);
	//}
	

	return true;
}

bool MarkerShape::markerShapeSet(const osg::Vec3& halfLengths)
{
	if (_shapeType != MarkerShape::Box || !_shape)
	{
		return false;
	}

	((osg::Box*)(_shape.get()))->set(_center, halfLengths);

	return true;
}

bool MarkerShape::markerShapeSet(float radius, float height)
{
	if (_shapeType != MarkerShape::Box || !_shape)
	{
		return false;
	}

	switch(_shapeType)
	{
		case MarkerShape::Sphere:
			{
				return false;
			}
		case MarkerShape::Box:
			{
				return false;
			}
		case MarkerShape::Cylinder:
			{
				((osg::Cylinder*)(_shape.get()))->set(_center, radius, height);
				break;
			}
		case MarkerShape::Cone:
			{
				((osg::Cone*)(_shape.get()))->set(_center, radius, height);
				break;
			}
		case MarkerShape::Capsule:
			{
				((osg::Capsule*)(_shape.get()))->set(_center, radius, height);
				break;
			}
		default:
			{
				return false;
			}	
	}

	return true;
}

void MarkerShape::InitDrawable()
{
	if (!_shape && _shapeType != Point)
	{
		return;
	}
	if (_shapeType == Point)
	{
		_pointShape = new PointShape();
		_pointShape->setColor(osg::Vec4(1,1,1,0.8));
		_ratio = 10.0;
		//_pointShape->setRadius(_ratio);
		_pointShape->setCenter(_center);

		addDrawable(_pointShape);
		//osgText::Text* text = new osgText::Text;
		//text->setText("adf");
		//text->setAxisAlignment(osgText::Text::SCREEN);
		//text->setPosition(osg::Vec3(0,0,0));
		//text->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f) );
		//osg::StateSet* PositonStateSet = new osg::StateSet();
		//text->setStateSet(PositonStateSet);
		//// 禁止深度测试，因此几何体将忽略已绘制物体的深度值，直接进行绘制
		//PositonStateSet->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
		//PositonStateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
		//PositonStateSet->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		//PositonStateSet->setRenderBinDetails(1111111112, "RenderBin");

		//addDrawable(text);

		//setCullCallback(new PointGeodeCallBack());
		this->setCullingActive(false);
	}
	else
	{
		_ratio = 0.5;
		osg::ref_ptr<osg::TessellationHints> hint = new osg::TessellationHints();
		hint->setDetailRatio(_ratio);//顶点数

		_shapeDrawable = new osg::ShapeDrawable(_shape,hint);
		_shapeDrawable->setColor(osg::Vec4(0,0,1,1));
		this->addDrawable(_shapeDrawable);

		//osgUtil::SmoothingVisitor::smooth(*(_shapeDrawable.get()->asGeometry()));
	}

}

const osg::Vec4& MarkerShape::getColor()
{
	if (_shapeType == MarkerShape::Point)
	{
		return _pointShape->getColor();
	}
	return _shapeDrawable->getColor();
}

void MarkerShape::setRatio(float ratio)
{ 
	_ratio = ratio;
	//if (_shapeType == MarkerShape::Point)
	//{
	//	_pointShape->setRadius(ratio);
	//	return;
	//}
	osg::ref_ptr<osg::TessellationHints> hint = new osg::TessellationHints();
	hint->setDetailRatio(_ratio);//顶点数

	_shapeDrawable->setTessellationHints(hint);
}

void MarkerShape::setColor( const osg::Vec4& color )
{
	if (_shapeType == MarkerShape::Point)
	{
		_pointShape->setColor(color);
	}
	else
	{
		getShapeDrawable()->setColor(color);
	}
}
