#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QObject>
#include <QApplication>

#include <QGraphicsView>
#include <QGraphicsItem>

#include <QScrollBar>
#include <QPainter>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QObject *parent = 0);
//    void paintEvent(QPaintEvent *event) override;

signals:

public slots:
    void zoomInSlot();
    void zoomOutSlot();
};

#endif // GRAPHICSVIEW_H
