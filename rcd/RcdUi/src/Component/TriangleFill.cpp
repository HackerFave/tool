#include "TriangleFill.h"
#include <QRandomGenerator>
#include <QPainter>
#include <QDebug>

#define FREQUENCY       100

TriangleFill::TriangleFill(QWidget *parent) : QWidget(parent),
    m_valueColor(0x3d, 0xfe, 0xfe),
    m_type(Throttle),
    m_currentValue(0),
    m_targetValue(0),
    m_moveSpeed(0),
    m_minValue(0),
    m_maxValue(100)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerHandle()));
}

void TriangleFill::setParameter(TriangleFillType type,float minValue,float maxValue)
{
    m_type = type;
    m_minValue = minValue;
    m_maxValue = maxValue;

    if(m_type == Throttle){
        m_valueColor.setRgb(0x3d, 0xfe, 0xfe);
    }else{
        m_valueColor.setRgb(0xfa, 0x5a, 0x00);
    }
}

void TriangleFill::setValue(float value)
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

bool TriangleFill::calculate()
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
    return true;
}

void TriangleFill::timerHandle()
{
    this->update();
}

void TriangleFill::paintEvent(QPaintEvent *)
{
    static const QPoint erasePoints[3] = {
            QPoint(-90, -50),
            QPoint(90, -50),
            QPoint(-90, 50)
        };

    static QColor bgColor(0x44, 0x74, 0x81,200);

    this->calculate();

    QPixmap material(this->width(),this->height());
    material.fill(Qt::transparent);

    QPainter painter(&material);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.translate(material.width() / 2, material.height() / 2);
    qreal side = qMin(material.width()/180.0,material.height()/100.0);
    painter.scale(side , side);

    painter.setPen(Qt::NoPen);
    painter.setBrush(bgColor);
    for(int i=0;i<5;i++){
        painter.drawRect(-40+26*i,-50,15,100);
    }

    float tmpFloat = m_currentValue/20;
    int tmpInt = (int)tmpFloat;
    painter.setBrush(m_valueColor);
    for(int i=0;i<tmpInt;i++){
        painter.drawRect(-40+26*i,-50,15,100);
    }
    painter.drawRect(-40+26*tmpInt,-50,15*(tmpFloat-tmpInt),100);

    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.setPen(Qt::white);
    painter.setBrush(QColor(255,0,0));
    painter.drawConvexPolygon(erasePoints, 3);

    painter.end();

    if(m_type == Brake){
        material = QPixmap::fromImage(material.toImage().mirrored(true,false));
    }

    painter.begin(&material);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.translate(material.width() / 2, material.height() / 2);
    painter.scale(side , side);
    painter.setPen(Qt::white);
    if(m_type == Throttle){
        painter.drawText(-85,20,QString("%1%").arg(QString::number(m_currentValue,'.',1)));
        painter.drawText(-83,45,QString::fromUtf8("油门"));
    }else{
        painter.drawText(45,20,QString("%1%").arg(QString::number(m_currentValue,'.',1)));
        painter.drawText(50,45,QString::fromUtf8("刹车"));
    }
    painter.end();


    QPainter mainPainter(this);
    mainPainter.drawPixmap(QPoint(0,0),material);
    mainPainter.end();

}
