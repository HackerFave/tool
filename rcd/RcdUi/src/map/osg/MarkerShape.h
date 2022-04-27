#ifndef MARKERSHAPE_H
#define MARKERSHAPE_H 1

#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include "PointShape.h"


class MarkerShape : public osg::Geode
{
public:
	enum ShapeType
	{
		// ��
		Point,
		// ��
		Sphere,//set(const Vec3& center,float radius)
		// ������
		Box,//set(const Vec3& center,const Vec3& halfLengths)
		// Բ��
		Cylinder,//set(const Vec3& center,float radius, float height)
		// Բ׶
		Cone,//set(const Vec3& center,float radius, float height)
		// ̫�ղ�
		Capsule//set(const Vec3& center,float radius, float height)
	};

	MarkerShape(osg::Shape* shape); //��������ĳ�ʼ��
	MarkerShape(const osg::Vec3& center = osg::Vec3(0, 0, 0), float radius = 1.0f/*��*/);
	MarkerShape(const osg::Vec3& center, const osg::Vec3& halfLengths/*������*/);
	MarkerShape(ShapeType shapeType, const osg::Vec3& center, float radius, float height/*Բ����Բ׶��̫�ղ�*/);

public:
	bool markerShapeSet(float radius);
	bool markerShapeSet(const osg::Vec3& halfLengths);
	bool markerShapeSet(float radius, float height);

	osg::Shape* getShape() const { return _shape; }
	osg::ShapeDrawable* getShapeDrawable() const { return _shapeDrawable; }

	const osg::Vec4& getColor();
	void setColor(const osg::Vec4& color);

	void setRatio(float ratio);
	float getRatio() const { return _ratio; }

protected:
	~MarkerShape() {};

	void InitDrawable();



private:
	osg::ref_ptr<osg::Shape> _shape;
	osg::ref_ptr<osg::ShapeDrawable> _shapeDrawable;
	osg::ref_ptr<PointShape> _pointShape;

	ShapeType _shapeType;
	osg::Vec3 _center;
	float	  _ratio;
};

#endif