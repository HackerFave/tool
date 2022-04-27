#ifndef HDBOUNDARYLAYER_H
#define HDBOUNDARYLAYER_H
#include "Layer.h"
#include <osg/Geometry>
#include "HDRoadLayer.h"


class PolyInfo;
class GisModel;

class HDBoundaryLayer : public Layer
{
public:
	HDBoundaryLayer();
	virtual ~HDBoundaryLayer();

	virtual bool pick(int x, int y) { return true; };

	virtual void filterVertex(osg::Vec3d& centerPos) {}

	void setPolyInfo(PolyInfo* laneInfo);
	PolyInfo* getPolyInfo() { return m_curPolyInfo; }
	void changeLatLonToUTM(GisModel* m_pGlobe);
	void changeUTMToLatLon(GisModel* model);
	void drawBoundary();

	virtual HDBoundaryLayer* asBoundaryLayer() { return this; };


	virtual void updateColor(osg::Vec4& color);;
	virtual void resetColor();

	osg::Vec3dArray* getPosArray() { return m_dPosArray; }

	osg::Vec3dArray* getVertexArray() { return m_dVertexArray; }

	int getRecType();

	int getRecId();
	virtual void setName(const std::string& name);

	HDRoadLayer* getRelatedRoadLayer();

protected:
	void setVertexArray(osg::Vec3dArray* vertexarray);
protected:
	void generateBoundaryArray();
	void SetCurColor(int type);
protected:
	PolyInfo* m_curPolyInfo;
	osg::Vec3dArray* m_dPosArray;
	osg::Vec3dArray* m_dVertexArray;
	bool m_needUpdate;
	osg::ref_ptr<osg::Geometry> m_lineGeom;
	osg::ref_ptr<osg::Geometry> m_polygonGeom;
	osg::Vec4 m_lineColor;
	osg::Vec4 m_polyColor;
	double m_layerHeight;
	osg::Vec4 m_loadColor = osg::Vec4(0.56, 0.67, 0.98, 0.3f);
	osg::Vec4 m_unloadColor = osg::Vec4(0.196, 0.886, 0.196, 0.3f);
	osg::Vec4 m_parkColor = osg::Vec4(0.878, 0.4, 1, 0.3f);

	osg::Vec4 m_oilColor = osg::Vec4(1, 0, 0, 0.3f);
	osg::Vec4 m_maintenanceColor = osg::Vec4(0, 1, 0, 0.3f);



	osg::Vec4 m_otherColor = osg::Vec4(0.047, 0.95, 0.91, 0.3f);
	osg::PositionAttitudeTransform* m_text;
};

#endif