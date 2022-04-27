#include "EngineProxy.h"
#include "GridLayer.h"
#include "FlatModel.h"
//#include "quickosgviewer.h"
#include "LaneInfoManager.h"
#include "HDPolygonManager.h"
#include "HDRoadManager.h"
#include "SqliteReader.h"
#include "HDBoundaryLayer.h"
//#include "HmiFunction/paraparsing.h"
//#include "pub.h"
#include "VehicleLayer.h"

EngineProxy* EngineProxy::_proxy = 0;

void EngineProxy::generateHDMapData()
{

	auto boundaryList = HDPolygonManager::instance()->getPolyVector();

	for (int i = 0; i < boundaryList.size(); i++)
	{

		osg::ref_ptr<HDBoundaryLayer> layer = new HDBoundaryLayer;
		layer->setPolyInfo(boundaryList[i]);
		layer->changeLatLonToUTM(m_pGlobe);
		layer->drawBoundary();
		m_pBoundaryList->addLayer(layer);
	}

	//生成路段;
	auto roadList = HDRoadManager::instance()->getRoadVector();
	for (int i = 0; i < roadList.size(); i++)
	{
		osg::ref_ptr<HDRoadLayer> layer = new HDRoadLayer;
		layer->setRoadInfo(roadList[i]);
		layer->changeLatLonToUTM(m_pGlobe);
		layer->drawRoad();
		m_pRoadList->addLayer(layer);
	}


	//生成车道,类似原始数据;
	auto laneInfoList = LaneInfoManager::instance()->getLaneInfoList();
	QList<HDLaneLayer*> joinedLayer;
	for (int i = 0; i < laneInfoList.size(); i++)
	{
		osg::ref_ptr<HDLaneLayer> layer = new HDLaneLayer;
		layer->setLaneInfo(laneInfoList[i]);
		layer->changeLatLonToUTM();
		layer->drawLane();
		if (laneInfoList[i]->GetJoinedType() != 0)
		{
			joinedLayer.push_back(layer.get());
		}
		m_pLaneList->addLayer(layer);
	}
	
    resetRenderGroup(0);

	reDrawGrid();
}

void EngineProxy::initResourceMap()
{
	//m_resourceMap.insert(MINELOADER, "resource/selfmineload.png");
	//m_resourceMap.insert(WIDEMINELOADER, "resource/selfwidemineload.png");
	//m_resourceMap.insert(MINEHALFLOADER, "resource/selfmineload.png");
	//m_resourceMap.insert(WIDEMINEHALFLOADER, "resource/selfwidemineload.png");
	//m_resourceMap.insert(MINE, "resource/selfmineunload.png");
	//m_resourceMap.insert(WIDEMINE, "resource/selfwidemineunload.png");
	//m_resourceMap.insert(EXCAVATOR, "resource/selfexcavator.png");
    m_resourceMap.insert(0, "resource/map/car.png");

	//m_resourceMap.insert(DIGGER, "resource/selfdigger.png");
	//m_resourceMap.insert(CRUSHER, "resource/selfcrush.png");
	//m_resourceMap.insert(PARKING, "resource/parkingMap.png");
	//m_resourceMap.insert(COLLECTOR, "resource/selfcollector.png");

	//m_resourceMap.insert(STOPPOINTS, "resource/stopPos2.png");
	//m_resourceMap.insert(WARNING, "resource/siteIco.png");

}

EngineProxy::EngineProxy()
	:m_pGlobe(nullptr)
{
}


EngineProxy* EngineProxy::instance()
{
	if (!_proxy)
	{
		_proxy = new EngineProxy();
	}
	return _proxy;
}

EngineProxy::~EngineProxy()
{
	delete m_pGlobe;
	m_pGlobe = nullptr;
}

void EngineProxy::setEyePosition(osg::Vec3d& position)
{
	if (m_pManipulator)
	{
		m_pManipulator->setHomePosition(position);
	}
}

GisModel* EngineProxy::getGlobe()
{
	return m_pGlobe;
}


void EngineProxy::clearCurData()
{
	while (m_pLaneList->getNumChildren() != 0)
	{
		m_pLaneList->removeLayer((Layer*)m_pLaneList->getChild(0));
	}
	LaneInfoManager::instance()->clearLaneInfo();
	while (m_pBoundaryList->getNumChildren() != 0)
	{
		m_pBoundaryList->removeLayer((Layer*)m_pBoundaryList->getChild(0));
	}
	HDPolygonManager::instance()->clear();

	while (m_pRoadList->getNumChildren() != 0)
	{
		m_pRoadList->removeLayer((Layer*)m_pRoadList->getChild(0));
	}
	HDRoadManager::instance()->clear();
}

void EngineProxy::setManipulator(OrthoManipulator* mani)
{
	m_pViewer->setCameraManipulator(m_pManipulator, true);
}

void EngineProxy::setRenderGroup(osg::Group* group)
{
	m_pRoot = group;
}

void EngineProxy::init(osg::GraphicsContext* context)
{

	m_pViewer = new osgViewer::Viewer;
	m_pViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
	m_pComViewer = new osgViewer::CompositeViewer;
	{
		m_pComViewer->setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);
	}

	double w = context->getTraits()->width;
	double h = context->getTraits()->height;
	osg::Camera* camera = m_pViewer->getCamera();
	camera->setGraphicsContext(context);
	camera->setViewport(new osg::Viewport(0, 0, w, h));
	camera->setProjectionMatrixAsOrtho2D(0.0, w, 0.0, h);
    camera->setClearColor(osg::Vec4(0,45/255.0,72/255.0,1.0)/*osg::Vec4(1, 1, 1, 1)*/);
//    camera->setClearMask(1);
//    camera->setColorMask(0,45/255.0,72/255.0,0.5);
	osg::CullStack::CullingMode cullingMode = camera->getCullingMode();

	cullingMode &= ~(osg::CullStack::SMALL_FEATURE_CULLING);

	camera->setCullingMode(cullingMode);
	camera->setViewMatrix(osg::Matrix::identity());
	//camera->setClearColor(osg::Vec4(1.0, 1.0, 1.0,1.0));
	//camera->setViewMatrixAsLookAt(osg::Vec3d(0.0, 0.0, 45.0), osg::Vec3d(0.0,0.0,0.0), osg::Vec3d(0.0, 1.0,0.0));
	m_pComViewer->addView(m_pViewer);
	m_pViewer->realize();//一定要在此实现，否则不能将qopenglcontext移动到图形线程


	initResourceMap();
	m_pRoot = new osg::Group();
	m_pGlobe = new FlatModel();
	m_pManipulator = new OrthoManipulator(m_pViewer);
	setManipulator(m_pManipulator);
	m_pManipulator->addViewChangeListener(m_pGlobe);

    //m_gridLayer = new GridLayer();

	m_pNoPickLayerList = new LayerList();
    //m_pNoPickLayerList->addLayer(m_gridLayer);
	m_pRoot->addChild(m_pNoPickLayerList);
	
	m_drawGroup = new osg::Group();

	m_pLaneList = new HDLaneLayerList();
	m_drawGroup->addChild(m_pLaneList);

	m_pBoundaryList = new HDBoundaryLayerList();
	m_drawGroup->addChild(m_pBoundaryList);

	m_pRoadList = new HDRoadLayerList();
	m_drawGroup->addChild(m_pRoadList);

	m_pVehicleList = new VehicleLayerList();
	m_drawGroup->addChild(m_pVehicleList);

	m_pRouteTaskLayer = new RouteAndTaskLayer();
	m_drawGroup->addChild(m_pRouteTaskLayer);

	m_pRoot->addChild(m_drawGroup);

	m_pViewer->setSceneData(m_pRoot);
}
//reset - tanchuang
void EngineProxy::resetRenderGroup(osg::Node* group)
{
	if (group == 0)
	{
        group = m_pRoot;
	}
	if (group->getBound().radius() > 0.0)
	{
		m_pManipulator->resetRoot(group);
	}
}

QString EngineProxy::getResourceByType(int type)
{
	auto itr = m_resourceMap.find(type);
	if (itr != m_resourceMap.end())
	{
		return itr.value();
	}
	return QString();
}

void EngineProxy::reDrawGrid()
{
    osg::Vec3d pos = m_pGlobe->getCenterWorldPos(0, 0);
    pos = m_pGlobe->convertWorldPosToLatLongHeight(pos);
    //m_gridLayer->RedrawByUpdate(pos.y(), pos.x());

	osg::ref_ptr<VehicleLayer> layer = new VehicleLayer(0);
    layer->UpdateLatLon(pos);// jingweidu
    layer->UpdateHeading(30.0);//hangxiangjiao
	m_pVehicleList->addLayer(layer);
}

void EngineProxy::frame()
{
	m_pComViewer->frame();
}

HDLaneLayerList* EngineProxy::getLanelayerList()
{
	return m_pLaneList.get();
}

HDRoadLayerList* EngineProxy::getRoadLayerList()
{
	return m_pRoadList.get();
}

HDBoundaryLayerList* EngineProxy::getBoundaryLayerList()
{
	return m_pBoundaryList.get();
}

VehicleLayerList* EngineProxy::getVehicleLayerList()
{
	return m_pVehicleList.get();
}

RouteAndTaskLayer* EngineProxy::getRouteAndTaskLayer()
{
	return m_pRouteTaskLayer.get();
}

void EngineProxy::ReadDbInfo(QString filepath)
{
	clearCurData();
	SqliteReader reader(filepath);
	reader.open();
	reader.read();
	reader.close();
	generateHDMapData();
}

//void EngineProxy::RenderCreateComplete() const
//{
//	QuickOSGViewer* viewer = dynamic_cast<QuickOSGViewer*>(sender());
//	EngineProxy::instance()->setManipulator(viewer->getManipulator());
//	EngineProxy::instance()->setRenderGroup(viewer->getRoot());
//	EngineProxy::instance()->init();
//
//	if (ParaParsing::instance()->paraSt.pub.vehicleType != COLLECTOR)
//	{
//		qDebug() << ParaParsing::instance()->paraSt.pub.mapPath + "/HDMap.db";
//		EngineProxy::instance()->ReadDbInfo(ParaParsing::instance()->paraSt.pub.mapPath + "/HDMap.db");
//	}
//}

