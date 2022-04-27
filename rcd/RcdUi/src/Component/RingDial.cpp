#include "RingDial.h"
#include <QRandomGenerator>
#include <QPainter>
#include <QDebug>

#define SPAN_ANGLE      220.0
#define START_ANGLE     -20.0
#define REAL_WIDTH      200.0
#define REAL_HEIGHT     150.0
#define REAL_BLANK      5.0
#define FREQUENCY       100

RingDial::RingDial(QWidget *parent) : QWidget(parent),
    m_type(Speedometer),
    m_currentValue(0),
    m_targetValue(0),
    m_moveSpeed(0),
    m_minValue(0),
    m_maxValue(60)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerHandle()));
}

void RingDial::setParameter(RingDialType type,float minValue,float maxValue)
{
    m_type = type;
    m_minValue = minValue;
    m_maxValue = maxValue;
}

void RingDial::setValue(float value)
{
    if(value > m_maxValue){
        m_targetValue = m_maxValue;
    }else if(value < m_minValue){
        m_targetValue = m_minValue;
    }else{
        m_targetValue = value;
    }

    if(abs(m_targetValue - m_currentValue) < 0.01){
        m_currentValue = m_targetValue;
        this->update();
    }else{
        m_moveSpeed = (m_targetValue - m_currentValue)*2/FREQUENCY;
        m_timer.start(1000/FREQUENCY);
    }
}

float RingDial::calculateAngle()
{
    m_currentValue += m_moveSpeed;

    if(m_moveSpeed >= 0){
        if(m_currentValue >= m_targetValue) {
            m_currentValue = m_targetValue;
            m_timer.stop();
            //setValue(QRandomGenerator::global()->generateDouble()*(m_maxValue-m_minValue)+m_minValue);
            //setValue(m_minValue);
        }
    }else{
        if(m_currentValue <= m_targetValue) {
            m_currentValue = m_targetValue;
            m_timer.stop();
            //setValue(QRandomGenerator::global()->generateDouble()*(m_maxValue-m_minValue)+m_minValue);
            //setValue(m_maxValue);
        }
    }

    return ( (1 - (m_currentValue-m_minValue)/(m_maxValue-m_minValue))*SPAN_ANGLE);
}

void RingDial::timerHandle()
{
    this->update();
}

void RingDial::paintEvent(QPaintEvent *)
{


    QPixmap material(this->width(),this->height());
    material.fill(Qt::transparent);

    QPainter painter(&material);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::LosslessImageRendering);

    painter.translate(material.width()/2, material.height()/2);
    qreal side = qMin(material.width()/(REAL_WIDTH + REAL_BLANK), material.height()/(REAL_HEIGHT + REAL_BLANK)); //缩放
    painter.scale(side , side);
    painter.translate(0, REAL_HEIGHT/4); //原点下移到四分之一处

    painter.setPen(QPen(QColor(0x3d, 0xfe, 0xfe),2));
    QRectF outerRing(REAL_WIDTH/-2, REAL_WIDTH/-2, REAL_WIDTH, REAL_WIDTH); //宽度做外圈直径
    QRectF innerRing(outerRing);
    innerRing -= QMarginsF(25,25,25,25);    //内圈半径小25
    int startAngle = START_ANGLE * 16;
    int spanAngle = SPAN_ANGLE * 16;

    painter.drawPie(outerRing, startAngle, spanAngle);  //画背景
    painter.drawPie(innerRing, startAngle, spanAngle);

    painter.setBrush(QColor(0x3d, 0xfe, 0xfe)); //画当前实际值
    float angle = calculateAngle();
    startAngle = (angle + START_ANGLE) * 16;
    spanAngle = (SPAN_ANGLE - angle) * 16;
    painter.drawPie(outerRing, startAngle, spanAngle);

    painter.setCompositionMode(QPainter::CompositionMode_Clear); //清除多余扇形
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255,0,0));
    innerRing -= QMarginsF(1,1,1,1);
    painter.drawEllipse(innerRing);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPixelSize(20);
    painter.setFont(font);
    if(m_type == Speedometer){
        painter.drawText(-20,20,"km/h");
    }else{
        painter.drawText(-40,20,"x100r/min");
    }
    font.setPixelSize(25);
    painter.setFont(font);
    painter.drawText(-20,0,QString("%1").arg(QString::number(m_currentValue,'.',1)));

    painter.end();

    QPainter mainPainter(this);
    mainPainter.drawPixmap(QPoint(0,0),material);
    mainPainter.end();
}
