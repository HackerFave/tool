#ifndef LANETRAILINFO_H
#define LANETRAILINFO_H
#include <QList>
#include <osg/Vec3d>

class LaneTrailInfo
{
public:
	void ReadInfos(QString& data);
	void setId(unsigned int id);
	void cpInfo(LaneTrailInfo& copyInfo);
	void setLonLatHeight(osg::Vec3d& vec);
	void updateLonLat(double lon, double lat);
	osg::Vec3d getLonLatHeight();
	void setHeading(double heading);
	void setSpeed(double speed);
	QString generateWriteInfo();
	QString generateTmpInfo();

	void setInfos(unsigned int& pID, QStringList& list);

	QString getLpxString();

	unsigned int pointID;
	double Heading;
	double Lat;
	double Lon;
	double Alt;
	double Speed;
	double LDis;
	double RDis;
	double CollectAlt;
};
class LaneInfo
{
public:
	LaneInfo();
	virtual ~LaneInfo();

	void cpLaneInfo(LaneInfo&);

	int getLaneId() const { return m_idLane; }
	void setLaneId(int id) { m_idLane = id; }
	int getElementType() const { return m_typeElement; }
	void setElementType(int val) { m_typeElement = val; }
	int getElementId() const { return m_idElement; }
	void setElementId(int val) { m_idElement = val; }
	int getPreNodeId() const { return m_idPreNode; }
	void setPreNodeId(int val) { m_idPreNode = val; }
	int getSucNodeId() const { return m_idSucNode; }
	void setSucNodeId(int val) { m_idSucNode = val; }

	void readLaneListFromJson(QByteArray& info);

	void readLaneTrainInfo(QString& info);
	void readLaneTrainInfoFromRawData(QString& filename);

	QString generateLaneTrainInfo();

	QString generateLaneTmpInfo();

	QList<LaneTrailInfo>& getLaneList();
	void setLaneTrailSpeed(int id, double value);
	osg::Vec3d getLaneTrailLatLonHeight(int id);

	void setLaneList(QList<LaneTrailInfo>& list);

	void updateTrailInfoHeight(QVector<double> mCurHeight);


	QString GetLaneName() const { return m_laneName; }
	void SetLaneName(QString val) { m_laneName = val; }
	int GetLoadType() const { return m_nLoadType; }
	void SetLoadType(int val) { m_nLoadType = val; }


	int GetJoinedType() const { return m_nJoinedType; }
	void SetJoinedType(int val) { m_nJoinedType = val; }
	int GetJoinedLaneid() const { return m_nJoinedLaneid; }
	void SetJoinedLaneid(int val) { m_nJoinedLaneid = val; }
	double GetJoinedPointLat() const { return m_dJoinedPointLat; }
	void SetJoinedPointLat(double val) { m_dJoinedPointLat = val; }
	double GetJoinedPointLon() const { return m_dJoinedPointLon; }
	void SetJoinedPointLon(double val) { m_dJoinedPointLon = val; }
	int GetLength() const { return m_nLength; }
	void SetLength(int val) { m_nLength = val; }
	bool GetNoParking() const { return m_bNoParking; }
	void SetNoParking(bool val) { m_bNoParking = val; }
	int GetParkCapacity() const { return m_nParkCapacity; }
	void SetParkCapacity(int val) { m_nParkCapacity = val; }
	int GetSpeedLimit() const { return m_nSpeedLimit; }
	void SetSpeedLimit(int val) { m_nSpeedLimit = val; }
	QString GetAttributes() const { return m_strAttributes; }
	void SetAttributes(QString val) { m_strAttributes = val; }


	int GetGear();
	void SetGear(int val);


	int GetWorkAndTest();
	void SetWorkAndTest(int val);

protected:
	QList<LaneTrailInfo> m_LaneTrailList;
	int m_idLane;
	int m_typeElement;
	int m_idElement;
	int m_idPreNode;
	int m_idSucNode;
	QString m_laneName;

	int		m_nLoadType;
	int		m_nJoinedType;
	int		m_nJoinedLaneid;
	double	m_dJoinedPointLat;
	double	m_dJoinedPointLon;
	int		m_nLength;
	bool	m_bNoParking;
	int		m_nParkCapacity;
	int		m_nSpeedLimit;
	QString m_strAttributes;
	int m_Gear;
	int m_WorkAndTest;
public:
	void clearJoined();
};

#endif