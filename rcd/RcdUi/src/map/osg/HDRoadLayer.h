#ifndef HDROADLAYER_H
#define HDROADLAYER_H


#include "Layer.h"
#include <osg/LineStipple>
#include <osg/LineWidth>
#include <osg/Geometry>

class RoadInfo;
class GisModel;
class HDRoadLayer : public Layer
{
public:
	HDRoadLayer();
	virtual ~HDRoadLayer();

	virtual bool pick(int x, int y) { return true; };

	virtual void filterVertex(osg::Vec3d& centerPos) {}

	void setRoadInfo(RoadInfo* roadInfo);
	void changeLatLonToUTM(GisModel* m_pGlobe);
	//void changeUTMToLatLon(GisModel* model);
	void drawRoad();

	virtual HDRoadLayer* asRoadLayer() { return this; }

	virtual void updateColor(osg::Vec4& color);;
	virtual void resetColor();

	void updateLeftColor();
	void resetGeometryColor();
	void updateRightColor();

	int getLeftType();
	void setLeftType(int type);
	int getRightType();
	void setRightType(int type);
	void setTwoSide(bool bTwoSide);

	void SetCurColor(bool bTwoSide);

	bool getTwoSide();
	int getElementID();

	osg::Geometry* getLeftGeometry();
	osg::Geometry* getRightGeometry();

	osg::Vec3dArray* getLeftVertexArray();
	osg::Vec3dArray* getRightVertexArray();

	osg::Vec3dArray* getLeftPosArray();
	osg::Vec3dArray* getRightPosArray();

	double getLayerHeight();

	RoadInfo* getRoadInfo();

	void updatePosArrayByCut(osg::Vec3dArray* leftPosArray, osg::Vec3dArray* rightPosArray);
	void updateVertexArrayByCut(osg::Vec3dArray* newleftArray, osg::Vec3dArray* newRightArray);

	virtual void setName(const std::string& name);

protected:
	void updateBorder();

protected:
	RoadInfo* m_curRoadInfo;
	osg::Vec3dArray* m_dLeftArray;
	osg::Vec3dArray* m_dRightArray;
	osg::Vec3dArray* m_dVertexLeftArray;
	osg::Vec3dArray* m_dVertexRightArray;
	osg::ref_ptr<osg::Geometry> m_lineGeom1;
	osg::ref_ptr<osg::Geometry> m_lineGeom2;
	osg::ref_ptr<osg::Geometry> m_polygonGeom;
	osg::Vec4 m_lineColor;
	osg::Vec4 m_polyColor;
	osg::Vec4 m_singleRoadColor = osg::Vec4(1.0, 0.78, 0.329, 0.3f);
	osg::Vec4 m_doubleRoadColor = osg::Vec4(0.576, 0.823, 0.357, 0.3f);
	double m_layerHeight;

	osg::LineWidth* m_pLineWidth1;
	osg::LineWidth* m_pLineWidth2;

	osg::LineStipple* m_pLineStrpple;
	bool m_bLeftType;
	bool m_bRightType;
};


#endif