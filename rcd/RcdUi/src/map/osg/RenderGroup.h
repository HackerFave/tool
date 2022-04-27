#ifndef _RENDERGROUP_H_
#define _RENDERGROUP_H_


#include <osg/PositionAttitudeTransform>
class Layer;
class LayerList;
class RenderGroup : public osg::PositionAttitudeTransform
{
public:
	RenderGroup();
	~RenderGroup();

	virtual bool addChild(osg::Node* node);

	virtual bool pick(int x, int y) { return false; };

	virtual void filterVertex(osg::Vec3d& centerPos) { }

	/*!
	*\fn		Layer* asLayer()
	*\brief		是否是图层
	*\return	Layer*
	*/
	virtual Layer* asLayer() { return NULL; }

	/*!
	*\fn		LayerList* asLayerList()
	*\brief		是否是图层组
	*\return	Layer*
	*/
	virtual LayerList* asLayerList() { return NULL; }

	/*!
	*\fn		virtual EarthRenderGroup* asEarthRenderGroup()
	*\brief		是否为EarthRenderGroup节点
	*\return	EarthRenderGroup*
	*/
	virtual RenderGroup* asRenderGroup() { return this; }
};
#endif