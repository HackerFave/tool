#include "Dashboard.h"
#include <QRandomGenerator>
#include <QPainter>
#include <QDebug>

#define MAX_ANGLE       100.0
#define START_ANGLE     -100.0
#define FREQUENCY       100

Dashboard::Dashboard(QWidget *parent) : QWidget(parent),
    m_currentValue(0),
    m_targetValue(0),
    m_moveSpeed(0),
    m_minValue(0),
    m_maxValue(120)
{
    m_corollaImage.load(":/resource/dashboard/corolla.png");
    m_logoImage.load(":/resource/logo/vlogo.png");
    m_needleImage.load(":/resource/dashboard/needle.png");
    m_name = QString::fromUtf8("电池电压");
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerHandle()));
}

void Dashboard::setParameter(DashboardType type,float minValue,float maxValue)
{
    switch (type) {
        case Volmeter:{
                m_corollaImage.load(":/resource/dashboard/corolla.png");
                m_logoImage.load(":/resource/logo/vlogo.png");
                m_name = QString::fromUtf8("电池电压");
        }break;
        case OilMeter:{
                m_corollaImage.load(":/resource/dashboard/oil_corolla.png");
                m_logoImage.load(":/resource/logo/oil_logo.png");
                m_name = QString::fromUtf8("当前油量");
        }break;
        case OilPressureMeter:{
                m_corollaImage.load(":/resource/dashboard/oil_corolla.png");
                m_logoImage.load(":/resource/logo/oil_logo.png");
                m_name = QString::fromUtf8("发动机油压");
        }break;
        case CoolantTemperature:{
                m_corollaImage.load(":/resource/dashboard/cool_corolla.png");
                m_logoImage.load(":/resource/logo/cool_logo.png");
                m_name = QString::fromUtf8("冷却液温度");
        }break;
        default:
            break;
    }
    m_minValue = minValue;
    m_maxValue = maxValue;
}

void Dashboard::setValue(float value)
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

float Dashboard::calculateAngle()
{
    m_currentValue += m_moveSpeed;

    if(m_moveSpeed >= 0){
        if(m_currentValue >= m_targetValue) {
            m_currentValue = m_targetValue;
            m_timer.stop();
            setValue(QRandomGenerator::global()->generateDouble()*(m_maxValue-m_minValue)+m_minValue);
            //setValue(m_minValue);
        }
    }else{
        if(m_currentValue <= m_targetValue) {
            m_currentValue = m_targetValue;
            m_timer.stop();
            setValue(QRandomGenerator::global()->generateDouble()*(m_maxValue-m_minValue)+m_minValue);
            //setValue(m_maxValue);
        }
    }

    return ((m_currentValue-m_minValue)/(m_maxValue-m_minValue)*(MAX_ANGLE-START_ANGLE) + START_ANGLE);
}

void Dashboard::timerHandle()
{
    this->update();
}

void Dashboard::paintEvent(QPaintEvent *)
{

    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::LosslessImageRendering);

//    painter.setBrush(QColor(0,0x1d,0x33));
//    painter.drawRect(0,0,width(),height());

    painter.translate(width() / 2, height()/2);
    qreal side = qMin(width()/208.0, height()/208.0);
    painter.scale(side , side);
    painter.drawPixmap(m_corollaImage.width()/(-2),m_corollaImage.height()/(-2),m_corollaImage);
    painter.drawPixmap(m_corollaImage.width()/(-4),m_corollaImage.height()/4,m_logoImage);
    painter.setPen(Qt::white);
    painter.drawText(m_corollaImage.width()/(-10),m_corollaImage.height()/3+5,m_name);

    painter.rotate(calculateAngle());
    painter.drawPixmap(m_needleImage.width()/(-2),m_needleImage.height()*(-1)+16,m_needleImage);

    painter.end();
}
