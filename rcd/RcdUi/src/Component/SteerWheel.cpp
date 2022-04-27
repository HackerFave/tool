#include "SteerWheel.h"
#include <QRandomGenerator>
#include <QPainter>
#include <QDebug>

#define MAX_ANGLE       540.0
#define START_ANGLE     -540.0
#define FREQUENCY       100

SteerWheel::SteerWheel(QWidget *parent) : QWidget(parent),
    m_currentValue(0),
    m_targetValue(0),
    m_moveSpeed(0),
    m_minValue(START_ANGLE),
    m_maxValue(MAX_ANGLE)
{
    m_steerWheelImage.load(":/resource/dashboard/steerwheel.png");

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerHandle()));
}

void SteerWheel::setParameter(float minValue,float maxValue)
{
    m_minValue = minValue;
    m_maxValue = maxValue;
}

void SteerWheel::setValue(float value)
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

float SteerWheel::calculateAngle()
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

    return ((m_currentValue-m_minValue)/(m_maxValue-m_minValue)*(MAX_ANGLE-START_ANGLE) + START_ANGLE);
}

void SteerWheel::timerHandle()
{
    this->update();
}

void SteerWheel::paintEvent(QPaintEvent *)
{

    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::LosslessImageRendering);

    painter.translate(width() / 2, height()/2);
    qreal side = qMin(width()/240.0, height()/240.0);
    painter.scale(side , side);

    painter.rotate(calculateAngle());
    painter.drawPixmap(m_steerWheelImage.width()/(-2),m_steerWheelImage.height()/(-2),m_steerWheelImage);

    painter.end();
}
