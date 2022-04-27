#include "HDLaneLayer.h"
#include "LaneInfo.h"
#include "GisModel.h"
#include <osg/Geode>
#include <osg/Point>
#include <osg/LineWidth>
#include "LaneInfoManager.h"
#include <osg/LineSegment>
#include <osg/TemplatePrimitiveFunctor>
#include "CalcInterPoint.h"
#include "EngineProxy.h"
#include "LayerList.h"





HDLaneLayer::HDLaneLayer()
{
	m_dPosArray = new osg::Vec3dArray;
	m_dVertexArray = new osg::Vec3dArray;
	m_dVertexJoinedArray = new osg::Vec3dArray;
	m_needUpdate = false;
	m_layerHeight = -400;

}


HDLaneLayer::~HDLaneLayer()
{

	LaneInfoManager::instance()->removeLaneById(m_curLaneInfo->getLaneId());
}

void HDLaneLayer::setLaneInfo(LaneInfo* laneInfo)
{
	m_curLaneInfo = laneInfo;
	setName(m_curLaneInfo->GetLaneName().toStdString());
	QList<LaneTrailInfo> list = m_curLaneInfo->getLaneList();
	m_dPosArray->clear();
	for (int i = 0; i < list.size(); i++)
	{
		m_dPosArray->push_back(list[i].getLonLatHeight());
	}
}

LaneInfo* HDLaneLayer::getLaneInfo()
{
	return m_curLaneInfo;
}

void HDLaneLayer::getElementInfo(int& type, int& id)
{
	type = m_curLaneInfo->getElementType();
	id = m_curLaneInfo->getElementId();
}

void HDLaneLayer::setPosArray(osg::Vec3dArray* list)
{
	m_dPosArray = list;
}

void HDLaneLayer::updateJoinedLaneInfo(HDLaneLayer* pickLayer)
{
	int type = m_curLaneInfo->GetJoinedType();
	//前拼接
	int newId;
	int myId;
	if (type == 1)
	{
		newId = pickLayer->getLaneInfo()->getPreNodeId();
		myId = m_curLaneInfo->getPreNodeId();
		m_curLaneInfo->setPreNodeId(newId);
	}
	else if (type == 2)
	{
		newId = pickLayer->getLaneInfo()->getSucNodeId();
		myId = m_curLaneInfo->getSucNodeId();
		m_curLaneInfo->setSucNodeId(newId);
	}
	//m_curLaneInfo->setPreNodeId(newId);
	m_curLaneInfo->SetJoinedLaneid(pickLayer->getLaneInfo()->getLaneId());
	GisModel* model = EngineProxy::instance()->getGlobe();
	osg::Vec3d pos = model->convertWorldPosToLatLongHeight(m_interPoint);
	m_curLaneInfo->SetJoinedPointLat(pos.y());
	m_curLaneInfo->SetJoinedPointLon(pos.x());
}

void HDLaneLayer::drawLaneInterval(osg::Vec3Array* drawArray)
{
	osg::Geode* geode = new osg::Geode();
	m_geometry = new osg::Geometry();



	osg::Vec3d prePos;

	GisModel* model = EngineProxy::instance()->getGlobe();


	int prim1Size = drawArray->size();
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));

	m_geometry->setColorArray(colors.get());
	m_geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	m_geometry->setUseDisplayList(false);
	m_geometry->setUseVertexBufferObjects(true);



	osg::ref_ptr<osg::DrawElementsUShort> prim1 = new osg::DrawElementsUShort(GL_LINE_STRIP);
	for (int i = 0; i < prim1Size; i++)
	{
		prim1->push_back(i);
	}



	double sin45 = sin(osg::PI_4);
	double cos45 = cos(osg::PI_4);
	double sinneg45 = -sin45;
	double cosneg45 = cos45;
	osg::Vec3d start, end, norm, pos, left, right;
	osg::Matrix3d matrix1, matrix2;
	for (int i = 1; i < prim1Size; i++)
	{
		if (i % 50 != 0)
			continue;
		end = drawArray->at(i);
		start = drawArray->at(i - 1);
		norm = start - end;
		norm.normalize();
		pos = end + norm * 5; // 设置宽
		pos.z() = 1.0;
		matrix1 = osg::Matrix3d(cos45, -sin45, (1 - cos45) * start.x() + sin45 * start.y(), sin45, cos45, (1 - cos45) * start.y() - sin45 * start.x(), 0, 0, 1);
		matrix2 = osg::Matrix3d(cosneg45, -sinneg45, (1 - cosneg45) * start.x() + sinneg45 * start.y(), sinneg45, cosneg45, (1 - cosneg45) * start.y() - sinneg45 * start.x(), 0, 0, 1);
		left = osg::Vec3d(matrix1[0] * pos.x() + matrix1[1] * pos.y() + matrix1[2],
			matrix1[3] * pos.x() + matrix1[4] * pos.y() + matrix1[5], m_layerHeight);

		right = osg::Vec3d(matrix2[0] * pos.x() + matrix2[1] * pos.y() + matrix2[2],
			matrix2[3] * pos.x() + matrix2[4] * pos.y() + matrix2[5], m_layerHeight);

		drawArray->push_back(left);
		drawArray->push_back(end);
		drawArray->push_back(right);

	}

	osg::ref_ptr<osg::DrawElementsUShort> prim2 = new osg::DrawElementsUShort(GL_TRIANGLES);
	for (int i = prim1Size; i < drawArray->size(); i++)
	{
		prim2->push_back(i);
	}



	double latitude = 0.0;
	double longitude = 0.0;
	double alt = 0.0;
	float head = 0.0;
	float distance = 6.0;

	auto calcFunc = [&]()
	{
		float slope;  //直线的斜率
		float dx = 0.0, dy = 0.0;

		while (head < 0.0)
		{
			head += 360.0;
		}
		while (head > 360.0)
		{
			head -= 360.0;
		}

		if (head >= 2.0 && head <= 178.0) {
			slope = tan((90 - head) / 180 * 3.141592);
			dx = -1 * distance / sqrt(1 + slope * slope);
			dy = dx * slope;
		}
		else if (head >= 182.0 && head <= 358.0) {
			slope = tan((90 - head) / 180 * 3.141592);
			dx = distance / sqrt(1 + slope * slope);
			dy = dx * slope;
		}
		else if (head > 358.0 || head < 2.0) {
			dx = 0;
			dy = -1 * distance;
		}
		else if (head > 178.0 && head < 182.0) {
			dx = 0;
			dy = distance;
		}
		osg::Vec3d pos;
		pos.x() = dx / (111700 * cos(latitude / 180 * 3.141592)) + longitude;
		pos.y() = dy / 111700 + latitude;
		pos.z() = 0.0;
		drawArray->push_back(model->convertLatLongHeightToWorldPos(pos));

	};

	int newSize = drawArray->size();

	osg::ref_ptr<osg::DrawElementsUShort> prim3 = new osg::DrawElementsUShort(GL_LINE_STRIP);

	QList<LaneTrailInfo> lanelist = m_curLaneInfo->getLaneList();
	for (int i = 0; i < lanelist.size(); i++)
	{
		longitude = lanelist.at(i).Lon;
		latitude = lanelist.at(i).Lat;
		head = lanelist.at(i).Heading;
		head -= 90.0;
		alt = lanelist.at(i).Alt;
		calcFunc();
	}

	for (int i = newSize; i < drawArray->size(); i++)
	{
		prim3->push_back(i);
	}

	int newSize2 = drawArray->size();

	osg::ref_ptr<osg::DrawElementsUShort> prim4 = new osg::DrawElementsUShort(GL_LINE_STRIP);

	for (int i = 0; i < lanelist.size(); i++)
	{
		longitude = lanelist.at(i).Lon;
		latitude = lanelist.at(i).Lat;
		head = lanelist.at(i).Heading;
		head += 90.0;
		alt = lanelist.at(i).Alt;
		calcFunc();
	}

	for (int i = newSize2; i < drawArray->size(); i++)
	{
		prim4->push_back(i);
	}


	m_geometry->setVertexArray(drawArray);
	m_geometry->addPrimitiveSet(prim1);

	m_geometry->addPrimitiveSet(prim2);


	// 		m_geometry->addPrimitiveSet(prim3);
	// 
	// 		m_geometry->addPrimitiveSet(prim4);


	m_geometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	m_geometry->getOrCreateStateSet()->setAttribute(new osg::Point(2.0f), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	m_geometry->getOrCreateStateSet()->setAttributeAndModes(new osg::LineWidth(2.0f), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	geode->addDrawable(m_geometry.get());
	addChild(geode);
}

void HDLaneLayer::changeLatLonToUTM()
{
	GisModel* model = EngineProxy::instance()->getGlobe();
	m_dVertexArray->clear();
	for (int i = 0; i < m_dPosArray->size(); i++)
	{
		m_dVertexArray->push_back(model->convertLatLongHeightToWorldPos(m_dPosArray->at(i)));
	}
}

void HDLaneLayer::changeUTMToLatLon()
{
	GisModel* model = EngineProxy::instance()->getGlobe();
	if (m_needUpdate)
	{
		m_dPosArray->clear();
		for (int i = 0; i < m_dVertexArray->size(); i++)
		{
			m_dPosArray->push_back(model->convertWorldPosToLatLongHeight(m_dVertexArray->at(i)));
		}
	}
}

void HDLaneLayer::drawLane()
{
	osg::Vec3Array* drawArray = new osg::Vec3Array;
	GisModel* model = EngineProxy::instance()->getGlobe();
	osg::ref_ptr<osg::Vec3dArray> lengthArray = new osg::Vec3dArray;
	for (int i = 0; i < m_dVertexArray->size(); i++)
	{
		drawArray->push_back(osg::Vec3(m_dVertexArray->at(i).x(), m_dVertexArray->at(i).y(), m_layerHeight));//m_layerHeight
		osg::Vec3d point = model->computeWGSPosFromPosition(m_dPosArray->at(i));
		point.z() = 0.0f;
		lengthArray->push_back(point);
	}
	double length = 0.0;
	for (int i = 1; i < lengthArray->size(); i++)
	{
		length += (lengthArray->at(i) - lengthArray->at(i - 1)).length();
	}
	m_curLaneInfo->SetLength(length);

	drawLaneInterval(drawArray);

}

void HDLaneLayer::drawJoinedLane(HDLaneLayer* pickLayer)
{
	osg::Vec3Array* drawArray = new osg::Vec3Array();
	int type = m_curLaneInfo->GetJoinedType();
	QList<LaneTrailInfo> curInfolist;
	QList<LaneTrailInfo> nowList = m_curLaneInfo->getLaneList();
	osg::ref_ptr<osg::Vec3dArray> lengthArray = new osg::Vec3dArray;
	GisModel* model = EngineProxy::instance()->getGlobe();
	osg::Vec3d point;
	if (type == 1)
	{
		for (int i = 0; i < m_index; i++)
		{
			m_dVertexJoinedArray->push_back(pickLayer->m_dVertexArray->at(i));
			drawArray->push_back(osg::Vec3(pickLayer->m_dVertexArray->at(i).x(), pickLayer->m_dVertexArray->at(i).y(), m_layerHeight));
			point = model->computeWGSPosFromPosition(pickLayer->m_dPosArray->at(i));
			point.z() = 0.0f;
			lengthArray->push_back(point);
		}

		drawArray->push_back(osg::Vec3(m_interPoint.x(), m_interPoint.y(), m_layerHeight));

		for (int i = m_joinIndex + 1; i < m_dVertexArray->size(); i++)
		{
			m_dVertexJoinedArray->push_back(m_dVertexArray->at(i));
			drawArray->push_back(osg::Vec3(m_dVertexArray->at(i).x(), m_dVertexArray->at(i).y(), m_layerHeight));

			curInfolist.push_back(nowList.at(i));

			point = model->computeWGSPosFromPosition(m_dPosArray->at(i));
			point.z() = 0.0f;
			lengthArray->push_back(point);

		}


	}
	else if (type == 2)
	{
		for (int i = 0; i < m_joinIndex; i++)
		{
			m_dVertexJoinedArray->push_back(m_dVertexArray->at(i));
			drawArray->push_back(osg::Vec3(m_dVertexArray->at(i).x(), m_dVertexArray->at(i).y(), m_layerHeight));
			curInfolist.push_back(nowList.at(i));
			point = model->computeWGSPosFromPosition(m_dPosArray->at(i));
			point.z() = 0.0f;
			lengthArray->push_back(point);
		}
		drawArray->push_back(osg::Vec3(m_interPoint.x(), m_interPoint.y(), m_layerHeight));
		for (int i = m_index; i < pickLayer->m_dVertexArray->size(); i++)
		{
			m_dVertexJoinedArray->push_back(pickLayer->m_dVertexArray->at(i));
			drawArray->push_back(osg::Vec3(pickLayer->m_dVertexArray->at(i).x(), pickLayer->m_dVertexArray->at(i).y(), m_layerHeight + 1));
			point = model->computeWGSPosFromPosition(pickLayer->m_dPosArray->at(i));
			point.z() = 0.0f;
			lengthArray->push_back(point);
		}
	}
	if (curInfolist.size() > 0)
	{
		m_curLaneInfo->setLaneList(curInfolist);
	}

	double length = 0.0;
	for (int i = 1; i < lengthArray->size(); i++)
	{
		length += (lengthArray->at(i) - lengthArray->at(i - 1)).length();
	}
	m_curLaneInfo->SetLength(length);

	drawLaneInterval(drawArray);
}

void HDLaneLayer::drawJoinedLaneInit(HDLaneLayer* pickLayer)
{
	osg::Vec3Array* drawArray = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec3dArray> lengthArray = new osg::Vec3dArray;
	GisModel* model = EngineProxy::instance()->getGlobe();
	osg::Vec3d point;
	int type = m_curLaneInfo->GetJoinedType();
	if (type == 1)
	{
		for (int i = 0; i < m_index; i++)
		{
			m_dVertexArray->push_back(pickLayer->m_dVertexArray->at(i));
			drawArray->push_back(osg::Vec3(pickLayer->m_dVertexArray->at(i).x(), pickLayer->m_dVertexArray->at(i).y(), m_layerHeight));
			point = model->computeWGSPosFromPosition(pickLayer->m_dPosArray->at(i));
			point.z() = 0.0f;
			lengthArray->push_back(point);
		}
		drawArray->push_back(osg::Vec3(m_interPoint.x(), m_interPoint.y(), m_layerHeight));

		for (int i = 0; i < m_dVertexArray->size(); i++)
		{
			m_dVertexArray->push_back(m_dVertexArray->at(i));
			drawArray->push_back(osg::Vec3(m_dVertexArray->at(i).x(), m_dVertexArray->at(i).y(), m_layerHeight));
			point = model->computeWGSPosFromPosition(m_dPosArray->at(i));
			point.z() = 0.0f;
			lengthArray->push_back(point);
		}


	}
	else if (type == 2)
	{
		for (int i = 0; i < m_dVertexArray->size(); i++)
		{
			m_dVertexArray->push_back(m_dVertexArray->at(i));
			drawArray->push_back(osg::Vec3(m_dVertexArray->at(i).x(), m_dVertexArray->at(i).y(), m_layerHeight));
			point = model->computeWGSPosFromPosition(m_dPosArray->at(i));
			point.z() = 0.0f;
			lengthArray->push_back(point);
		}
		drawArray->push_back(osg::Vec3(m_interPoint.x(), m_interPoint.y(), m_layerHeight));
		for (int i = m_index; i < pickLayer->m_dVertexArray->size(); i++)
		{
			m_dVertexArray->push_back(pickLayer->m_dVertexArray->at(i));
			drawArray->push_back(osg::Vec3(pickLayer->m_dVertexArray->at(i).x(), pickLayer->m_dVertexArray->at(i).y(), m_layerHeight));
			point = model->computeWGSPosFromPosition(pickLayer->m_dPosArray->at(i));
			point.z() = 0.0f;
			lengthArray->push_back(point);
		}
	}
	double length = 0.0;
	for (int i = 1; i < lengthArray->size(); i++)
	{
		length += (lengthArray->at(i) - lengthArray->at(i - 1)).length();
	}
	m_curLaneInfo->SetLength(length);

	drawLaneInterval(drawArray);
}

void HDLaneLayer::updateColor(osg::Vec4& color)
{
	setPosition(osg::Vec3d(0.0f, 0.0f, 10.0f));
	osg::Vec4Array* colorArray = (osg::Vec4Array*)m_geometry->getColorArray();
	colorArray->clear();
	colorArray->push_back(color);
	m_geometry->dirtyGLObjects();

}

void HDLaneLayer::resetColor()
{
	setPosition(osg::Vec3d(0.0f, 0.0f, 0.0f));
	osg::Vec4Array* colorArray = (osg::Vec4Array*)m_geometry->getColorArray();
	colorArray->clear();
	colorArray->push_back(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
	m_geometry->dirtyGLObjects();
}

osg::Vec3dArray* HDLaneLayer::getVertexArray()
{
	if (m_curLaneInfo && m_curLaneInfo->GetJoinedType() != 0)
	{
		return m_dVertexJoinedArray;
	}
	return m_dVertexArray;
}

void HDLaneLayer::updateVertexArrayByCut(osg::Vec3dArray* array)
{
	m_geometry->removePrimitiveSet(0, m_geometry->getNumPrimitiveSets());
	m_dVertexArray = array;
	m_needUpdate = true;
	changeUTMToLatLon();
	osg::Vec3Array* drawArray = new osg::Vec3Array;

	osg::ref_ptr<osg::Vec3Array> lengthArray = new osg::Vec3Array;
	GisModel* model = EngineProxy::instance()->getGlobe();
	for (int i = 0; i < m_dVertexArray->size(); i++)
	{
		drawArray->push_back(osg::Vec3(m_dVertexArray->at(i).x(), m_dVertexArray->at(i).y(), m_layerHeight));//m_layerHeight
		osg::Vec3d point = model->computeWGSPosFromPosition(m_dPosArray->at(i));
		point.z() = 0.0f;
		lengthArray->push_back(point);
	}
	double length = 0.0;
	for (int i = 1; i < lengthArray->size(); i++)
	{
		length += (lengthArray->at(i) - lengthArray->at(i - 1)).length();
	}
	m_curLaneInfo->SetLength(length);

	int prim1Size = drawArray->size();

	osg::ref_ptr<osg::DrawElementsUShort> prim1 = new osg::DrawElementsUShort(GL_LINE_STRIP);
	for (int i = 0; i < prim1Size; i++)
	{
		prim1->push_back(i);
	}

	double sin45 = sin(osg::PI_4);
	double cos45 = cos(osg::PI_4);
	double sinneg45 = -sin45;
	double cosneg45 = cos45;
	osg::Vec3d start, end, norm, pos, left, right;
	osg::Matrix3d matrix1, matrix2;
	for (int i = 1; i < prim1Size; i++)
	{
		if (i % 50 != 0)
			continue;
		end = drawArray->at(i);
		start = drawArray->at(i - 1);
		norm = start - end;
		norm.normalize();
		pos = end + norm * 5; // 设置宽
		pos.z() = 1.0;
		matrix1 = osg::Matrix3d(cos45, -sin45, (1 - cos45) * start.x() + sin45 * start.y(), sin45, cos45, (1 - cos45) * start.y() - sin45 * start.x(), 0, 0, 1);
		matrix2 = osg::Matrix3d(cosneg45, -sinneg45, (1 - cosneg45) * start.x() + sinneg45 * start.y(), sinneg45, cosneg45, (1 - cosneg45) * start.y() - sinneg45 * start.x(), 0, 0, 1);
		left = osg::Vec3d(matrix1[0] * pos.x() + matrix1[1] * pos.y() + matrix1[2],
			matrix1[3] * pos.x() + matrix1[4] * pos.y() + matrix1[5], m_layerHeight);

		right = osg::Vec3d(matrix2[0] * pos.x() + matrix2[1] * pos.y() + matrix2[2],
			matrix2[3] * pos.x() + matrix2[4] * pos.y() + matrix2[5], m_layerHeight);

		drawArray->push_back(left);
		drawArray->push_back(end);
		drawArray->push_back(right);

	}

	osg::ref_ptr<osg::DrawElementsUShort> prim2 = new osg::DrawElementsUShort(GL_TRIANGLES);
	for (int i = prim1Size; i < drawArray->size(); i++)
	{
		prim2->push_back(i);
	}

	m_geometry->setVertexArray(drawArray);
	m_geometry->addPrimitiveSet(prim1);

	m_geometry->addPrimitiveSet(prim2);

}

void HDLaneLayer::setName(const std::string& name)
{
	m_curLaneInfo->SetLaneName(QString::fromStdString(name));
	Layer::setName(name);
}

osg::Geometry* HDLaneLayer::getGeometry()
{
	return m_geometry.get();
}



void HDLaneLayer::clearCalcJoinData()
{
	m_interPoint = osg::Vec3d();
	m_index = m_joinIndex = -1;
}

bool HDLaneLayer::calcInterserctions(HDLaneLayer* pickLayer)
{
	//计算交点
	osg::Vec3dArray* array = m_dVertexArray;

	for (int i = 0; i < array->size() - 2; i++)
	{
		osg::Vec3d pos1 = array->at(i);
		osg::Vec3d pos2 = array->at(i + 1);
		if (calcInterserction(pos1, pos2, pickLayer))
		{
			m_joinIndex = i;
			updateJoinedLaneInfo(pickLayer);
			return true;
		}
	}
	return false;
}

bool HDLaneLayer::judgeJoinedLayer(HDLaneLayer* pickLayer)
{
	int type = m_curLaneInfo->GetJoinedType();
	osg::Vec3d pos;
	pos.x() = m_curLaneInfo->GetJoinedPointLon();
	pos.y() = m_curLaneInfo->GetJoinedPointLat();
	pos.z() = 0.0;
	GisModel* model = EngineProxy::instance()->getGlobe();
	pos = model->convertLatLongHeightToWorldPos(pos);
	//前拼接
	if (type == 1)
	{
		return calcInterserction(m_dVertexArray->at(0), pos, pickLayer);
	}
	else if (type == 2)
	{
		return calcInterserction(pos, m_dVertexArray->at(m_dVertexArray->size() - 1), pickLayer);
	}
	return false;
}

bool HDLaneLayer::calcInterserction(osg::Vec3d& pos1, osg::Vec3d& pos2, HDLaneLayer* pickLayer)
{
	osg::ref_ptr<osg::LineSegment> pointerLine = new osg::LineSegment(pos1, pos2);
	//if (!pointerLine->intersect(m_pickMarker->getBound()))
	//{
	//	return false;
	//}


	osg::Geometry* geom = pickLayer->getGeometry();
	osg::TemplatePrimitiveFunctor<CalcInterPoint> pf;
	pf.setSegment(pos1, pos2);
	geom->accept(pf);

	if (pf.isIntersect())
	{
		m_interPoint = pf.getInterPoint();
		m_index = pf.getIntersectIndex();
		return true;
	}
	return false;
}

