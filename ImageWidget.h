#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QPointF>
#include <QMouseEvent>
#include <QRectF>
#include <QDragEnterEvent>
#include <QGraphicsItem>
#include <QGraphicsSceneWheelEvent>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

enum Enum_ZoomState{
    NO_STATE,
    RESET,
    ZOOM_IN,
    ZOOM_OUT
};


class ImageWidget : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    ImageWidget(QPixmap *pixmap);
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem* option,QWidget* widget);
    void wheelEvent(QGraphicsSceneWheelEvent* event);
    void ResetItemPos();
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    qreal getScaleValue() const;
    void setQGraphicsViewWH(int nWidth, int nHeight);
    void itemChanged();
    void drawLineWithArrow(QPainter &painter, QPen pen, QPoint pt_start, QPoint pt_end);

    QPixmap Mat2QPixmap(const cv::Mat &mat);
    QImage Mat2QImage(const cv::Mat &mat);
    cv::Mat ImageToMat(QImage &image);
    cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true );
    QPixmap  m_pix;

private:
    qreal    m_scaleValue;
    qreal    m_scaleDefault;

    int      m_zoomState;
    bool     m_isMove;
    QPointF  m_startPos;

signals:
    void mousemove_signal(QPointF point);
    void mousepress_signal(QPointF point);
    void itemChanged_signal(QPointF pt_leftTop, QPointF pt_leftBottom, QPointF pt_rightTop, QPointF pt_rightBottom);
    void drawLine_signal(QPointF pt_start, QPointF pt_end);

};

#endif // IMAGEWIDGET_H
