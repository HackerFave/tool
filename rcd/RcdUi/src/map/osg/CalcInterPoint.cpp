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
	// ������abc �����2��  
	double area_abc = (_start.x() - c.x()) * (_end.y() - c.y()) - (_start.y() - c.y()) * (_end.x() - c.x());

	// ������abd �����2��  
	double area_abd = (_start.x() - d.x()) * (_end.y() - d.y()) - (_start.y() - d.y()) * (_end.x() - d.x());

	// ���������ͬ���������߶�ͬ��,���ཻ (�Ե����߶��ϵ����,�����������ཻ����);  
	if (area_abc*area_abd >= 0) {
		return false;
	}

	// ������cda �����2��  
	double area_cda = (c.x() - _start.x()) * (d.y() - _start.y()) - (c.y() - _start.y()) * (d.x() - _start.x());
	// ����ͬ������
	if (abs(area_cda) > 0.0001)
	{
		// ������cdb �����2��  
		// ע��: ������һ��С�Ż�.����Ҫ���ù�ʽ�������,����ͨ����֪����������Ӽ��ó�.  
		double area_cdb = area_cda + area_abc - area_abd;
		if (area_cda * area_cdb >= 0) {
			return false;
		}
	}

	//���㽻������;  
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

