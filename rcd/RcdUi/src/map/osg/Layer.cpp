#include "Layer.h"



Layer::Layer()
{
	m_bSelected = false;
}


Layer::~Layer()
{
}


HDLaneLayer* Layer::asLaneLayer()
{
	return nullptr;
}

HDBoundaryLayer* Layer::asBoundaryLayer()
{
	return nullptr;
}

HDRoadLayer* Layer::asRoadLayer()
{
	return nullptr;
}

void Layer::setName(const std::string& name)
{
	RenderGroup::setName(name);
}

void Layer::setSelected(bool state)
{
	m_bSelected = state;
}

bool Layer::getSelected()
{
	return m_bSelected;
}

