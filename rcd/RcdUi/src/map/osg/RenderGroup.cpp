#include "RenderGroup.h"

RenderGroup::RenderGroup()
{
}


RenderGroup::~RenderGroup()
{
}

bool RenderGroup::addChild(osg::Node* node)
{
	//
	if (osg::Group::addChild(node))
	{
		//此处增加监听,为后续树上的操作增加处理;
		return true;
	}
	return false;
}
