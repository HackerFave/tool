#include "graphicsview.h"
#include <QDebug>
GraphicsView::GraphicsView(QObject *parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GraphicsView::zoomInSlot()//+
{
    if(this->matrix().m11() < 1)
    {
        qreal width = this->size().width() - 2;
        qreal height = this->size().height() - 2;
        qreal scenewidth = this->scene()->sceneRect().width();
        qreal sceneheight = this->scene()->sceneRect().height();

        qreal widthscale = width / scenewidth;
        qreal heightscale = height/sceneheight;

        qreal step;
        if(widthscale <= heightscale)
            step = (1.0 - widthscale) / 10.0;
        else
            step = (1.0 - heightscale) / 10.0;

        //qDebug()<<"step:"<<step;

        QMatrix matrix = this->matrix();
        matrix.setMatrix(this->matrix().m11() + step, 0, 0, this->matrix().m22() + step, 0, 0);
        this->setMatrix(matrix);
    }
}

void GraphicsView::zoomOutSlot()//-
{
    qreal width = this->size().width() - 2;
    qreal height = this->size().height() - 2;
    qreal scenewidth = this->scene()->sceneRect().width();
    qreal sceneheight = this->scene()->sceneRect().height();

    qreal widthscale = width / scenewidth;
    qreal heightscale = height/sceneheight;

    qreal step;
    if(widthscale <= heightscale)
        step = (1.0 - widthscale) / 10.0;
    else
        step = (1.0 - heightscale) / 10.0;
    if(step<0)return;
    QMatrix matrix = this->matrix();
    if(matrix.m11() > step)
    {
        matrix.setMatrix(matrix.m11() - step, 0, 0, this->matrix().m22() - step, 0, 0);
        this->setMatrix(matrix);
    }
    else
    {
        if((widthscale < 1) && (heightscale < 1))
        {
            if(widthscale <= heightscale)
                this->setMatrix(QMatrix(widthscale, 0, 0, widthscale, 0, 0));
            else
                this->setMatrix(QMatrix(heightscale, 0, 0, heightscale, 0, 0));
        }
    }
}
