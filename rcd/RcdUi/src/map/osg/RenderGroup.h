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
	*\brief		�Ƿ���ͼ��
	*\return	Layer*
	*/
	virtual Layer* asLayer() { return NULL; }

	/*!
	*\fn		LayerList* asLayerList()
	*\brief		�Ƿ���ͼ����
	*\return	Layer*
	*/
	virtual LayerList* asLayerList() { return NULL; }

	/*!
	*\fn		virtual EarthRenderGroup* asEarthRenderGroup()
	*\brief		�Ƿ�ΪEarthRenderGroup�ڵ�
	*\return	EarthRenderGroup*
	*/
	virtual RenderGroup* asRenderGroup() { return this; }
};
#endif