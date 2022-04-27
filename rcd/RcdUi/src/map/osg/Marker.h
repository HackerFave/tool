#ifndef MARKER_H
#define MARKER_H 1

#include "MarkerShape.h"
#include <osg/PositionAttitudeTransform>
#include <osg/Material>



class  Marker : public osg::PositionAttitudeTransform
{
public:
	Marker(MarkerShape::ShapeType shapeType);
	//Marker(MarkerShape::ShapeType shapeType, const osg::Vec3d& pos);//δ��
	Marker(MarkerShape::ShapeType shapeType, const osg::Vec3d& pos);//δ��
	Marker(MarkerShape::ShapeType shapeType, osg::Material* material);//δ��
	Marker(MarkerShape::ShapeType shapeType, const PositionAttitudeTransform& pat, osg::Material* material);//δ��

public:
	// ����״̬
	void setMaterial(osg::Material* material);
	osg::Material* getMaterial() const { return _material; }

	//void setStateSet(osg::StateSet* stateSet) { _stateSet = stateSet;}
	//osg::StateSet* getStateSet() const { return _stateSet;}

	// ������ɫ
	void setColor(const osg::Vec4& color);
	const osg::Vec4& getColor();

	//// ����״̬��û��ʵ�֣�
	//void setHeading(GTCore::GeoAngle heading) { _heading = heading;}
	//GTCore::GeoAngle getHeading() const { return _heading;}

	//������������
	//void setPosition(const osg::Vec3d& pos); 
	//���þ�γ������
	//void setPosition(const GTGis::Position& pos); //{ _pos = pos;}
	//GTGis::Position getPosition() { return _pos;}
	osg::Vec3d getWorldPosition() const { return _position; }

	//������������;�γ�����겻������һ������
	void setPointNoUpdate(const osg::Vec3& pos);
	//void setPositionNoUpdate(const GTGis::Position& pos);

	// ���ű���
	void setScale(const osg::Vec3d& scale);
	// ��ð뾶��С
	float getRadius() const { return _radius; }

	// �������Ӧ��Ļ���Լ������С�������ȶ���˱���Ϊtrue
	bool getIsSelf() const { return _isScaleSelf; }
	void setIsSelf(bool isSelf) { _isScaleSelf = isSelf; }

	// ���ô�С�����Դ�С��//������δʹ��
	void setSize(float f_value);
	void setSize(osg::Vec3& halfLengths);
	void setSize(float radius, float height);

	// �����Ƿ���ʾ���������壬����������ʱ�Ƿ���ʾ����������
	void setShowFullShape(bool isShow) { _isShowFullShape = isShow; }
	bool getIsShowFullShape() const { return _isShowFullShape; }

	// ���ü�����Ķ�����
	void setRatio(float ratio);
	float getRatio();

	void setTextNeed(bool need);

	void setTextColor(const osg::Vec4& color);

	const osg::Vec4& getTextColor();

protected:
	~Marker() {};

	void InitGeode();


	//����㾭γ����Ϣ
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
