#include "MyGraphicsView.h"
#include <QDebug>


MyGraphicsView::MyGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    pen.setWidth(2);
    pen.setColor(Qt::green);

    QRectF rect(0,0,1,1);
    rectItem = new QGraphicsRectItem(rect);
    rectItem->setPen(pen);

}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point = event->pos();
    emit mouseMovePoint(point);
    QGraphicsView::mouseMoveEvent(event);
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
}

void MyGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{

    QPointF pt_scene = event->pos();
    //qDebug() << "MyGraphicsView: " << pt_scene;
    emit mouseDoubleClicked_signal(pt_scene);
}




