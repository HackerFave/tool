#ifndef _ENGINEPROXY_H_
#define _ENGINEPROXY_H_
#include <QList>
#include <osg/Vec3d>
#include "ViewChangeListener.h"
#include "OrthoManipulator.h"
#include "LayerList.h"
#include <QObject>
#include "HDLaneLayerList.h"
#include "HDRoadLayerList.h"
#include "HDBoundaryLayerList.h"
#include "GridLayer.h"
#include "VehicleLayerList.h"
#include "RouteAndTaskLayer.h"
#include <QMap>
#include <osgViewer/CompositeViewer>
#include <osgViewer/Viewer>

class GisModel;

class EngineProxy : public QObject
{
	Q_OBJECT
public:
	static EngineProxy* instance();


	virtual ~EngineProxy();

	void setEyePosition(osg::Vec3d& position);

	GisModel* getGlobe();

	void clearCurData();

	void setManipulator(OrthoManipulator* mani);

	void setRenderGroup(osg::Group* group);

	void init(osg::GraphicsContext* context);

	void resetRenderGroup(osg::Node* group);

	QString getResourceByType(int type);

	void reDrawGrid();

	void frame();

	HDLaneLayerList* getLanelayerList();
	HDRoadLayerList* getRoadLayerList();
	HDBoundaryLayerList* getBoundaryLayerList();
	VehicleLayerList* getVehicleLayerList();
	RouteAndTaskLayer* getRouteAndTaskLayer();
	void ReadDbInfo(QString filepath);
public slots:
	//void RenderCreateComplete() const;
protected:
	EngineProxy();

	void generateHDMapData();

	void initResourceMap();

protected:
	static EngineProxy* _proxy;

protected:
	GisModel* m_pGlobe;
	osg::ref_ptr<OrthoManipulator> m_pManipulator;
	osg::ref_ptr<LayerList> m_pNoPickLayerList;
	osg::ref_ptr<osgViewer::Viewer> m_pViewer;
	osg::ref_ptr<osgViewer::CompositeViewer> m_pComViewer;
	osg::ref_ptr<osg::Group> m_pRoot;

	osg::ref_ptr<HDLaneLayerList> m_pLaneList;
	osg::ref_ptr<HDBoundaryLayerList> m_pBoundaryList;
	osg::ref_ptr<HDRoadLayerList> m_pRoadList;
	osg::ref_ptr<VehicleLayerList> m_pVehicleList;
	osg::ref_ptr<RouteAndTaskLayer> m_pRouteTaskLayer;
	osg::Group* m_drawGroup;
	GridLayer* m_gridLayer;
	QMap<int, QString> m_resourceMap;
};

#endif
