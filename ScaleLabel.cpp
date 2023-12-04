#include "ScaleLabel.h"

ScaleLabel::ScaleLabel(QWidget *parent)
{
    this->setBackgroundRole(QPalette::Base);
    m_scale = 1.0f;
}

ScaleLabel::~ScaleLabel()
{

}

void ScaleLabel::mousePressEvent(QMouseEvent *event)
{
    m_lastPt = event->pos();

}

void ScaleLabel::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->pos().x() - m_lastPt.x();
    int dy = event->pos().y() - m_lastPt.y();

    if(event->buttons() & Qt::LeftButton)
    {
        m_MoveDelta += QPoint(dx,dy);
    }
    update();

    m_lastPt = event->pos();
}

void ScaleLabel::wheelEvent(QWheelEvent *event)
{
    double numDegrees = event->angleDelta().y() / 180.0;
    if(numDegrees>0)
    {
        m_scale *= 1.1;

    }
    else
    {
        m_scale *= 0.9;
    }

#ifdef _DEBUG
    qDebug("m_scale is %.3f",m_scale);
#endif

    this->update();
}

void ScaleLabel::setQImage(QImage &img)
{
    m_showImg = img;
    float h = this->height();
    float w = this->width();
    m_scale = std::min(h / m_showImg.height(),w / m_showImg.width());

#ifdef _DEBUG
    qDebug("h(%.1f),w(%.1f),m_scale(%.3f)",h,w,m_scale);
#endif
}

void ScaleLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    m_painter.begin(this);

    m_painter.scale(m_scale,m_scale);
    m_painter.translate(m_MoveDelta);


    m_painter.drawImage(0,0,m_showImg);



    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(Qt::red);
    m_painter.setPen(pen);

    int w_half = m_showImg.width() / 2;
    int h_half = m_showImg.height() / 2;
    QRect rect(w_half - 50,h_half - 50,100,100);
    m_painter.drawRect(rect);
    m_painter.drawLine(w_half - 50,h_half,0,h_half);
    m_painter.drawLine(w_half + 50,h_half,w_half * 2, h_half);
    m_painter.drawLine(w_half, h_half - 50, w_half,0);
    m_painter.drawLine(w_half, h_half + 50, w_half, h_half*2);

    m_painter.end();

    this->update();



}
