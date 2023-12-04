#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QEvent>
#include <QMouseEvent>
#include <QWidget>
#include <QPoint>
#include <QPointF>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPen>
#include <QWheelEvent>


class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:

    MyGraphicsView(QWidget *parent=0);

    int x,y,W,H;
    QPixmap pix;
    QPoint lastPoint;
    QPoint endPoint;

    qreal scale;
    QPixmap tempPix;


    QGraphicsRectItem *rectItem;
    QPen pen;



protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;


signals:
    void mouseMovePoint(QPoint point);
    void mouseClicked(QPoint point);
    void mouseDoubleClicked_signal(QPointF pointf);


};

#endif // MYGRAPHICSVIEW_H
