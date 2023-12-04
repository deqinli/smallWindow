#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MyGraphicsView.h"
#include "ImageWidget.h"
#include <QLabel>
#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private:
    Ui::MainWindow *ui;
    QLabel *labViewCord;
    QLabel *labSceneCord;
    QLabel *labItemCord;
    ImageWidget *m_imageWidget;
    ImageWidget *m_imageWidgetOrg;
    QGraphicsScene *qGraphicsScene;

    QGraphicsRectItem *m_rectItemOrg;
    QGraphicsEllipseItem *m_ellipseOrg;
    QGraphicsLineItem *m_line_H;
    QGraphicsLineItem *m_line_V;
    QGraphicsRectItem *m_viewRect;
    QGraphicsRectItem *m_viewRectInit;
    QGraphicsItemGroup *itemGroup;
    QGraphicsScene *qGraphicsSceneOrg;



    MyGraphicsView *imgView;
    QPixmap pix;
    QPixmap pixOrg;

    int width_GView;
    int height_GView;
    int width_GViewOrg;
    int height_GViewOrg;
    int width_box;
    int height_box;

    void init();
    void setDefault();
    void setConnection();
    void recvShowPicSignal();
    void drawViewRect(QRect rect);
    void init_itemGroup();
    void DrawLineWithArrow(QPainter &painter,QPen pen,QPoint pt_start, QPoint pt_end);
    void getCurrentSquarPoint(QPointF &pt_leftTop, QPointF &pt_leftBottom, QPointF &pt_rightTop, QPointF &pt_rightBottom);

private slots:
    void deal_mouseDoubleClicked(QPointF pointf);
    void deal_mouseMovePoint_slot(QPoint point);
    void deal_wheelChanged_slot(QPointF pt_leftTop, QPointF pt_leftBottom, QPointF pt_rightTop, QPointF pt_rightBottom);
    void deal_mousePress_slot(QPointF pt);
    void on_btnMove_clicked();


};
#endif // MAINWINDOW_H
