#ifndef _ORTHOMANIPULATOR_H_
#define _ORTHOMANIPULATOR_H_


//#include <osgGA\CameraManipulator>
//#include <osg\BoundingBox>
//#include <osgViewer\Viewer>
#include <osgGA/CameraManipulator>
#include <osg/BoundingBox>
#include <osgViewer/Viewer>
#include <QList>
#include "ViewChangeListener.h"


class OrthoManipulator : public osgGA::CameraManipulator
{
public:
	OrthoManipulator(osgViewer::Viewer* viewer, osg::Node* root = nullptr);
	virtual ~OrthoManipulator(void);
	virtual void setHomePosition(const osg::Vec3d& center);
	virtual void setByMatrix(const osg::Matrixd& matrix);
	virtual void setByInverseMatrix(const osg::Matrixd& matrix);
	virtual osg::Matrixd getMatrix() const;
	virtual osg::Matrixd getInverseMatrix() const;
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

	void addViewChangeListener(ViewChangeListener* listener);

	const double& getScaleRation();
	const osg::Vec3d& getEye();
	void setEye(osg::Vec3d& eye);
	void home(double);

	void resetRoot(osg::Node* root);
protected:
	void updateOrthoProject();
	inline bool changeEyePos();
	inline void calcOrtho(double& left, double& right, double& bottom, double& top);
	inline osg::Vec4d calcOrtho();
	bool sendViewChange();
	void zoomOn(const osgGA::GUIEventAdapter& ea);
	void addMouseEvent(const osgGA::GUIEventAdapter& ea);
	void flushMouseEventStack();
	osg::BoundingBox computeCloudCenter();
private:
	osg::Vec3d _eye;
	osgViewer::Viewer* _viewer;
	osg::Node* _root;
	double _scaleRatio;
	double _zoom_radio;

	bool _first;

	QList<ViewChangeListener*>	m_vListeners;

	osg::ref_ptr< const osgGA::GUIEventAdapter > _ga_t1;
	osg::ref_ptr< const osgGA::GUIEventAdapter > _ga_t0;
	int m_viewerWidth;
	int m_viewerHeight;
};

inline const double& OrthoManipulator::getScaleRation() { return _scaleRatio; }
inline const osg::Vec3d& OrthoManipulator::getEye() { return _eye; }

#endif;
