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
		//�˴����Ӽ���,Ϊ�������ϵĲ������Ӵ���;
		return true;
	}
	return false;
}
