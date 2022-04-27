#ifndef _LAYER_H_
#define _LAYER_H_

#include <osg/Group>
#include "RenderGroup.h"
class HDLaneLayer;
class HDBoundaryLayer;
class HDRoadLayer;
class Layer : public RenderGroup
{
public:
	Layer();
	virtual ~Layer();

	virtual void updateColor(osg::Vec4& color) {};
	virtual void resetColor() {}
	virtual void addPostion(osg::Vec3d& pos) {}
	virtual const osg::Vec3d& getPATPostion() const { return osg::Vec3d(); }

	virtual Layer* asLayer() { return this; }
	virtual HDLaneLayer* asLaneLayer();
	virtual HDBoundaryLayer* asBoundaryLayer();
	virtual HDRoadLayer* asRoadLayer();

	virtual void setName(const std::string& name);

	void setSelected(bool state);
	bool getSelected();


protected:
	bool m_bSelected;
};

#endif