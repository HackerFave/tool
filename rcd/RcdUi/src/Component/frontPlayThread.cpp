#include "frontPlayThread.h"
#include "QVector2D"
#include <QDebug>

//using namespace cv;
using namespace std;
const double PI = 3.14159265358979323846;
frontPlayThread::frontPlayThread(int w,int h, QString lineId, QObject *parent) :
    QObject(parent),m_width(w),m_height(h),_lineId(lineId)
{

    _config = new QSettings("rcdconfig.ini",QSettings::IniFormat);

    m_fai = 0.001f;
    //后轮轴长
    m_w = 2.526;
    //前后轮轴距
    m_L = 4.575;
    //后轮距车位距离
    m_D = 2.479;
    //    m_alpha = 49.1
    m_alpha = PI / 180  *49.1;
//    m_cameraHeight = _config->value(QString("%1/m_cameraHeight").arg(_lineId+"Line")).toFloat();
    m_beta = PI / 180 *20;
//    aspect_ratio = 640.0 /240.0;
    aspect_ratio = 640.0/480.0 *_config->value(QString("%1/Resolution").arg(_lineId+"Line")).toFloat();
    vertical_angle = 98.2f ;
    setAspectRatio(aspect_ratio);
    pointlist = new osg::Vec3Array;
    pointlist2 = new osg::Vec3Array;

    arrayl = new osg::Vec3Array;
    arrayr = new osg::Vec3Array;
    m_Pitch = 0.0;

}
void frontPlayThread::slotCalculation()
{
    osg::Matrixd viewMatrix = osg::Matrixd(1, 0, 0, 0,
                                           0, -0.85364941791658211, -0.52084803089834220, 0,
                                           0, 0.52084803089834220, -0.85364941791658211, 0,
                                           -0.023503065109252930, -0.010859527659907347, -3.2332406426414462, 1.0000000000000000);

    osg::Matrixd projMatrix;

    projMatrix.makePerspective(vertical_angle, aspect_ratio, 0.028f, 10.0f);
    //
    osg::Matrixd windowViewMatrix = osg::Matrixd(m_width / 2.0 +_config->value(QString("%1/SpinWidth")
                                                                               .arg(_lineId+"Line")).toFloat()
                                                 , 0, 0, 0,
                                                 0, m_height / 2+_config->value(QString("%1/SpinHeight")
                                                                                .arg(_lineId+"Line")).toFloat()
                                                 , 0, 0,
                                                 0, 0, 0.5, 0,
                                                 m_width / 2 +_config->value(QString("%1/MarginWidth")
                                                                             .arg(_lineId+"Line")).toFloat()
                                                 , m_height / 2 +_config->value(QString("%1/MarginHeight")
                                                                                .arg(_lineId+"Line")).toFloat(),
                                                 0.5, 1.0
                                                 );

    osg::Matrixd matrix;
    matrix = viewMatrix * projMatrix * windowViewMatrix;
    int n = 11;

    double temp1 = 0;
    double temp2 = 0;
    QPointF pos;

    osg::Vec3 position(0.0,0.0,0.0);
    osg::Vec3 scale(1.0,1.0,1.0);

    while (1) {
        m_semaphore.acquire();

        double tanfai = tan(m_fai * PI / 180.0f);
        double r = m_L / tanfai - m_w / 2;
        double r2 = m_L / tanfai + m_w / 2;
        int huchang = 5;
        double hudu1 = huchang / r;
        double hudu2 = huchang / r2;


        pointlist->clear();
        pointlist2->clear();
        double temphudu1 = hudu1 / (n - 1);
        double temphudu2 = hudu2 / (n - 1);


        osg::Vec3 vecpos;
        for (int i = 0; i < n; i++)
        {
            if (m_fai < 0)
            {
                temp1 = temphudu1 * i;
                temp2 = temphudu2 * i;
            }
            else {
                temp1 = temphudu1 * i;
                temp2 = temphudu2 * i;
            }
            vecpos = osg::Vec3(r * cos(temp1) - m_L / tanfai, 0.0f, r * sin(temp1) - m_D);
            pointlist->push_back(vecpos);
            vecpos = osg::Vec3(r2 * cos(temp2) - m_L / tanfai, 0.0f, r2 * sin(temp2) - m_D);

            pointlist2->push_back(vecpos);
        }

        arrayl->clear();
        arrayr->clear();
        m_fir.clear();
        m_sec.clear();
        osg::Quat quat(_config->value(QString("%1/m_Pitch")
                                      .arg(_lineId+"Line")).toFloat(),
                       osg::Vec3d(1.0, 0.0, 0.0));
         osg::Matrixd modelMatrix;
        osg::Vec3f left0 = pointlist->at(0);
        osg::Vec3f right0 = pointlist->at(0);
        position = (left0 + right0) / 2.0;

        modelMatrix.makeRotate(quat);
        modelMatrix.postMultTranslate(position);
        modelMatrix.preMultScale(scale);
        modelMatrix.preMultTranslate(-position);

        for (int i = 0; i < pointlist->size(); i++)
        {
            arrayl->push_back(pointlist->at(i) * modelMatrix * matrix);
            arrayr->push_back(pointlist2->at(i) * modelMatrix * matrix);
            pos.setX(arrayl->at(i).x());
            pos.setY(arrayl->at(i).y());

            m_fir.push_back(QPointF(-(pos.x() - m_width), -(pos.y() - m_height)));

            pos.setX(arrayr->at(i).x());
            pos.setY(arrayr->at(i).y());
            m_sec.push_back(QPointF(-(pos.x() - m_width), -(pos.y() - m_height)));

        }

        emit signalPoints(m_fir, m_sec);
    };
}
void frontPlayThread::setAspectRatio(float a)
{
    if(a>0||a*vertical_angle<180)
        aspect_ratio=a;
    else
    {
        qDebug()<<"Error: fail to set Aspect Angle";
        qDebug()<<"Reason: value you set <= 0 or (Vertical Angle * it >= 180_";
    }
    return;
}

void frontPlayThread::slotm_fai(double value)
{
    m_fai = value;
    m_semaphore.release();
}
void frontPlayThread::setm_Pitch(float pitch)
{
    m_Pitch = pitch;
}
void frontPlayThread::setaspect_ratio(float ratio)
{
    aspect_ratio =ratio;
    qDebug()<<aspect_ratio<<"aspect_ratio ------------------";
}
