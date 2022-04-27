#ifndef HDLANELAYER_H
#define HDLANELAYER_H

#include "Layer.h"
#include <osg/Geometry>

class LaneInfo;
class GisModel;

class HDLaneLayer : public Layer
{
public:
	HDLaneLayer();
	virtual ~HDLaneLayer();

	virtual bool pick(int x, int y) { return true; };

	virtual void filterVertex(osg::Vec3d& centerPos) {}

	void setLaneInfo(LaneInfo* laneInfo);
	LaneInfo* getLaneInfo();

	void getElementInfo(int& type, int& id);

	void changeLatLonToUTM();
	void changeUTMToLatLon();
	void drawLane();
	void drawJoinedLane(HDLaneLayer* pickLayer);
	void drawJoinedLaneInit(HDLaneLayer* pickLayer);


	virtual HDLaneLayer* asLaneLayer() { return this; };

	virtual void updateColor(osg::Vec4& color);
	virtual void resetColor();

	osg::Vec3dArray* getVertexArray();

	double getLayerHeight() { return m_layerHeight; }

	void updateVertexArrayByCut(osg::Vec3dArray* array);

	virtual void setName(const std::string& name);

	osg::Geometry* getGeometry();



public:
	//拼接车道线相关
	void clearCalcJoinData();
	bool calcInterserctions(HDLaneLayer* pickLayer);
	bool judgeJoinedLayer(HDLaneLayer* pickLayer);
	bool calcInterserction(osg::Vec3d& pos1, osg::Vec3d& pos2, HDLaneLayer* pickLayer);


protected:
	//testuse为了测试道路加的函数目前不用;
	void setPosArray(osg::Vec3dArray* array);

	//为绘制拼接车道线做的相应的工作;



	void updateJoinedLaneInfo(HDLaneLayer* pickLayer);

	void drawLaneInterval(osg::Vec3Array* drawArray);

protected:
	LaneInfo* m_curLaneInfo;
	osg::Vec3dArray* m_dPosArray;
	osg::Vec3dArray* m_dVertexArray;
	osg::Vec3dArray* m_dVertexJoinedArray;
	bool m_needUpdate;
	osg::ref_ptr<osg::Geometry> m_geometry;
	double m_layerHeight;



	osg::Vec3d m_interPoint;
	long int m_index;
	long int m_joinIndex;
};

#endif