//#include <osg\Math>
#include <osg/Math>
#include "OrthoManipulator.h"
#include <QDebug>

OrthoManipulator::OrthoManipulator(osgViewer::Viewer* viewer, osg::Node* root)
	: _viewer(viewer), _scaleRatio(1.0)
	, _zoom_radio(0.9), _root(root), _eye(0, 0, 0)
{
	setName("ortho");
	_first = true;
}


OrthoManipulator::~OrthoManipulator(void)
{
}


void OrthoManipulator::setByMatrix(const osg::Matrixd& matrix)
{

}

void OrthoManipulator::setByInverseMatrix(const osg::Matrixd& matrix)
{

}

osg::Matrixd OrthoManipulator::getMatrix() const
{
	return	osg::Matrix::translate(_eye);
}

osg::Matrixd OrthoManipulator::getInverseMatrix() const
{
	return osg::Matrixd::inverse(osg::Matrix::translate(_eye));
}

bool OrthoManipulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	if (ea.getHandled()) return false;

	if (ea.getEventType() == osgGA::GUIEventAdapter::FRAME)
	{
		updateOrthoProject();
	}
	if (ea.getEventType() == osgGA::GUIEventAdapter::SCROLL)
	{
		zoomOn(ea);
		return true;
	}
	if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH)
	{
		flushMouseEventStack();
		addMouseEvent(ea);
		return true;
	}
	if (ea.getEventType() == osgGA::GUIEventAdapter::DRAG)
	{
		addMouseEvent(ea);

		if (ea.getButtonMask() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
		{
			return changeEyePos();
		}
		else
			//准备发送给editor去操作;
			return false;
	}
	return false;
}

void OrthoManipulator::updateOrthoProject()
{
	double left, right, top, bottom;
	calcOrtho(left, right, bottom, top);


	_viewer->getCamera()->setProjectionMatrixAsOrtho(left, right,
		bottom, top, -10000.0f, 10000.0f);//-10000.0f, 10000.0f
	if (_first)
	{
		_first = false;
		sendViewChange();
	}
}

void OrthoManipulator::home(double)
{
	osg::BoundingBox bounding = computeCloudCenter();
	if (bounding.radius2() == 0)
	{
        qDebug() << "empty box";
		return;
	}

	double xSize = bounding.xMax() - bounding.xMin();
	double ySize = bounding.yMax() - bounding.yMin();

	int viewWidth = _viewer->getCamera()->getViewport()->width();
	int viewHeight = _viewer->getCamera()->getViewport()->height();

	double maxSize = osg::maximum(xSize, ySize);
	double minWidth = osg::minimum(viewWidth, viewHeight);
	double ratio = maxSize / double(minWidth);

	_scaleRatio = ratio;
	_eye = bounding.center() + osg::Vec3d(0, 0, 1000);

	sendViewChange();
	
}

void OrthoManipulator::zoomOn(const osgGA::GUIEventAdapter& ea)
{
	osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();
	double zoom_radio = sm == osgGA::GUIEventAdapter::SCROLL_UP ? _zoom_radio : 1.0 / _zoom_radio;
	double ratio_ = _scaleRatio * zoom_radio;
	//if (ratio_ < 0.100)
	//{
	//	return;
	//}
	int viewWidth = _viewer->getCamera()->getViewport()->width();
	int viewHeight = _viewer->getCamera()->getViewport()->height();

	double x = ea.getX();
	double y = ea.getY();

	double transX = (ea.getX() - viewWidth / 2) * _scaleRatio * (1 - zoom_radio);
	double transY = (ea.getY() - viewHeight / 2) * _scaleRatio * (1 - zoom_radio);

	_eye += osg::Vec3d(transX, transY, 0);

	_scaleRatio = ratio_;
	sendViewChange();
}

void OrthoManipulator::addMouseEvent(const osgGA::GUIEventAdapter& ea)
{
	_ga_t1 = _ga_t0;
	_ga_t0 = &ea;
}

void OrthoManipulator::flushMouseEventStack()
{
	_ga_t1 = 0L;
	_ga_t0 = 0L;
}

void OrthoManipulator::setHomePosition(const osg::Vec3d& center)
{
	//_scaleRatio = 0.2;
	_eye = center + osg::Vec3d(0, 0, 50);
}

void OrthoManipulator::setEye(osg::Vec3d& eye)
{
	_eye = eye;
}

bool OrthoManipulator::changeEyePos()
{
	if (_ga_t0.get() == NULL || _ga_t1.get() == NULL)
		return false;

	double deltaX = _ga_t1->getX() - _ga_t0->getX();
	double deltaY = _ga_t1->getY() - _ga_t0->getY();

	double transX = deltaX * _scaleRatio;
	double transY = deltaY * _scaleRatio;

	_eye += osg::Vec3d(transX, transY, 0);

	sendViewChange();
	return true;
}

void OrthoManipulator::resetRoot(osg::Node* root)
{
    qDebug() << "enter resetRoot";
	_root = root;
	home(1.0);
}

void OrthoManipulator::calcOrtho(double& left, double& right, double& bottom, double& top)
{
	int viewWidth = _viewer->getCamera()->getViewport()->width();
	int viewHeight = _viewer->getCamera()->getViewport()->height();

	left = -double(viewWidth) / 2.0 * _scaleRatio;
	right = double(viewWidth)* _scaleRatio + left;
	bottom = double(-viewHeight) / 2.0 *_scaleRatio;
	top = double(viewHeight) * _scaleRatio + bottom;

	m_viewerWidth = viewWidth;
	m_viewerHeight = viewHeight;
}

osg::Vec4d OrthoManipulator::calcOrtho()
{
	double left, right, bottom, top;
	calcOrtho(left, right, bottom, top);
	return osg::Vec4d(left, right, bottom, top);
}

osg::BoundingBox OrthoManipulator::computeCloudCenter()
{
	//osg::Node* node = _viewer->getSceneData();
	//FindPCLDrawableVistor fpclv;
	osg::BoundingBox bounding;

	//_root->accept(fpclv);
	//std::vector<CXPCLDrawable *> pcls = fpclv.getPCLDrawableList();
	//if (pcls.empty())
	//{
	//	osg::BoundingSphere sphere = node->getBound();
	//	sphere.radius() = osg::maximum(sphere.radius(), 100.0f);
	//	bounding.expandBy(sphere);
	//}
	//else
	//{
	//	for (std::vector<CXPCLDrawable *>::iterator itr = pcls.begin(); itr != pcls.end(); itr++)
	//	{
	//		CXPCLDrawable* pcl = *itr;
	//		osg::BoundingSphere sphere = pcl->getBound();
	//		bounding.expandBy(sphere);
	//	}
	//}
	osg::BoundingSphere sphere = _root->getBound();
	sphere.radius() = osg::maximum(sphere.radius(), 0.0f);//100.0f
	bounding.expandBy(sphere);

	return bounding;
}


//osg::BoundingBox osgGA::OrthoManipulator::computeCloudCenter()
//{
//	osg::Node* node = _viewer->getSceneData();
//	FindPCLDrawableVistor fpclv;
//	osg::BoundingBox bounding;
//
//	_root->accept(fpclv);
//	std::vector<CXPCLDrawable *> pcls = fpclv.getPCLDrawableList();
//	if (pcls.empty())
//	{
//		osg::BoundingSphere sphere = _root->getBound();
//		sphere.radius() = osg::maximum(sphere.radius(), 100.0f);
//		bounding.expandBy(sphere);
//	}
//	else
//	{
//		for (std::vector<CXPCLDrawable *>::iterator itr = pcls.begin(); itr != pcls.end(); itr++)
//		{
//			CXPCLDrawable* pcl = *itr;
//			osg::BoundingSphere sphere = pcl->getBound();
//			bounding.expandBy(sphere);
//		}
//	}
//	return bounding;
//}

bool OrthoManipulator::sendViewChange()
{
	ViewChangeEvent event;
	event.sector = calcOrtho();
	event.eyePos = _eye;
	event.scaleRadio = _scaleRatio;
	event.view.set(m_viewerWidth, m_viewerHeight);
	for (int i = 0; i < m_vListeners.size(); i++)
	{
		if (!m_vListeners[i]->viewChanged(event))
		{
			return false;
		}
	}
	return true;
}

void OrthoManipulator::addViewChangeListener(ViewChangeListener* listener)
{
	m_vListeners.push_back(listener);

	ViewChangeEvent event;
	event.sector = calcOrtho();
	event.eyePos = _eye;
	event.scaleRadio = _scaleRatio;
	event.view.set(m_viewerWidth, m_viewerHeight);
	//为新加的listener增加信息;
	listener->viewChanged(event);
}

