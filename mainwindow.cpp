#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "ScaleLabel.h"
#include <QStatusBar>

#define IMAGE_PATH "../images/sample.jpg"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QImage qImg(IMAGE_PATH);
    pix = QPixmap::fromImage(qImg);
    pixOrg = pix.copy();
    init();
    setDefault();
    setConnection();
    init_itemGroup();
    recvShowPicSignal();



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::deal_mouseDoubleClicked(QPointF pointf)
{
    qDebug()<< "MainWindow: " << pointf;
    //width_box = 40;
    //height_box = 40;
    QPoint pt(pointf.x(),pointf.y());
    QPointF ptf = mapToParent(pt);
    QRectF rect(0,0,40,40);
    QGraphicsRectItem *rectItem = new QGraphicsRectItem(rect);

    QPen pen;
    pen.setWidth(10);
    pen.setColor(Qt::yellow);
    rectItem->setPen(pen);
    //imgScene->addItem(rectItem);

}

void MainWindow::setConnection()
{
    //connect(ui->qGraphicsView,&MyGraphicsView::mouseDoubleClicked_signal,this,&MainWindow::deal_mouseDoubleClicked);
    connect(ui->qGraphicsView,&MyGraphicsView::mouseMovePoint,this,&MainWindow::deal_mouseMovePoint_slot);
    connect(m_imageWidget,&ImageWidget::itemChanged_signal,this,&MainWindow::deal_wheelChanged_slot);
    connect(m_imageWidget,&ImageWidget::mousepress_signal,this,&MainWindow::deal_mousePress_slot);
    connect(ui->btn_Move,SIGNAL(clicked()),this,SLOT(on_btnMove_clicked()));
}

void MainWindow::init()
{
    qGraphicsScene = new QGraphicsScene;

    // 状态栏
    labViewCord=new QLabel("View Coord:");
    labViewCord->setMinimumWidth(150);
    ui->statusBar->addWidget(labViewCord);

    labSceneCord=new QLabel("Scene Coord:");
    labSceneCord->setMinimumWidth(150);
    ui->statusBar->addWidget(labSceneCord);

    labItemCord=new QLabel("item coord:");
    labItemCord->setMinimumWidth(150);
    ui->statusBar->addWidget(labItemCord);

    m_viewRect = NULL;
    itemGroup = new QGraphicsItemGroup;
    // 不可缩放窗口的场景、图元初始化
    qGraphicsSceneOrg = new QGraphicsScene;
    m_rectItemOrg = new QGraphicsRectItem;
    m_ellipseOrg = new QGraphicsEllipseItem;
    m_line_H = new QGraphicsLineItem;
    m_line_V = new QGraphicsLineItem;
}

void MainWindow::setDefault()
{
    ui->qGraphicsView->setDragMode(QGraphicsView::NoDrag);

    ui->qGraphicsView->setStyleSheet("padding: 0px; border: 0px;");//无边框
    ui->qGraphicsView->setMouseTracking(true);//跟踪鼠标位置
    ui->qGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平条
    ui->qGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏竖条
    ui->qGraphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->qGraphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    ui->qGraphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    // 可缩放窗口的指针场景、图元初始化
    qGraphicsScene = new QGraphicsScene;
    m_imageWidget = new ImageWidget(&pix);
    m_imageWidgetOrg = new ImageWidget(&pixOrg);



    ui->qGraphicsViewOrg->setDragMode(QGraphicsView::NoDrag);

    ui->qGraphicsViewOrg->setStyleSheet("padding: 0px; border: 0px;");//无边框
    ui->qGraphicsViewOrg->setMouseTracking(true);//跟踪鼠标位置
    ui->qGraphicsViewOrg->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平条
    ui->qGraphicsViewOrg->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏竖条
    ui->qGraphicsViewOrg->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->qGraphicsViewOrg->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    ui->qGraphicsViewOrg->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);


    //itemGroup->addToGroup(m_rectItemOrg);
    //itemGroup->addToGroup(m_ellipseOrg);
}

void MainWindow::recvShowPicSignal()
{
    width_GView = ui->qGraphicsView->width();
    height_GView = ui->qGraphicsView->height();
    m_imageWidget->setQGraphicsViewWH(width_GView,height_GView);
    qGraphicsScene->addItem(m_imageWidget);

    ui->qGraphicsView->setSceneRect(QRect(-(width_GView/2),(-height_GView/2),width_GView,height_GView));
    ui->qGraphicsView->setScene(qGraphicsScene);



    // 小窗口全景图
    width_GViewOrg = ui->qGraphicsViewOrg->width();
    height_GViewOrg = ui->qGraphicsViewOrg->height();

    QGraphicsPixmapItem *pixItem = qGraphicsSceneOrg->addPixmap(pixOrg);
    //qGraphicsSceneOrg->setSceneRect(QRect(-pix.width()/4,-pix.height()/4,pix.width(),pix.height()));
    qGraphicsSceneOrg->setSceneRect(QRect(0,0,pix.width(),pix.height()));
    ui->qGraphicsViewOrg->setScene(qGraphicsSceneOrg);
    ui->qGraphicsViewOrg->fitInView(pixItem,Qt::KeepAspectRatioByExpanding);


//    width_GViewOrg = ui->qGraphicsViewOrg->width();
//    height_GViewOrg = ui->qGraphicsViewOrg->height();
//    m_imageWidgetOrg->setQGraphicsViewWH(width_GViewOrg,height_GViewOrg);
//    qGraphicsSceneOrg->addItem(m_imageWidgetOrg);

//    ui->qGraphicsViewOrg->setSceneRect(QRect(-(width_GViewOrg/2),(-height_GViewOrg/2),width_GViewOrg,height_GViewOrg));
//    ui->qGraphicsViewOrg->setScene(qGraphicsSceneOrg);

    QRect rect(0,0,pix.width(),pix.height());
    m_rectItemOrg->setRect(rect);
    m_ellipseOrg->setRect(0,0,10,10);


    QPen pen;
    pen.setWidth(10);
    pen.setColor(Qt::yellow);
    m_rectItemOrg->setPen(pen);
    m_ellipseOrg->setPen(pen);
    itemGroup->addToGroup(m_rectItemOrg);
    //itemGroup->addToGroup(m_ellipseOrg);
    itemGroup->addToGroup(m_line_H);
    itemGroup->addToGroup(m_line_V);
    qGraphicsSceneOrg->addItem(itemGroup);


}

void MainWindow::deal_mouseMovePoint_slot(QPoint point)
{
    QPoint pt(pix.width()/2,pix.height()/2);

    labViewCord->setText(QString::asprintf("View coord: %d,%d",point.x(),point.y()));
    QPointF pointScene = ui->qGraphicsView->mapToScene(point);
    labSceneCord->setText(QString::asprintf("Scene coord: %.0f,%.0f",pointScene.x(),pointScene.y()));
    QPointF pointItem = m_imageWidget->mapFromScene(pointScene);
    labItemCord->setText(QString::asprintf("Item coord: %.0f,%.0f",pointItem.x() + pt.x(),pointItem.y() + pt.y()));

    QPoint pt_leftTop_view(0,0);
    QPoint pt_leftBottom_view(0,ui->qGraphicsView->height());
    QPoint pt_rightTop_view(ui->qGraphicsView->width(),0);
    QPoint pt_rightBottom_view(ui->qGraphicsView->width(),ui->qGraphicsView->height());

    QPointF pt_leftTop = ui->qGraphicsView->mapToScene(pt_leftTop_view);
    QPointF pt_leftBottom = ui->qGraphicsView->mapToScene(pt_leftBottom_view);
    QPointF pt_rightTop = ui->qGraphicsView->mapToScene(pt_rightTop_view);
    QPointF pt_rightBottom = ui->qGraphicsView->mapToScene(pt_rightBottom_view);

    int nWidth_rect = abs(pt_rightBottom.x() - pt_leftBottom.x());
    int nHeight_rect = abs(pt_leftBottom.y() - pt_leftTop.y());

    QRect rect(pt_leftTop.x(),pt_leftTop.y(),nWidth_rect,nHeight_rect);
    //drawViewRect(rect);
}

void MainWindow::deal_wheelChanged_slot(QPointF pt_leftTop, QPointF pt_leftBottom, QPointF pt_rightTop, QPointF pt_rightBottom)
{
    QRect rect(pt_leftTop.x(),pt_leftTop.y(),abs(pt_rightTop.x() - pt_leftTop.x()),abs(pt_leftBottom.y()-pt_leftTop.y()));
    QPointF Pt_center;
    Pt_center.setX((pt_leftTop.x() + pt_rightTop.x()) / 2);
    Pt_center.setY((pt_leftTop.y() + pt_leftBottom.y())/ 2);
    //qGraphicsSceneOrg->clear();
    drawViewRect(rect);
}

void MainWindow::deal_mousePress_slot(QPointF pointf)
{
    qDebug()<<"control Press point: " << pointf;
}

void MainWindow::drawViewRect(QRect rect)
{

    qGraphicsSceneOrg->removeItem(m_rectItemOrg);
    //qGraphicsSceneOrg->removeItem(m_ellipseOrg);
    qGraphicsSceneOrg->removeItem(m_line_H);
    qGraphicsSceneOrg->removeItem(m_line_V);
    delete m_rectItemOrg;
//    delete m_ellipseOrg;
    delete m_line_H;
    delete m_line_V;
    QRect rect_center;
    rect_center.setX(rect.x() + rect.width()/2-10);
    rect_center.setY(rect.y() + rect.height()/2-10);
    rect_center.setWidth(20);
    rect_center.setHeight(20);
    QLineF line_H(QPointF(rect.x() + rect.width()/2-40,rect.y() + rect.height()/2),QPointF(rect.x() + rect.width()/2+40,rect.y() + rect.height()/2));
    QLineF line_V(QPointF(rect.x() + rect.width()/2,rect.y() + rect.height()/2-40),QPointF(rect.x() + rect.width()/2,rect.y() + rect.height()/2+40));
    m_rectItemOrg = new QGraphicsRectItem(rect);
    //m_ellipseOrg = new QGraphicsEllipseItem(rect_center);
    m_line_H = new QGraphicsLineItem(line_H);
    m_line_V = new QGraphicsLineItem(line_V);
    QPen pen,pen_red;
    pen.setWidth(10);
    pen_red.setWidth(5);
    pen.setColor(Qt::yellow);
    pen_red.setColor(Qt::red);
    m_rectItemOrg->setPen(pen);
    //m_ellipseOrg->setPen(pen_red);
    m_line_H->setPen(pen_red);
    m_line_V->setPen(pen_red);

    qGraphicsSceneOrg->addItem(m_rectItemOrg);
    //qGraphicsSceneOrg->addItem(m_ellipseOrg);
    qGraphicsSceneOrg->addItem(m_line_H);
    qGraphicsSceneOrg->addItem(m_line_V);
}


void MainWindow::init_itemGroup()
{
    m_viewRectInit = new QGraphicsRectItem;
    itemGroup->addToGroup(m_viewRectInit);
    qGraphicsSceneOrg->addItem(itemGroup);

}


void MainWindow::DrawLineWithArrow(QPainter &painter,QPen pen,QPoint pt_start, QPoint pt_end)
{
    pen.setColor(Qt::green);
    pen.setWidth(10);

}


void MainWindow::on_btnMove_clicked()
{
    int nOffset_x = ui->spinBox_moveX->value();
    int nOffset_y = ui->spinBox_moveY->value();
    double L = sqrt(pow(nOffset_x,2) + pow(nOffset_y,2));
    double Va = 0;
    while(Va < L)
    {
        Va += 1;
        double Vx = nOffset_x / L;
        double Vy = nOffset_y / L;
        m_imageWidget->moveBy(Vx,Vy);
        m_imageWidget->update();
        m_imageWidget->pos();
        cv::waitKey(50);
        QPointF pt_leftTop,pt_leftBottom,pt_rightTop,pt_rightBottom;
        getCurrentSquarPoint(pt_leftTop, pt_leftBottom, pt_rightTop, pt_rightBottom);
        deal_wheelChanged_slot(pt_leftTop, pt_leftBottom, pt_rightTop, pt_rightBottom);
    }

}

void MainWindow::getCurrentSquarPoint(QPointF &pt_leftTop, QPointF &pt_leftBottom, QPointF &pt_rightTop, QPointF &pt_rightBottom)
{
    QPointF ptImg = QPoint(m_imageWidget->m_pix.width()/2,m_imageWidget->m_pix.height()/2);
    pt_leftTop = m_imageWidget->mapFromScene(-291,-291) + ptImg;
    pt_leftBottom = m_imageWidget->mapFromScene(-291,291) + ptImg;
    pt_rightTop = m_imageWidget->mapFromScene(291,-291) + ptImg;
    pt_rightBottom = m_imageWidget->mapFromScene(291,291) + ptImg;
}
