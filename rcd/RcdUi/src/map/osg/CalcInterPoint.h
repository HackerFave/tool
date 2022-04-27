#ifndef CALCINTERPOINT_H
#define CALCINTERPOINT_H

#include <osg/Vec3d>
#include <osg/Vec3>
#include <osg/Array>

class CalcInterPoint
{
private:
	osg::Vec3d _pos;
	bool _isInter;
	osg::Vec3d _start;
	osg::Vec3d _end;
	unsigned int _index;
	unsigned int _index2;
	osg::ref_ptr<osg::Vec3dArray> _posArray;
	bool _singleSelect;
	bool _bSecendInter;
public:
	CalcInterPoint();
	virtual ~CalcInterPoint();
	bool isIntersect()
	{
		return _isInter;
	}

	osg::Vec3d& getInterPoint()
	{
		return _pos;
	}
	void setMultiSelect()
	{
		_singleSelect = false;
	}

	osg::Vec3dArray* getInterPointList()
	{
		return _posArray.get();
	}

	unsigned int getIntersectIndex()
	{
		return _index;
	}

	unsigned int getIntersectIndex2()
	{
		return _index2;
	}

	void clear();

	void setSegment(const osg::Vec3d& start, const osg::Vec3d& end);
	void operator() (const osg::Vec3& v1, bool) const
	{
		//std::cout << "\rpoint(" << v1 << ")" << std::endl;
	}
	bool segmentsInterSector(const osg::Vec3d& c, const osg::Vec3d& d);

	void operator() (const osg::Vec3& v1, const osg::Vec3& v2, bool);

	void operator() (const osg::Vec3d& v1, const osg::Vec3d& v2, bool);

	void operator() (const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3, bool) const
	{
	}

	void operator() (const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3, const osg::Vec3& v4, bool) const
	{
	}
	void calcIntersector(osg::Vec3dArray* array);
};

#endif