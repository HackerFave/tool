#ifndef POLYINFO_H
#define POLYINFO_H
#include <osg/Array>
#include <QString>
#include <QVector>

class PolyInfo
{
public:
	PolyInfo() {};
	void setInfos(int id, int type, osg::Vec3dArray* array);

	QString getPolyInfo();
	QString getMercInfo();
	QString getPolyInfoByHD();
	QString getPolyInfoTmpByHD();
	int getID() { return m_id; }
	int getType() { return m_type; }
	osg::Vec3dArray* getPosArray() { return m_array; }

	const QString& getName() { return m_strName; }
	void setName(QString& name) { m_strName = name; }

	const QString& getAttr() { return m_strAttr; }
	void setAttr(QString& attr) { m_strAttr = attr; }

	void updatePosArray(osg::Vec3dArray* array);

protected:
	osg::Vec3dArray* m_array;
	int m_type;
	int m_id;
	QString m_strAttr;
	QString m_strName;
};



#endif