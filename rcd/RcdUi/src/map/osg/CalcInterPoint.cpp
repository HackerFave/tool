#include "CalcInterPoint.h"
#include <osg/Array>




CalcInterPoint::CalcInterPoint()
{
	_posArray = new osg::Vec3dArray;
	_index = -1;
	_index2 = -1;
	_isInter = false;
	_singleSelect = true;
	_bSecendInter = false;
}

CalcInterPoint::~CalcInterPoint()
{
	_posArray->unref();
}

void CalcInterPoint::clear()
{
	_pos = osg::Vec3d();
	_isInter = false;

	_index = -1;
	_posArray->clear();
}

void CalcInterPoint::setSegment(const osg::Vec3d& start, const osg::Vec3d& end)
{
	_start = start;
	_end = end;
}

bool CalcInterPoint::segmentsInterSector(const osg::Vec3d& c, const osg::Vec3d& d)
{
	if (!_bSecendInter)
	{
		_index2++;
	}
	// 三角形abc 面积的2倍  
	double area_abc = (_start.x() - c.x()) * (_end.y() - c.y()) - (_start.y() - c.y()) * (_end.x() - c.x());

	// 三角形abd 面积的2倍  
	double area_abd = (_start.x() - d.x()) * (_end.y() - d.y()) - (_start.y() - d.y()) * (_end.x() - d.x());

	// 面积符号相同则两点在线段同侧,不相交 (对点在线段上的情况,本例当作不相交处理);  
	if (area_abc*area_abd >= 0) {
		return false;
	}

	// 三角形cda 面积的2倍  
	double area_cda = (c.x() - _start.x()) * (d.y() - _start.y()) - (c.y() - _start.y()) * (d.x() - _start.x());
	// 考虑同点的情况
	if (abs(area_cda) > 0.0001)
	{
		// 三角形cdb 面积的2倍  
		// 注意: 这里有一个小优化.不需要再用公式计算面积,而是通过已知的三个面积加减得出.  
		double area_cdb = area_cda + area_abc - area_abd;
		if (area_cda * area_cdb >= 0) {
			return false;
		}
	}

	//计算交点坐标;  
	double t = area_cda / (area_abd - area_abc);
	double dx = t * (_end.x() - _start.x());
	double	dy = t * (_end.y() - _start.y());
	//return { x: _start.x + dx , y : _start.y + dy };
	_pos.x() = _start.x() + dx;
	_pos.y() = _start.y() + dy;
	_pos.z() = -50.0;
	if (!_singleSelect)
	{
		_posArray->push_back(_pos);
		if (_posArray->size() == 2)
		{
			_bSecendInter = true;
		}
	}


	return true;
}

void CalcInterPoint::operator()(const osg::Vec3& v1, const osg::Vec3& v2, bool)
{
	if (!_isInter)
	{
		_isInter = segmentsInterSector(v1, v2);
		_index++;
	}
	else {
		if (!_singleSelect)
		{
			segmentsInterSector(v1, v2);
		}

	}

	//std::cout << "\tline(" << v1 << ") (" << v2 << ")" << std::endl;
}

void CalcInterPoint::operator()(const osg::Vec3d& v1, const osg::Vec3d& v2, bool)
{
	if (!_isInter)
	{
		_isInter = segmentsInterSector(v1, v2);
		_index++;
	}
	else
	{
		if (!_singleSelect)
		{
			segmentsInterSector(v1, v2);
		}
	}
}


void CalcInterPoint::calcIntersector(osg::Vec3dArray* array)
{
	for (int i = 1; i < array->size(); i++)
	{
		this->operator()(array->at(i - 1), array->at(i), false);
	}
}

