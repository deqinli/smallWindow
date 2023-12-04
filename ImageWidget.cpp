#include "ImageWidget.h"
#include <QApplication>
#include <QDebug>
#include <QtMath>
#include <QPolygonF>

ImageWidget::ImageWidget(QPixmap *pixmap)
{
    m_pix = *pixmap;
    setAcceptDrops(true);//If enabled is true, this item will accept hover events; otherwise, it will ignore them. By default, items do not accept hover events.
    m_scaleValue = 0;
    m_scaleDefault = 0;
    m_isMove = false;
    setCursor(Qt::CrossCursor);
}
QRectF ImageWidget::boundingRect() const
{
    return QRectF(-m_pix.width() / 2, -m_pix.height() / 2,
                  m_pix.width(), m_pix.height());
}

void ImageWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(-m_pix.width() / 2, -m_pix.height() / 2, m_pix);
    this->update();
}

void ImageWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(QApplication::keyboardModifiers()==Qt::ControlModifier)
    {
        if(event->button()== Qt::LeftButton)
            {
                m_startPos = event->pos();//鼠标左击时，获取当前鼠标在图片中的坐标，
                emit mousepress_signal(m_startPos);

            }
    }

    else if(event->button()== Qt::LeftButton)
    {
        m_startPos = event->pos();//鼠标左击时，获取当前鼠标在图片中的坐标，
        m_isMove = true;//标记鼠标左键被按下


        //qDebug() << m_startPos;
    }
    else if(event->button() == Qt::RightButton)
    {
        ResetItemPos();//右击鼠标重置大小
    }


}

void ImageWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF point = mapToScene( event->pos());
    emit mousemove_signal(point);
    if(m_isMove)
    {
        QPointF point = (event->pos() - m_startPos)*m_scaleValue;
        setCursor(Qt::ClosedHandCursor);

        moveBy(point.x(), point.y());

        QPainter painter;
        QPen pen;
        pen.setColor(Qt::green);
        pen.setWidth(10);
        painter.setPen(pen);

        QPoint start(m_startPos.x(),m_startPos.y());
        QPoint end(point.x(),point.y());
        painter.viewport();
        //drawLineWithArrow(painter, pen, start, end);
    }

    itemChanged();

}


void ImageWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    m_isMove = false;//标记鼠标左键已经抬起
    setCursor(Qt::CrossCursor);
}

void ImageWidget::wheelEvent(QGraphicsSceneWheelEvent *event)//鼠标滚轮事件
{
    qDebug() << "m_scaleDefault : "<<m_scaleDefault << ", scale ratio: " << m_scaleValue;
    if((event->delta() > 0)&&(m_scaleValue >= 200))//最大放大到原始图像的200倍
    {
        return;
    }
    else if((event->delta() < 0)&&(m_scaleValue <= m_scaleDefault))//图像缩小到自适应大小之后就不继续缩小
    {
        ResetItemPos();//重置图片大小和位置，使之自适应控件窗口大小
    }
    else
    {
        qreal qrealOriginScale = m_scaleValue;
        if(event->delta() > 0)//鼠标滚轮向前滚动
        {
            m_scaleValue*=1.1;//每次放大10%
        }
        else
        {
            m_scaleValue*=0.9;//每次缩小10%
        }
        setScale(m_scaleValue);
        if(event->delta() > 0)
        {
            moveBy(-event->pos().x()*qrealOriginScale*0.1, -event->pos().y()*qrealOriginScale*0.1);//使图片缩放的效果看起来像是以鼠标所在点为中心进行缩放的
        }
        else
        {
            moveBy(event->pos().x()*qrealOriginScale*0.1, event->pos().y()*qrealOriginScale*0.1);//使图片缩放的效果看起来像是以鼠标所在点为中心进行缩放的
        }
    }

    itemChanged();

}


void ImageWidget::setQGraphicsViewWH(int nwidth, int nheight)//将主界面的控件QGraphicsView的width和height传进本类中，并根据图像的长宽和控件的长宽的比例来使图片缩放到适合控件的大小
{
    int nImgWidth = m_pix.width();
    int nImgHeight = m_pix.height();
    qreal temp1 = nwidth*1.0/nImgWidth;
    qreal temp2 = nheight*1.0/nImgHeight;
    if(temp1>temp2)
    {
        m_scaleDefault = temp2;
    }
    else
    {
        m_scaleDefault = temp1;
    }
    setScale(m_scaleDefault);
    m_scaleValue = m_scaleDefault;
    setPos(0,0);
}

void ImageWidget::ResetItemPos()//重置图片位置
{
    m_scaleValue = m_scaleDefault;//缩放比例回到一开始的自适应比例
    setScale(m_scaleDefault);//缩放到一开始的自适应大小
    setPos(0,0);

    itemChanged();
}

void ImageWidget::itemChanged()
{
    //QPointF point = mapToScene( event->pos());
    QPointF ptImg = QPoint(m_pix.width()/2,m_pix.height()/2);
    QPointF pt_leftTop = mapFromScene(-291,-291) + ptImg;
    QPointF pt_leftBottom = mapFromScene(-291,291) + ptImg;
    QPointF pt_rightTop = mapFromScene(291,-291) + ptImg;
    QPointF pt_rightBottom = mapFromScene(291,291) + ptImg;

    //emit mousemove_signal(pt_leftTop);

    qDebug() << "pt_leftTop: view:(0,0),Scene:(" << pt_leftTop.x() << "," << pt_leftTop.y() << ")";
    qDebug() << "pt_leftBottom: view:(0,321),Scene:(" << pt_leftBottom.x() << "," << pt_leftBottom.y() << ")";
    qDebug() << "pt_rightTop: view:(321,0),Scene:(" << pt_rightTop.x() << "," << pt_rightTop.y() << ")";
    qDebug() << "pt_rightBottom: view:(321,321),Scene:(" << pt_rightBottom.x() << "," << pt_rightBottom.y() << ")";
    qDebug()<<"================================================================";
    emit itemChanged_signal(pt_leftTop, pt_leftBottom, pt_rightTop,  pt_rightBottom);
}

qreal ImageWidget::getScaleValue() const
{
    return m_scaleValue;
}

QImage ImageWidget::Mat2QImage(const cv::Mat &mat)
{
    switch (mat.type())
    {
    case CV_8UC4:
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image;
    }
    case CV_8UC3:
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_8UC1:
    {
        QVector<QRgb> sColorTable;
        if (sColorTable.isEmpty())
        {
            for (int i=0; i < 256; i++)
            {
                sColorTable.push_back(qRgb(i, i, i));
            }
        }
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        image.setColorTable(sColorTable);
        return image;
    }
    default:
        qDebug("Image format is not supported: depth=%d and %d channels\n", mat.depth(), mat.channels());
        break;
    }
    return QImage();
}

QPixmap ImageWidget::Mat2QPixmap(const cv::Mat &mat)
{
    return QPixmap::fromImage(Mat2QImage(mat));
}

cv::Mat ImageWidget::ImageToMat(QImage &image) //QImage转Mat
{
    cv::Mat mat = cv::Mat::zeros(image.height(), image.width(),image.format()); //初始化Mat
    switch(image.format()) //判断image的类型
    {
        case QImage::QImage::Format_Grayscale8:  //灰度图
            mat = cv::Mat(image.height(), image.width(),
            CV_8UC1,(void*)image.constBits(),image.bytesPerLine());
            break;
        case QImage::Format_RGB888: //3通道彩色
            mat = cv::Mat(image.height(), image.width(),
                CV_8UC3,(void*)image.constBits(),image.bytesPerLine());
            break;
        case QImage::Format_ARGB32: //4通道彩色
            mat = cv::Mat(image.height(), image.width(),
                CV_8UC4,(void*)image.constBits(),image.bytesPerLine());
            break;
        default:
            return mat;
    }
    return mat;
}

cv::Mat ImageWidget::QImageToCvMat( const QImage &inImage, bool inCloneImageData )
{
    switch ( inImage.format() )
        {
        // 8-bit, 4 channel
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
        {
            cv::Mat  mat( inImage.height(), inImage.width(),
                          CV_8UC4,
                          const_cast<uchar*>(inImage.bits()),
                          static_cast<size_t>(inImage.bytesPerLine())
                          );

            return (inCloneImageData ? mat.clone() : mat);
        }

        // 8-bit, 3 channel
        case QImage::Format_RGB32:
        case QImage::Format_RGB888:
        {
            if ( !inCloneImageData )
            {
                qDebug() << "CVS::QImageToCvMat() - Conversion requires cloning because we use a temporary QImage";
            }

            QImage   swapped = inImage;

            if ( inImage.format() == QImage::Format_RGB32 )
            {
                swapped = swapped.convertToFormat( QImage::Format_RGB888 );
            }

            swapped = swapped.rgbSwapped();

            return cv::Mat( swapped.height(), swapped.width(),
                            CV_8UC3,
                            const_cast<uchar*>(swapped.bits()),
                            static_cast<size_t>(swapped.bytesPerLine())
                            ).clone();
        }

        // 8-bit, 1 channel
        case QImage::Format_Indexed8:
        {
            cv::Mat  mat( inImage.height(), inImage.width(),
                          CV_8UC1,
                          const_cast<uchar*>(inImage.bits()),
                          static_cast<size_t>(inImage.bytesPerLine())
                          );

            return (inCloneImageData ? mat.clone() : mat);
        }

        default:
            qDebug() << "CVS::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
            break;
        }

        return cv::Mat();
}

void ImageWidget::drawLineWithArrow(QPainter &painter, QPen pen, QPoint pt_start, QPoint pt_end)
{
    qDebug() << "drawLineWithArrow========================================>>";
    painter.setRenderHint(QPainter::Antialiasing,true);

    qreal arrowSize = 20;
    painter.setPen(pen);
    painter.setBrush(pen.color());

    QLine line(pt_end,pt_start);

    double angle = std::atan2(-line.dy(),line.dx());
    QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                          cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                         cos(angle + M_PI - M_PI / 3) * arrowSize);

    QPolygonF arrowHead;
    arrowHead.clear();
    arrowHead << line.p1() << arrowP1 << arrowP2;
    painter.drawLine(line);
    painter.drawPolygon(arrowHead);
}
