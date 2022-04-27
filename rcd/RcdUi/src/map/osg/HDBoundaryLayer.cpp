#include "HDBoundaryLayer.h"
#include "PolyInfo.h"
#include "GisModel.h"
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Point>
#include <osg/LineWidth>
#include <osg/BlendFunc>
#include <osg/PolygonOffset>
#include "HDPolygonManager.h"
#include <osgUtil/Tessellator>
#include "poly2tri/poly2tri.h"
#include "EngineProxy.h"
#include "HDLaneLayerList.h"
#include "HDLaneLayer.h"
#include "LaneInfo.h"
#include "HDRoadLayer.h"
#include "HDRoadLayerList.h"
#include "osgText/Text"
#include "QTextCodec"
#include <QSet>

using namespace std;


HDBoundaryLayer::HDBoundaryLayer()
{
	m_dPosArray = new osg::Vec3dArray();
	m_dVertexArray = new osg::Vec3dArray();
	m_needUpdate = false;
	m_layerHeight = -500;


	m_lineColor = osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f);
	m_polyColor = osg::Vec4(0.5f, 0.5f, 0.5f, 0.3f);


}

HDBoundaryLayer::~HDBoundaryLayer()
{
	HDPolygonManager::instance()->removePolyById(m_curPolyInfo->getType(), m_curPolyInfo->getID());
}

void HDBoundaryLayer::setPolyInfo(PolyInfo* polyInfo)
{
	m_curPolyInfo = polyInfo;
	m_dPosArray->clear();
	SetCurColor(m_curPolyInfo->getType());
	m_dPosArray = m_curPolyInfo->getPosArray();

	setName(m_curPolyInfo->getName().toStdString());
}

void HDBoundaryLayer::changeLatLonToUTM(GisModel* model)
{
	if (m_dPosArray->size() == 0)
	{
		return;
	}
	m_dVertexArray->push_back(model->convertLatLongHeightToWorldPos(m_dPosArray->at(0)));
	for (int i = 1; i < m_dPosArray->size(); i++)
	{
		osg::Vec3d pos = model->convertLatLongHeightToWorldPos(m_dPosArray->at(i));
		int index = m_dVertexArray->size() - 1;
		if (m_dVertexArray->at(index).x() == pos.x())
		{
			continue;
		}
		m_dVertexArray->push_back(pos);
	}

}

void HDBoundaryLayer::changeUTMToLatLon(GisModel* model)
{
	if (m_needUpdate)
	{
		m_dPosArray->clear();
		for (int i = 0; i < m_dVertexArray->size(); i++)
		{
			m_dPosArray->push_back(model->convertWorldPosToLatLongHeight(m_dVertexArray->at(i)));
		}
	}
}




void HDBoundaryLayer::drawBoundary()
{
	osg::Geode* geode = new osg::Geode();
	m_lineGeom = new osg::Geometry();

	osg::Vec3Array* drawArray = new osg::Vec3Array;
	for (int i = 0; i < m_dVertexArray->size(); i++)
	{
		drawArray->push_back(osg::Vec3(m_dVertexArray->at(i).x(), m_dVertexArray->at(i).y(), m_layerHeight));//m_layerHeight
	}
	//osg::Vec3dArray* arrays = new osg::Vec3dArray;
	//arrays for vertices
	//osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	//m_lineGeom->setVertexArray(vertexArray);
	//array  for colors
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(m_lineColor);
	m_lineGeom->setColorArray(colors.get());
	m_lineGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	m_lineGeom->setVertexArray(drawArray);


	m_lineGeom->setUseDisplayList(false);
	m_lineGeom->setUseVertexBufferObjects(true);
	osg::ref_ptr<osg::DrawArrays> lines = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, drawArray->size());
	m_lineGeom->addPrimitiveSet(lines.get());

	m_polygonGeom = new osg::Geometry();
	colors->clear();
	colors->push_back(m_polyColor);
	m_polygonGeom->setColorArray(colors.get());
	m_polygonGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	m_polygonGeom->setVertexArray(drawArray);

	m_polygonGeom->setUseDisplayList(false);
	m_polygonGeom->setUseVertexBufferObjects(true);

	generateBoundaryArray();
	//osg::ref_ptr<osg::DrawArrays> polys = new osg::DrawArrays(osg::PrimitiveSet::POLYGON, 0, drawArray->size());
	//m_polygonGeom->addPrimitiveSet(polys.get());


	osg::ref_ptr<osg::PolygonOffset> polyOffset = new osg::PolygonOffset(-0.1f, 0.2f);
	m_lineGeom->getOrCreateStateSet()->setAttributeAndModes(polyOffset, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);

	//osg::ref_ptr<osg::DrawArrays> polys = new osg::DrawArrays(osg::PrimitiveSet::POLYGON, 0, arrays->size());
	//m_polygonGeom->addPrimitiveSet(polys.get());
	m_polygonGeom->getOrCreateStateSet()->setAttributeAndModes(new osg::BlendFunc, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	geode->getOrCreateStateSet()->setAttribute(new osg::Point(2.0f), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	geode->getOrCreateStateSet()->setAttributeAndModes(new osg::LineWidth(2.0f), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	geode->addDrawable(m_lineGeom.get());
	geode->addDrawable(m_polygonGeom.get());
	geode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	osg::Vec3d pos = geode->getBound().center();
	pos.z() = m_layerHeight + 10;
	std::string timesFont("fonts/simkai.ttf");


	addChild(geode);

	if (m_curPolyInfo->getType() == 3 || m_curPolyInfo->getType() == 2)
	{
		osgText::Text* text = new osgText::Text;
		text->setCharacterSize(20.0);
		QString qstr = m_curPolyInfo->getName();
		text->setText(qstr.toStdString(), osgText::String::ENCODING_UTF8);
		text->setFont(timesFont);
		text->setAlignment(osgText::Text::CENTER_CENTER);
		osg::Geode* textgeode = new osg::Geode;
		m_text = new osg::PositionAttitudeTransform;
		m_text->setPosition(pos);
		textgeode->addDrawable(text);
		m_text->addChild(textgeode);
		addChild(m_text);
	}
	
}

void HDBoundaryLayer::updateColor(osg::Vec4& color)
{
	osg::Vec4Array* colorArray = (osg::Vec4Array*)m_lineGeom->getColorArray();
	colorArray->clear();
	colorArray->push_back(color);
	m_lineGeom->dirtyGLObjects();

	colorArray = (osg::Vec4Array*)m_polygonGeom->getColorArray();
	colorArray->clear();
	osg::Vec4 polyColor = color;
	polyColor.a() = 0.3f;
	colorArray->push_back(polyColor);
	m_polygonGeom->dirtyGLObjects();
}

void HDBoundaryLayer::resetColor()
{
	osg::Vec4Array* colorArray = (osg::Vec4Array*)m_lineGeom->getColorArray();
	colorArray->clear();
	colorArray->push_back(m_lineColor);
	m_lineGeom->dirtyGLObjects();

	colorArray = (osg::Vec4Array*)m_polygonGeom->getColorArray();
	colorArray->clear();
	colorArray->push_back(m_polyColor);
	m_polygonGeom->dirtyGLObjects();
}

int HDBoundaryLayer::getRecType()
{
	return m_curPolyInfo->getType();
}

int HDBoundaryLayer::getRecId()
{
	return m_curPolyInfo->getID();
}

void HDBoundaryLayer::setName(const std::string& name)
{
	QString str = QString::fromStdString(name);
	m_curPolyInfo->setName(str);
	Layer::setName(name);
}

HDRoadLayer* HDBoundaryLayer::getRelatedRoadLayer()
{
	int type = getRecType();
	if (type != 2 && type != 3)
	{
		return nullptr;
	}

	HDLaneLayerList* lanelist = EngineProxy::instance()->getLanelayerList();

	HDRoadLayerList* roadlist = EngineProxy::instance()->getRoadLayerList();

	QVector<HDLaneLayer*> list = lanelist->findLaneLayerByBoundaryIdAndType(getRecId(), type);

	QSet<HDRoadLayer*> roadSet;
	//²éÕÒRoadLayer
	int preid = -1, sucid = -1, preLaneid = -1, sucLaneid = -1;
	HDLaneLayer* prelayer = nullptr;
	HDLaneLayer* suclayer = nullptr;
	for (HDLaneLayer* layer : list)
	{
		preid = layer->getLaneInfo()->getPreNodeId();

		sucid = layer->getLaneInfo()->getSucNodeId();

		preLaneid = lanelist->findLaneIdBySucNodeid(preid);

		sucLaneid = lanelist->findLaneIdByPreNodeid(sucid);

		prelayer = lanelist->findLaneLayerById(preLaneid);
		if (prelayer && prelayer->getLaneInfo()->getElementType() == 0)
		{
			int roadID = prelayer->getLaneInfo()->getElementId();
			roadSet.insert(roadlist->getLayerById(roadID));
		}

		suclayer = lanelist->findLaneLayerById(sucLaneid);
		if (suclayer && suclayer->getLaneInfo()->getElementType() == 0)
		{
			int roadID = suclayer->getLaneInfo()->getElementId();
			roadSet.insert(roadlist->getLayerById(roadID));
		}


		preid = -1;
		sucid = -1;
		preLaneid = -1;
		sucLaneid = -1;

		prelayer = nullptr;
		suclayer = nullptr;
	}

	if (roadSet.size() == 1)
	{
		return *(roadSet.begin());
	}

	return nullptr;
}

template <class C> void FreeClear(C & cntr) {
	for (typename C::iterator it = cntr.begin();
		it != cntr.end(); ++it) {
		delete * it;
	}
	cntr.clear();
}

void HDBoundaryLayer::generateBoundaryArray()
{
	vector<p2t::Triangle*> triangles;
	vector<p2t::Point*> polyline;

	for (int i = 0; i < m_dVertexArray->size() - 2; i = i + 2)
	{
		if (m_dVertexArray->at(i).x() == m_dVertexArray->at(i + 1).x())
		{
			m_dVertexArray->at(i).x() = m_dVertexArray->at(i).x() + 0.00000000001;
			//continue;
		}
		polyline.push_back(new p2t::Point(m_dVertexArray->at(i).x(), m_dVertexArray->at(i).y()));
	}
	int length = m_dVertexArray->size();
	if (m_dVertexArray->at(0).x() != m_dVertexArray->at(length - 1).x())
	{
		polyline.push_back(new p2t::Point(m_dVertexArray->at(length - 1).x(), m_dVertexArray->at(length - 1).y()));
	}

	// 		vector<p2t::Point*>::iterator itr = polyline.begin();
	// 		vector<p2t::Point*>::iterator itr2 = polyline.end();
	// 		itr2--;
	// 		itr2--;
	// 		vector<p2t::Point*>::iterator tempitr;
	// 		while (itr != itr2)
	// 		{
	// 			if ((*itr)->x == (*(itr + 1))->x)
	// 			{
	// 
	// 				polyline.erase(itr);
	// 				itr = polyline.begin();
	// 				continue;
	// 			}
	// 			itr++;
	// 		}
	// 		for (; itr != itr2;)
	// 		{
	// 			if ((*itr)->x == (*(itr + 1))->x)
	// 			{
	// 
	// 				polyline.erase(itr);
	// 			}
	// 			else {
	// 				itr++;
	// 			}
	// 			
	// 		}
	// 		for (int i = 0; i < polyline.size()-2; i++)
	// 		{
	// 			if (polyline[i]->x == polyline[i+1]->x)
	// 			{
	// 				polyline.erase(i);
	// 			}
	// 		}
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
	m_polygonGeom->setVertexArray(triArray);
	osg::ref_ptr<osg::DrawArrays> polys = new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, triArray->size());
	m_polygonGeom->addPrimitiveSet(polys.get());
	//m_polygonGeom->dirtyBound();
	//m_polygonGeom->dirtyDisplayList();
	FreeClear(triangles);
	FreeClear(polyline);
}

void HDBoundaryLayer::SetCurColor(int type)
{

	if (type == 2)
	{
		m_polyColor = m_loadColor;
		m_lineColor = m_polyColor;
		m_lineColor.a() = 1.0;
	}
	else if (type == 3)
	{
		m_polyColor = m_unloadColor;
		m_lineColor = m_polyColor;
		m_lineColor.a() = 1.0;

	}
	else if (type == 4)
	{
		m_polyColor = m_parkColor;
		m_lineColor = m_polyColor;
		m_lineColor.a() = 1.0;

	}
	else if (type == 5)
	{
		m_polyColor = m_oilColor;
		m_lineColor = m_polyColor;
		m_lineColor.a() = 1.0;

	}
	else if (type == 6)
	{
		m_polyColor = m_maintenanceColor;
		m_lineColor = m_polyColor;
		m_lineColor.a() = 1.0;

	}
	else {
		m_polyColor = m_otherColor;
		m_lineColor = m_polyColor;
		m_lineColor.a() = 1.0;
	}
	if (m_lineGeom)
	{
		resetColor();
	}

}

void HDBoundaryLayer::setVertexArray(osg::Vec3dArray* vertexArray)
{
	m_dVertexArray->clear();
	osg::Vec3d adjustPos(0.0f, 0.0f, m_layerHeight);
	for (int i = 0; i < vertexArray->size(); i++)
	{
		m_dVertexArray->push_back(vertexArray->at(i));
		m_dVertexArray->at(i) += adjustPos;
	}
	m_lineGeom->removePrimitiveSet(0);
	m_polygonGeom->removePrimitiveSet(0);
	generateBoundaryArray();
	osg::Vec3Array* drawArray = new osg::Vec3Array;
	for (int i = 0; i < m_dVertexArray->size(); i++)
	{
		drawArray->push_back(osg::Vec3(m_dVertexArray->at(i).x(), m_dVertexArray->at(i).y(), m_layerHeight));//m_layerHeight
	}
	m_lineGeom->setVertexArray(drawArray);

	osg::ref_ptr<osg::DrawArrays> lines = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, drawArray->size());
	m_lineGeom->addPrimitiveSet(lines.get());
}

