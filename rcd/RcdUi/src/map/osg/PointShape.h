#ifndef GTScene_POINTSHAPE_H
#define GTScene_POINTSHAPE_H 1

#include <osg/Geometry>

class PointShape : public  osg::Geometry
{
public:

	PointShape();


	void setRadius(float radius);
	float getRadius() const;
	const osg::Vec3& getCenter()const;
	void setCenter(const osg::Vec3& val);
	const osg::Vec4& getColor() const;
	void setColor(const osg::Vec4& color);
protected:

	virtual ~PointShape();

	void initPointShape();
protected:
	osg::Vec3    _center;
	float     _radius;
	osg::Vec4 _color;
	bool _firstColor;
	bool _firstRadius;
	bool _firstCenter;
};

#endif