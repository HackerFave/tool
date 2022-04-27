#ifndef FRONTPLAYTHREAD_H
#define FRONTPLAYTHREAD_H

#include <QObject>
#include <QThread>
#include <QPoint>
//#include <osg/Array>
//#include <osg/Matrixd>
//#include <osg/Array>
//#include <opencv2/opencv.hpp>
#include <QMutex>
#include <QSemaphore>
#include <QSettings>

//using namespace cv;
class frontPlayThread : public QObject
{
    Q_OBJECT
public:
    explicit frontPlayThread(int w,int h,QString lineId, QObject *parent = nullptr);
    void setAspectRatio(float a);
    void setm_Pitch(float pitch);//倾斜角
    void setaspect_ratio(float ratio);
    void slotm_fai(double);
public slots:
    void slotCalculation();//线程计算

signals:
    void signalPoints(QList<QPointF>, QList<QPointF>);
private:
    bool _isShowLine = true;

    double m_w;
    double m_L;
    double m_D;
    double m_fai;
    double m_alpha;
    double m_cameraHeight;
    double m_beta;
    float aspect_ratio;//宽高比
    float vertical_angle;
    float m_Pitch;
//    osg::Vec3Array* pointlist;
//    osg::Vec3Array* pointlist2;

//    osg::Vec3Array* arrayl = new osg::Vec3Array;
//    osg::Vec3Array* arrayr = new osg::Vec3Array;

    int m_width;
    int m_height;

    QMutex m_mutex;
    QSemaphore m_semaphore;
    QList<QPointF> m_fir;
    QList<QPointF> m_sec;
    QString _lineId;

    QSettings *_config = nullptr;
};

#endif // FRONTPLAYTHREAD_H
