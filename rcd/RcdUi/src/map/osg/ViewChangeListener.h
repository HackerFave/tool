#ifndef _VIEWCHANGELISTENER_H_
#define _VIEWCHANGELISTENER_H_


#include <osg/Vec4d>
#include <osg/Vec3d>
#include <osg/Vec2i>
struct ViewChangeEvent
{
	osg::Vec4d sector;
	osg::Vec3d eyePos;
	osg::Vec2i view;
	double scaleRadio;
};

class ViewChangeListener
{
public:
	virtual bool viewChanged(const ViewChangeEvent& event)
	{
		return true;
	}

	virtual ~ViewChangeListener() {}

};


#endif