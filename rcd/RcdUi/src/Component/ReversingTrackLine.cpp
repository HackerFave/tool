#include "ReversingTrackLine.h"
#include <QRandomGenerator>
#include <QPainter>
#include <QDebug>
#include <math.h>

#define PI              3.14159265
//#define SPAN_ANGLE      180.0
//#define START_ANGLE     0.0
//#define REAL_BLANK      0.0
//#define FREQUENCY       100

//#define REAL_WIDTH      800.0
//#define REAL_HEIGHT     600.0

//#define FBDistance      400.0   // 前轮后轮的圆心距离 对应到屏幕的像素长度
//#define WheelDistance   400.0   // 两后轮的距离 对应到屏幕的像素长度

// WheelDistance 绘制始终居中
//
//        全屏幕
//  |       |        |
//  |       |        |
//  |       |        |
//  |    |  |  |     |
//       <Wheel>
//  <    REAL_WIDTH  >

ReversingTrackLine::ReversingTrackLine(QWidget *parent) : QWidget(parent),
    m_currentValue(0),
    m_targetValue(0),
    m_moveSpeed(0)
  //    m_minValue(-40),
  //    m_maxValue(40)
{ 
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerHandle()));
    _config = new QSettings("rcdconfig.ini",QSettings::IniFormat);
    m_minValue =_config->value(QString("Line/MIN_Value")).toFloat();
    m_maxValue = _config->value(QString("Line/MAX_Value")).toFloat();
}

void ReversingTrackLine::setParameter(float minValue,float maxValue)
{
    m_minValue = minValue;
    m_maxValue = maxValue;
}

void ReversingTrackLine::setValue(float value) // 输入前轮和后轮的夹角
{
    if(value > m_maxValue){
        m_targetValue = m_maxValue;
    }else if(value < m_minValue){
        m_targetValue = m_minValue;
    }else if(value < 0.1 && value > -0.1){
        m_targetValue = 0.1;
    }else {
        m_targetValue = value;
    }

    if(abs(m_targetValue - m_currentValue) < 0.01){
        m_currentValue = m_targetValue;
        this->update();
    }else{
        m_moveSpeed = (m_targetValue - m_currentValue)*2/
                (_config->value(QString("Line/FREQUENCY")).toFloat());
        if(_config->value(QString("Line/isTEST")).toBool()){
            m_timer.start(1000/_config->value(QString("Line/FREQUENCY")).toFloat());
        }
    }
}
void ReversingTrackLine::setCurrentValue(float value)
{
    m_currentValue = value;
    this->update();
}
bool ReversingTrackLine::calculateRadius(float &Radius1,float &Radius2)
{
    m_currentValue += m_moveSpeed;
    float tmp = _config->value(QString("Line/FBDistance")).toFloat()/std::tan(m_currentValue * PI/180.0);
    //float tmp = FBDistance*std::cos(m_currentValue * PI/180.0)/std::sin(m_currentValue * PI/180.0);
    Radius1 = (tmp - _config->value(QString("Line/WheelDistance")).toFloat()/2.0);
    Radius2 = (tmp + _config->value(QString("Line/WheelDistance")).toFloat()/2.0);

    if(m_moveSpeed >= 0){
        if(m_currentValue >= m_targetValue) {
            m_currentValue = m_targetValue;
            //m_timer.stop();

            // 调试看效果代码
            setValue(QRandomGenerator::global()->generateDouble()*(m_maxValue-m_minValue)+m_minValue);
            //setValue(m_minValue);
        }
    }else{
        if(m_currentValue <= m_targetValue) {
            m_currentValue = m_targetValue;
            //m_timer.stop();

            // 调试看效果代码
            setValue(QRandomGenerator::global()->generateDouble()*(m_maxValue-m_minValue)+m_minValue);
            //setValue(m_maxValue);
        }
    }

    return true;
}

void ReversingTrackLine::timerHandle()
{
    this->update();
}

void ReversingTrackLine::paintEvent(QPaintEvent *)
{
    QPixmap material(this->width(),this->height());
    material.fill(Qt::transparent);//填充透明色

    QPainter painter(&material);
    //渲染 反锯齿绘制 线性插值算法而不是最邻近算法 使用无损图像渲染
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::LosslessImageRendering);

    QTransform transform;
    //    transform.translate(50, 50);  //坐标沿着x轴前进50，沿着y轴前进50

    transform.translate(material.width()/2+_config->value(QString("Line/SpinWidth")).toFloat(),
                        material.height()/4+_config->value(QString("Line/SpinHeight")).toFloat());//设置逻辑原点
    //    qDebug()<<material.width() << material.height()<<"********************";
    transform.rotate(_config->value(QString("Line/RotateAngle")).toFloat(),Qt::XAxis);//绕X轴旋转
    painter.setTransform(transform);
    qreal side = qMin(material.width()/(_config->value(QString("Line/REAL_WIDTH")).toFloat()
                                        + _config->value(QString("Line/REAL_BLANK")).toFloat()),
                      material.height()/(_config->value(QString("Line/REAL_HEIGHT")).toFloat()
                                         + _config->value(QString("Line/REAL_BLANK")).toFloat())); //缩放 返回最小值
    painter.scale(side , side);//改变刻度尺

    // painter.translate(0, REAL_HEIGHT*3/4.0); //原点下移到四分之一处

    painter.setPen(QPen(QColor(Qt::blue),_config->value(QString("Line/Line_W")).toFloat()));
    float Radius1,Radius2;
    calculateRadius(Radius1,Radius2);

    // 根据半径计算绘制范围
    float letf = -1*(_config->value(QString("Line/WheelDistance")).toFloat()/2 + Radius1*2);
    float top = _config->value(QString("Line/REAL_HEIGHT")).toFloat()/2 - Radius1 ;
    QRectF rect1(letf,top,Radius1*2,Radius1*2);

    // 根据半径计算绘制范围
    letf = _config->value(QString("Line/WheelDistance")).toFloat() /2 - Radius2*2;
    top = _config->value(QString("Line/REAL_HEIGHT")).toFloat()/2 - Radius2;
    QRectF rect2(letf,top,Radius2*2,Radius2*2);
    //    QRectF outerRing(innerRing);
    //    outerRing += QMarginsF(WheelDistance,WheelDistance,WheelDistance,WheelDistance);

    painter.drawArc(rect1, _config->value(QString("Line/START_ANGLE")).toFloat() * 16,
                    _config->value(QString("Line/SPAN_ANGLE")).toFloat() * 16);  //两条圆弧

    painter.drawArc(rect2, _config->value(QString("Line/START_ANGLE")).toFloat() * 16,
                    _config->value(QString("Line/SPAN_ANGLE")).toFloat() * 16);  //两条圆弧

    painter.end();

    QPainter mainPainter(this);
    mainPainter.drawPixmap(QPoint(0,_config->value(QString("Line/End_Point")).toFloat()),
                           material);


    mainPainter.end();


}
