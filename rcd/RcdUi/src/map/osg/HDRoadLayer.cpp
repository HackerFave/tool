#include "HDRoadLayer.h"
#include "HDRoadManager.h"
#include "GisModel.h"
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Point>
#include <osg/LineWidth>
#include <osg/BlendFunc>
#include <osg/PolygonOffset>
#include "EngineProxy.h"
#include "poly2tri/poly2tri.h"

using namespace std;

HDRoadLayer::HDRoadLayer()
{
	m_dLeftArray = new osg::Vec3dArray();
	m_dRightArray = new osg::Vec3dArray();
	m_dVertexLeftArray = new osg::Vec3dArray();
	m_dVertexRightArray = new osg::Vec3dArray();
	m_layerHeight = -490;
	m_lineColor = osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f);
	m_polyColor = osg::Vec4(0.5f, 0.5f, 0.5f, 0.3f);

	m_pLineStrpple = new osg::LineStipple();
	m_pLineStrpple->setFactor(1);
	m_pLineStrpple->setPattern(0xff00);

	m_pLineWidth1 = new osg::LineWidth(6.0f);
	m_pLineWidth2 = new osg::LineWidth(6.0f);

	m_bLeftType = true;
	m_bRightType = true;
}

HDRoadLayer::~HDRoadLayer()
{
	HDRoadManager::instance()->removeRoadById(m_curRoadInfo->m_id);
}


void HDRoadLayer::setRoadInfo(RoadInfo* roadInfo)
{
	m_curRoadInfo = roadInfo;
	SetCurColor(m_curRoadInfo->m_nTwoSide);
	m_dLeftArray->clear();
	m_dRightArray->clear();
	m_dLeftArray = m_curRoadInfo->m_pLeftArray;
	m_dRightArray = m_curRoadInfo->m_pRightArray;
	setName(m_curRoadInfo->m_strName.toStdString());

}

void HDRoadLayer::changeLatLonToUTM(GisModel* model)
{
	for (int i = 0; i < m_dLeftArray->size(); i++)
	{
		m_dVertexLeftArray->push_back(model->convertLatLongHeightToWorldPos(m_dLeftArray->at(i)));
	}

	for (int i = 0; i < m_dRightArray->size(); i++)
	{
		m_dVertexRightArray->push_back(model->convertLatLongHeightToWorldPos(m_dRightArray->at(i)));
	}
}

//void HDRoadLayer::changeUTMToLatLon(GisModel* model)
//{

//}

void HDRoadLayer::drawRoad()
{
	osg::Geode* geode = new osg::Geode();
	m_lineGeom1 = new osg::Geometry();
	m_lineGeom2 = new osg::Geometry();
	osg::Vec3Array* drawArrayline1 = new osg::Vec3Array();
	osg::Vec3Array* drawArrayline2 = new osg::Vec3Array();

	int nleftLength = m_dVertexLeftArray->size();
	int nRightLength = m_dVertexRightArray->size();
	//drawArrayline1->push_back(osg::Vec3(m_dVertexLeftArray->at(0).x(), m_dVertexLeftArray->at(0).y(), m_layerHeight));
	for (int i = 0; i < nleftLength - 1; i++)
	{
		if (i % 50 == 0)
		{
			drawArrayline1->push_back(osg::Vec3(m_dVertexLeftArray->at(i).x(), m_dVertexLeftArray->at(i).y(), m_layerHeight));//m_layerHeight
		}
	}
	if (nleftLength > 50)
	{
		drawArrayline1->push_back(osg::Vec3(m_dVertexLeftArray->at(nleftLength - 1).x(), m_dVertexLeftArray->at(nleftLength - 1).y(), m_layerHeight));
	}

	int nDrawLine1Length = drawArrayline1->size();
	//int nDivisor = int((m_dVertexRightArray->size()) / (nDrawLine1Length - 2));
	//if ((m_dVertexRightArray->size()) % (nDrawLine1Length - 2) == 0)
	//{
	//	nDivisor = nDivisor - 1;
	//}
	for (int i = 0; i < nRightLength - 1; i++)
	{
		if (i % 50 == 0)
		{
			drawArrayline2->push_back(osg::Vec3(m_dVertexRightArray->at(i).x(), m_dVertexRightArray->at(i).y(), m_layerHeight));//m_layerHeight
		}
	}
	if (nRightLength)
	{
		drawArrayline2->push_back(osg::Vec3(m_dVertexRightArray->at(nRightLength - 1).x(), m_dVertexRightArray->at(nRightLength - 1).y(), m_layerHeight));//m_layerHeight
	}

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(m_lineColor);
	m_lineGeom1->setColorArray(colors.get());
	m_lineGeom1->setColorBinding(osg::Geometry::BIND_OVERALL);
	m_lineGeom1->setVertexArray(drawArrayline1);

	osg::ref_ptr<osg::Vec4Array> colors2 = new osg::Vec4Array;
	colors2->push_back(m_lineColor);
	m_lineGeom2->setColorArray(colors2.get());
	m_lineGeom2->setColorBinding(osg::Geometry::BIND_OVERALL);
	m_lineGeom2->setVertexArray(drawArrayline2);

	m_lineGeom1->setUseDisplayList(false);
	m_lineGeom1->setUseVertexBufferObjects(true);
	osg::ref_ptr<osg::DrawArrays> line1 = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, drawArrayline1->size());
	m_lineGeom1->addPrimitiveSet(line1.get());

	m_lineGeom2->setUseDisplayList(false);
	m_lineGeom2->setUseVertexBufferObjects(true);
	osg::ref_ptr<osg::DrawArrays> line2 = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, drawArrayline2->size());
	m_lineGeom2->addPrimitiveSet(line2.get());

	m_polygonGeom = new osg::Geometry();
	osg::ref_ptr<osg::Vec4Array> colors3 = new osg::Vec4Array;
	colors3->clear();
	colors3->push_back(m_polyColor);
	m_polygonGeom->setColorArray(colors3.get());
	m_polygonGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	//osg::Vec3Array* drawArray = new osg::Vec3Array();
	int size = drawArrayline1->size();
	int size2 = drawArrayline2->size();


	vector<p2t::Triangle*> triangles;
	vector<p2t::Point*> polyline;
	for (int i = 0; i < size; i++)
	{
		if (polyline.size() > 0)
		{
			if (drawArrayline1->at(i).x() == polyline.back()->x)
			{
				continue;
			}
		}
		polyline.push_back(new p2t::Point(drawArrayline1->at(i).x(), drawArrayline1->at(i).y()));
	}
	for (int i = size2 - 1; i >= 0; i--)
	{
		if (polyline.size() > 0)
		{
			if (drawArrayline2->at(i).x() == polyline.back()->x)
			{
				continue;
			}
		}
		polyline.push_back(new p2t::Point(drawArrayline2->at(i).x(), drawArrayline2->at(i).y()));
	}

	while (polyline[0]->x == polyline[polyline.size() - 1]->x)
	{
		polyline.pop_back();
	}

	p2t::CDT* cdt = new p2t::CDT(polyline);
	cdt->Triangulate();
	triangles = cdt->GetTriangles();

	//m_polygonGeom->setVertexArray(drawArray);

	osg::Vec3Array* triArray = new osg::Vec3Array;
	for (int i = 0; i < triangles.size(); i++)
	{
		p2t::Triangle& t = *triangles[i];
		p2t::Point& a = *t.GetPoint(0);
		p2t::Point& b = *t.GetPoint(1);
		p2t::Point& c = *t.GetPoint(2);
		triArray->push_back(osg::Vec3(a.x, a.y, m_layerHeight));
		triArray->push_back(osg::Vec3(b.x, b.y, m_layerHeight));
		triArray->push_back(osg::Vec3(c.x, c.y, m_layerHeight));
	}



	//for (int i = 0; i < drawArrayline1->size(); i++)
	//{
	//	drawArray->push_back(drawArrayline1->at(i));
	//	//if (size2 > size && i == size - 1)
	//	//{
	//	//	drawArray->push_back(drawArrayline2->at(size2 - 1));
	//	//}
	//	//else
	//	//	drawArray->push_back(drawArrayline2->at(i));
	//	
	//}
	//for (int i = drawArrayline2->size() - 1; i >= 0; i--)
	//{
	//	drawArray->push_back(drawArrayline2->at(i));
	//}
	m_polygonGeom->setVertexArray(triArray);

	m_polygonGeom->setUseDisplayList(false);
	m_polygonGeom->setUseVertexBufferObjects(true);

	osg::ref_ptr<osg::DrawArrays> polys = new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, triArray->size());
	m_polygonGeom->addPrimitiveSet(polys.get());

	//osg::ref_ptr<osg::PolygonOffset> polyOffset = new osg::PolygonOffset(-0.1f, 0.2f);
	//m_lineGeom1->getOrCreateStateSet()->setAttributeAndModes(polyOffset, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	//m_lineGeom2->getOrCreateStateSet()->setAttributeAndModes(polyOffset, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	m_polygonGeom->getOrCreateStateSet()->setAttributeAndModes(new osg::BlendFunc, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	geode->getOrCreateStateSet()->setAttribute(new osg::Point(2.0f), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	geode->getOrCreateStateSet()->setAttributeAndModes(new osg::LineWidth(2.0f), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	geode->addDrawable(m_lineGeom1.get());
	geode->addDrawable(m_lineGeom2.get());
	geode->addDrawable(m_polygonGeom.get());
	addChild(geode);

	updateBorder();
}

void HDRoadLayer::updateColor(osg::Vec4& color)
{
	osg::Vec4Array* colorArray = (osg::Vec4Array*)m_lineGeom1->getColorArray();
	colorArray->clear();
	colorArray->push_back(color);
	m_lineGeom1->dirtyGLObjects();
	colorArray = (osg::Vec4Array*)m_lineGeom2->getColorArray();
	colorArray->clear();
	colorArray->push_back(color);
	m_lineGeom1->dirtyGLObjects();

	colorArray = (osg::Vec4Array*)m_polygonGeom->getColorArray();
	colorArray->clear();
	osg::Vec4 polyColor = color;
	polyColor.a() = 0.3f;
	colorArray->push_back(polyColor);
	m_polygonGeom->dirtyGLObjects();
}

void HDRoadLayer::resetColor()
{
	osg::Vec4Array* colorArray = (osg::Vec4Array*)m_lineGeom1->getColorArray();
	colorArray->clear();
	colorArray->push_back(m_lineColor);
	m_lineGeom1->dirtyGLObjects();

	colorArray = (osg::Vec4Array*)m_lineGeom2->getColorArray();
	colorArray->clear();
	colorArray->push_back(m_lineColor);
	m_lineGeom2->dirtyGLObjects();

	colorArray = (osg::Vec4Array*)m_polygonGeom->getColorArray();
	colorArray->clear();
	colorArray->push_back(m_polyColor);
	m_polygonGeom->dirtyGLObjects();
}

void HDRoadLayer::updateLeftColor()
{
	resetGeometryColor();
	osg::Vec4Array* colorArray = (osg::Vec4Array*)m_lineGeom1->getColorArray();
	colorArray->clear();
	colorArray->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	m_lineGeom1->dirtyGLObjects();
	//m_lineGeom2->dirtyGLObjects();
}

void HDRoadLayer::resetGeometryColor()
{
	osg::Vec4Array* colorArray = (osg::Vec4Array*)m_lineGeom1->getColorArray();
	colorArray->clear();
	colorArray->push_back(m_lineColor);
	m_lineGeom1->dirtyGLObjects();

	colorArray = (osg::Vec4Array*)m_lineGeom2->getColorArray();
	colorArray->clear();
	colorArray->push_back(m_lineColor);
	m_lineGeom2->dirtyGLObjects();
}

void HDRoadLayer::updateRightColor()
{
	resetGeometryColor();
	osg::Vec4Array* colorArray = (osg::Vec4Array*)m_lineGeom2->getColorArray();
	colorArray->clear();
	colorArray->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	//m_lineGeom1->dirtyGLObjects();
	m_lineGeom2->dirtyGLObjects();
}

int HDRoadLayer::getLeftType()
{
	return m_curRoadInfo->m_leftType;
}

void HDRoadLayer::setLeftType(int type)
{
	m_curRoadInfo->m_leftType = type;


	if (type == 2)
	{
		m_pLineWidth1->setWidth(2.0f);
		m_lineGeom1->getOrCreateStateSet()->setAttributeAndModes(m_pLineStrpple, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
		m_lineGeom1->getStateSet()->setAttribute(m_pLineWidth1, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	}
	else if (type == 1)
	{
		if (m_bLeftType)
		{
			m_pLineWidth1->setWidth(2.0f);
			m_lineGeom1->getOrCreateStateSet()->setAttributeAndModes(m_pLineStrpple, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
			m_lineGeom1->getStateSet()->setAttribute(m_pLineWidth1, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
			m_bLeftType = false;
		}
		m_pLineWidth1->setWidth(6.0f);
		m_lineGeom1->getStateSet()->setAttribute(m_pLineWidth1, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
		m_lineGeom1->getOrCreateStateSet()->setAttributeAndModes(m_pLineStrpple, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	}
	else if (type == 0)
	{
		m_pLineWidth1->setWidth(2.0f);
		m_lineGeom1->getOrCreateStateSet()->setAttributeAndModes(m_pLineStrpple, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
		m_lineGeom1->getStateSet()->setAttribute(m_pLineWidth1, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	}



}

int HDRoadLayer::getRightType()
{
	return m_curRoadInfo->m_rightType;
}

void HDRoadLayer::setRightType(int type)
{

	m_curRoadInfo->m_rightType = type;

	if (type == 2)
	{
		m_pLineWidth2->setWidth(2.0f);
		m_lineGeom2->getOrCreateStateSet()->setAttributeAndModes(m_pLineStrpple, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
		m_lineGeom2->getStateSet()->setAttribute(m_pLineWidth2, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	}
	else if (type == 1)
	{
		if (m_bLeftType)
		{
			m_pLineWidth2->setWidth(2.0f);
			m_lineGeom2->getOrCreateStateSet()->setAttributeAndModes(m_pLineStrpple, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
			m_lineGeom2->getStateSet()->setAttribute(m_pLineWidth2, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
			m_bLeftType = false;
		}
		m_pLineWidth2->setWidth(6.0f);
		m_lineGeom2->getOrCreateStateSet()->setAttribute(m_pLineWidth2, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
		m_lineGeom2->getOrCreateStateSet()->setAttributeAndModes(m_pLineStrpple, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	}
	else if (type == 0)
	{
		m_pLineWidth2->setWidth(2.0f);
		m_lineGeom2->getOrCreateStateSet()->setAttributeAndModes(m_pLineStrpple, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
		m_lineGeom2->getStateSet()->setAttribute(m_pLineWidth2, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	}

}

void HDRoadLayer::setTwoSide(bool bTwoSide)
{
	m_curRoadInfo->m_nTwoSide = bTwoSide;
	SetCurColor(bTwoSide);

	resetColor();
}

bool HDRoadLayer::getTwoSide()
{
	return m_curRoadInfo->m_nTwoSide;
}

int HDRoadLayer::getElementID()
{
	return m_curRoadInfo->m_id;
}

osg::Geometry* HDRoadLayer::getLeftGeometry()
{
	return m_lineGeom1.get();
}

osg::Geometry* HDRoadLayer::getRightGeometry()
{
	return m_lineGeom2.get();
}

osg::Vec3dArray* HDRoadLayer::getLeftVertexArray()
{
	return m_dVertexLeftArray;
}

osg::Vec3dArray* HDRoadLayer::getRightVertexArray()
{
	return m_dVertexRightArray;
}

osg::Vec3dArray* HDRoadLayer::getLeftPosArray()
{
	return m_dLeftArray;
}

osg::Vec3dArray* HDRoadLayer::getRightPosArray()
{
	return m_dRightArray;
}

double HDRoadLayer::getLayerHeight()
{
	return m_layerHeight;
}

RoadInfo* HDRoadLayer::getRoadInfo()
{
	return m_curRoadInfo;
}

void HDRoadLayer::updatePosArrayByCut(osg::Vec3dArray* leftPosArray, osg::Vec3dArray* rightPosArray)
{
	m_dLeftArray = leftPosArray;
	m_dRightArray = rightPosArray;
	m_curRoadInfo->m_pLeftArray = leftPosArray;
	m_curRoadInfo->m_pRightArray = rightPosArray;
}

void HDRoadLayer::updateVertexArrayByCut(osg::Vec3dArray* newleftArray, osg::Vec3dArray* newRightArray)
{
	osg::Vec3Array* drawArrayline1 = new osg::Vec3Array();
	osg::Vec3Array* drawArrayline2 = new osg::Vec3Array();
	m_dVertexLeftArray->clear();
	m_dVertexRightArray->clear();
	m_dVertexLeftArray = newleftArray;
	m_dVertexRightArray = newRightArray;
	int nleftLength = m_dVertexLeftArray->size();
	int nRightLength = m_dVertexRightArray->size();

	//drawArrayline1->push_back(osg::Vec3(m_dVertexLeftArray->at(0).x(), m_dVertexLeftArray->at(0).y(), m_layerHeight));
	for (int i = 0; i < nleftLength - 1; i++)
	{
		if (i % 50 == 0)
		{
			drawArrayline1->push_back(osg::Vec3(m_dVertexLeftArray->at(i).x(), m_dVertexLeftArray->at(i).y(), m_layerHeight));//m_layerHeight
		}
	}
	drawArrayline1->push_back(osg::Vec3(m_dVertexLeftArray->at(nleftLength - 1).x(), m_dVertexLeftArray->at(nleftLength - 1).y(), m_layerHeight));

	//int nDrawLine1Length = drawArrayline1->size();
	//int nDivisor = int((m_dVertexRightArray->size()) / (nDrawLine1Length - 2));
	for (int i = 0; i < nRightLength - 1; i++)
	{
		if (i % 50 == 0)
		{
			drawArrayline2->push_back(osg::Vec3(m_dVertexRightArray->at(i).x(), m_dVertexRightArray->at(i).y(), m_layerHeight));//m_layerHeight
		}
	}
	drawArrayline2->push_back(osg::Vec3(m_dVertexRightArray->at(nRightLength - 1).x(), m_dVertexRightArray->at(nRightLength - 1).y(), m_layerHeight));//m_layerHeight

	m_lineGeom1->removePrimitiveSet(0, m_lineGeom1->getNumPrimitiveSets());
	m_lineGeom2->removePrimitiveSet(0, m_lineGeom2->getNumPrimitiveSets());

	m_lineGeom1->setVertexArray(drawArrayline1);
	m_lineGeom2->setVertexArray(drawArrayline2);
	m_lineGeom1->setUseDisplayList(false);
	m_lineGeom1->setUseVertexBufferObjects(true);
	osg::ref_ptr<osg::DrawArrays> line1 = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, drawArrayline1->size());
	m_lineGeom1->addPrimitiveSet(line1.get());

	m_lineGeom2->setUseDisplayList(false);
	m_lineGeom2->setUseVertexBufferObjects(true);
	osg::ref_ptr<osg::DrawArrays> line2 = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, drawArrayline2->size());
	m_lineGeom2->addPrimitiveSet(line2.get());


	m_polygonGeom->removePrimitiveSet(0, m_polygonGeom->getNumPrimitiveSets());
	vector<p2t::Triangle*> triangles;
	vector<p2t::Point*> polyline;
	int size = drawArrayline1->size();
	int size2 = drawArrayline2->size();
	for (int i = 0; i < size; i++)
	{
		polyline.push_back(new p2t::Point(drawArrayline1->at(i).x(), drawArrayline1->at(i).y()));
	}
	for (int i = size2 - 1; i >= 0; i--)
	{
		polyline.push_back(new p2t::Point(drawArrayline2->at(i).x(), drawArrayline2->at(i).y()));
	}

	while (polyline[0]->x == polyline[polyline.size() - 1]->x)
	{
		polyline.pop_back();
	}

	p2t::CDT* cdt = new p2t::CDT(polyline);
	cdt->Triangulate();
	triangles = cdt->GetTriangles();

	osg::Vec3Array* triArray = new osg::Vec3Array;
	for (int i = 0; i < triangles.size(); i++)
	{
		p2t::Triangle& t = *triangles[i];
		p2t::Point& a = *t.GetPoint(0);
		p2t::Point& b = *t.GetPoint(1);
		p2t::Point& c = *t.GetPoint(2);
		triArray->push_back(osg::Vec3(a.x, a.y, m_layerHeight));
		triArray->push_back(osg::Vec3(b.x, b.y, m_layerHeight));
		triArray->push_back(osg::Vec3(c.x, c.y, m_layerHeight));
	}

	m_polygonGeom->setVertexArray(triArray);

	m_polygonGeom->setUseDisplayList(false);
	m_polygonGeom->setUseVertexBufferObjects(true);

	osg::ref_ptr<osg::DrawArrays> polys = new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, triArray->size());
	m_polygonGeom->addPrimitiveSet(polys.get());


}

void HDRoadLayer::updateBorder()
{
	setLeftType(m_curRoadInfo->m_leftType);
	setRightType(m_curRoadInfo->m_rightType);
}

void HDRoadLayer::setName(const std::string& name)
{
	m_curRoadInfo->m_strName = QString::fromStdString(name);
	Layer::setName(name);
}

void HDRoadLayer::SetCurColor(bool bTwoSide)
{
	if (bTwoSide)
	{
		m_polyColor = m_doubleRoadColor;
		m_lineColor = m_polyColor;
		m_lineColor.a() = 1.0;
	}
	else
	{
		m_polyColor = m_singleRoadColor;
		m_lineColor = m_polyColor;
		m_lineColor.a() = 1.0;

	}
	if (m_polygonGeom)
	{
		resetColor();
	}
}

