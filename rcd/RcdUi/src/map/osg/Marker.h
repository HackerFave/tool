#ifndef MARKER_H
#define MARKER_H 1

#include "MarkerShape.h"
#include <osg/PositionAttitudeTransform>
#include <osg/Material>



class  Marker : public osg::PositionAttitudeTransform
{
public:
	Marker(MarkerShape::ShapeType shapeType);
	//Marker(MarkerShape::ShapeType shapeType, const osg::Vec3d& pos);//未用
	Marker(MarkerShape::ShapeType shapeType, const osg::Vec3d& pos);//未用
	Marker(MarkerShape::ShapeType shapeType, osg::Material* material);//未用
	Marker(MarkerShape::ShapeType shapeType, const PositionAttitudeTransform& pat, osg::Material* material);//未用

public:
	// 设置状态
	void setMaterial(osg::Material* material);
	osg::Material* getMaterial() const { return _material; }

	//void setStateSet(osg::StateSet* stateSet) { _stateSet = stateSet;}
	//osg::StateSet* getStateSet() const { return _stateSet;}

	// 设置颜色
	void setColor(const osg::Vec4& color);
	const osg::Vec4& getColor();

	//// 俯仰状态（没有实现）
	//void setHeading(GTCore::GeoAngle heading) { _heading = heading;}
	//GTCore::GeoAngle getHeading() const { return _heading;}

	//设置世界坐标
	//void setPosition(const osg::Vec3d& pos); 
	//设置经纬度坐标
	//void setPosition(const GTGis::Position& pos); //{ _pos = pos;}
	//GTGis::Position getPosition() { return _pos;}
	osg::Vec3d getWorldPosition() const { return _position; }

	//设置世界坐标和经纬度坐标不更新另一个坐标
	void setPointNoUpdate(const osg::Vec3& pos);
	//void setPositionNoUpdate(const GTGis::Position& pos);

	// 缩放倍数
	void setScale(const osg::Vec3d& scale);
	// 获得半径大小
	float getRadius() const { return _radius; }

	// 如果不适应屏幕，自己定义大小，必须先定义此变量为true
	bool getIsSelf() const { return _isScaleSelf; }
	void setIsSelf(bool isSelf) { _isScaleSelf = isSelf; }

	// 设置大小（绝对大小）//三个都未使用
	void setSize(float f_value);
	void setSize(osg::Vec3& halfLengths);
	void setSize(float radius, float height);

	// 设置是否显示整个几何体，即当贴地面时是否显示整个几何体
	void setShowFullShape(bool isShow) { _isShowFullShape = isShow; }
	bool getIsShowFullShape() const { return _isShowFullShape; }

	// 设置集合体的顶点数
	void setRatio(float ratio);
	float getRatio();

	void setTextNeed(bool need);

	void setTextColor(const osg::Vec4& color);

	const osg::Vec4& getTextColor();

protected:
	~Marker() {};

	void InitGeode();


	//加入点经纬度信息
	void createText();

	inline int getIntSize(int num);

private:
	osg::ref_ptr<osg::StateSet> _stateSet;
	osg::ref_ptr<osg::Material> _material;
	MarkerShape::ShapeType _Type;

	//GTCore::GeoAngle _heading;
	float _radius;
	osg::Vec3d _position;

	//GTGis::Position _pos;
	bool _isShowFullShape;

	bool _isScaleSelf;

	//osg::ref_ptr<EarthText> _text;


	bool _needText;
};

#endif
