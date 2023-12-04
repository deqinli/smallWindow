#ifndef SCALELABEL_H
#define SCALELABEL_H
#include <QLabel>
#include <QImage>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <QWidget>


class ScaleLabel : public QLabel
{
    Q_OBJECT
public:
    ScaleLabel(QWidget *parent=nullptr);
    ~ScaleLabel();

    void setQImage(QImage &img);

public:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

private:
    QImage m_showImg;
    float m_scale;
    QPoint m_lastPt;
    QPoint m_MoveDelta;

    QPainter m_painter;


};

#endif // SCALELABEL_H
