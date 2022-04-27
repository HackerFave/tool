#include "RouteAndTaskLayer.h"
#include <osg/Point>
#include <osg/LineWidth>
#include <osgText/Text>
#include <osg/AutoTransform>

RouteAndTaskLayer::RouteAndTaskLayer()
{
	m_layerHeight = 570;
	float r = (rand() % 256) / 256.0;
	float g = (rand() % 256) / 256.0;
	float b = (rand() % 256) / 256.0;
	m_color = osg::Vec4(r, g, b, 0.6);
	r = (rand() % 256) / 256.0;
	g = (rand() % 256) / 256.0;
	b = (rand() % 256) / 256.0;
	m_RouteColor = osg::Vec4(r, g, b, 1.0);
	m_geometry = nullptr;
	m_geomRoute = nullptr;
	m_pVertexArray = nullptr;
	m_start = m_end = 0;
	m_pOriginArray = new osg::Vec3dArray();
}


RouteAndTaskLayer::~RouteAndTaskLayer()
{
}

void RouteAndTaskLayer::DrawTaskLane(osg::Vec3dArray* listInfo)
{
	m_pOriginArray->clear();
	for (int i = 0; i < listInfo->size(); i++)
	{
		m_pOriginArray->push_back(listInfo->at(i));
	}

	m_pVertexArray = new osg::Vec3Array();

	UpdateTaskVertexArray(listInfo);

	osg::Geode* geode = new osg::Geode();
	m_geometry = new osg::Geometry();

	osg::Vec3Array* drawArray = new osg::Vec3Array;
	for (int i = 0; i < m_pVertexArray->size(); i++)
	{
		drawArray->push_back(m_pVertexArray->at(i));
	}

	m_geometry->setVertexArray(drawArray);
	//array  for colors
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(m_color);
	m_geometry->setColorArray(colors.get());
	m_geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	m_geometry->setUseDisplayList(false);
	m_geometry->setUseVertexBufferObjects(true);



	osg::DrawArrays* line = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, drawArray->size());
	m_geometry->addPrimitiveSet(line);

	m_geometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	m_geometry->getOrCreateStateSet()->setAttribute(new osg::Point(2.0f), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	m_geometry->getOrCreateStateSet()->setAttributeAndModes(new osg::LineWidth(4.0f), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	geode->addDrawable(m_geometry.get());

	this->addChild(geode);

	m_startMarker = new Marker(MarkerShape::Point);
	m_startMarker->setPosition(m_pVertexArray->at(0));
	m_startMarker->setColor(m_color);
	this->addChild(m_startMarker);

	m_endMarker = new Marker(MarkerShape::Point);
	m_endMarker->setPosition(m_pVertexArray->at(m_pVertexArray->size() - 1));
	m_endMarker->setColor(m_color);
	this->addChild(m_endMarker);



	std::string timesFont("fonts/arial.ttf");

	osgText::Text* text = new osgText::Text;
	text->setCharacterSize(25.0);
	text->setText("lakjsdfljasdlf");
	text->setFont(timesFont);
	text->setAlignment(osgText::Text::CENTER_CENTER);
	osg::Geode* textgeode = new osg::Geode;
	m_text = new osg::PositionAttitudeTransform;
	m_text->setPosition(m_pVertexArray->at(0));
	textgeode->addDrawable(text);
	m_text->addChild(textgeode);

	this->addChild(m_text);

}

void RouteAndTaskLayer::UpdateRouteLane(int start, int end)
{
	if (!m_geomRoute.valid())
	{

		m_pRouteArray = new osg::Vec3Array();

		osg::Geode* geode = new osg::Geode();
		m_geomRoute = new osg::Geometry();
		geode->addDrawable(m_geomRoute.get());
		this->addChild(geode);
	}
	else
	{
		return redrawRouteLine(start, end);
	}
	updateRouteArray(start, end);

	osg::Vec3Array* drawArray = new osg::Vec3Array;
	for (int i = 0; i < m_pRouteArray->size(); i++)
	{
		drawArray->push_back(m_pRouteArray->at(i));
	}
	m_geomRoute->setVertexArray(m_pRouteArray);
	//array  for colors
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(m_RouteColor);
	m_geomRoute->setColorArray(colors.get());
	m_geomRoute->setColorBinding(osg::Geometry::BIND_OVERALL);

	m_geomRoute->setUseDisplayList(false);
	m_geomRoute->setUseVertexBufferObjects(true);



	osg::DrawArrays* line = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, m_pRouteArray->size());
	m_geomRoute->addPrimitiveSet(line);

	m_geomRoute->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	m_geomRoute->getOrCreateStateSet()->setAttribute(new osg::Point(2.0f), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	m_geomRoute->getOrCreateStateSet()->setAttributeAndModes(new osg::LineWidth(2.0f), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);

}

void RouteAndTaskLayer::updateRouteArray(int start, int end)
{
	m_pRouteArray->clear();
	osg::Vec3 pos;
	int startindex = start;
	int endindex = end;

	for (int i = startindex; i <= endindex; i++)
	{
		if (i % 5 == 0 || i == m_pOriginArray->size() - 1)
		{
			pos = m_pOriginArray->at(i);
			pos.z() = m_routeHeight;
			m_pRouteArray->push_back(pos);
		}
	}
}

void RouteAndTaskLayer::redrawRouteLine(int start, int end)
{

	if (m_geomRoute)
	{
		updateRouteArray(start, end);
		osg::Vec3Array* drawArray = new osg::Vec3Array;
		for (int i = 0; i < m_pRouteArray->size(); i++)
		{
			drawArray->push_back(m_pRouteArray->at(i));
		}


		m_geomRoute->removePrimitiveSet(0, m_geomRoute->getNumPrimitiveSets());


		m_geomRoute->setVertexArray(drawArray);

		osg::DrawArrays* lines = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, drawArray->size());
		m_geomRoute->addPrimitiveSet(lines);
		m_geomRoute->dirtyGLObjects();
		m_geomRoute->dirtyDisplayList();
	}

}

void RouteAndTaskLayer::UpdateTaskVertexArray(osg::Vec3dArray* listInfo)
{
	m_pVertexArray->clear();
	osg::Vec3 pos;

	m_pOriginArray->clear();


	for (int i = 0; i <= listInfo->size(); i++)
	{
		m_pOriginArray->push_back(listInfo->at(i));
		if (i % 5 == 0 || i == listInfo->size() - 1)
		{
			pos = listInfo->at(i);
			pos.z() = m_layerHeight;
			m_pVertexArray->push_back(pos);
		}
	}
}

void RouteAndTaskLayer::RedrawTaskLane()
{
	if (m_geometry)
	{
		osg::Vec3Array* drawArray = new osg::Vec3Array;
		for (int i = 0; i < m_pVertexArray->size(); i++)
		{
			drawArray->push_back(m_pVertexArray->at(i));
		}

		
		m_geometry->removePrimitiveSet(0, m_geometry->getNumPrimitiveSets());
		

		if (drawArray->size() <= 0)
		{
			return;
		}
		else if (drawArray->size() == 1)
		{
			m_startMarker->setPosition(m_pVertexArray->at(0));
			m_endMarker->setPosition(m_pVertexArray->at(0));

		}
		else
		{
			m_startMarker->setPosition(m_pVertexArray->at(0));
			m_endMarker->setPosition(m_pVertexArray->at(m_pVertexArray->size() - 1));
			m_text->setPosition(m_pVertexArray->at(0));
			m_geometry->setVertexArray(drawArray);

			osg::DrawArrays* lines = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, drawArray->size());
			m_geometry->addPrimitiveSet(lines);
			m_geometry->dirtyGLObjects();
			m_geometry->dirtyDisplayList();


		}


	}


}
