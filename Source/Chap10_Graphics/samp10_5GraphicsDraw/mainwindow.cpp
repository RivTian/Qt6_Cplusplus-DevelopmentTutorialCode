#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QGraphicsRectItem>
#include    <QInputDialog>
#include    <QColorDialog>
#include    <QFontDialog>
#include    <QKeyEvent>
#include    <QDesktopServices>

//函数模板
template<class T> void setBrushColor(T *item)
{
    QColor color=item->brush().color();
    color=QColorDialog::getColor(color,NULL,"选择填充颜色");
    if (color.isValid())
        item->setBrush(QBrush(color));
}

void MainWindow::setItemProperty(QGraphicsItem *item,QString desciption)
{
    item->setFlags(QGraphicsItem::ItemIsMovable         //可移动
                   | QGraphicsItem::ItemIsSelectable    //可选中
                   | QGraphicsItem::ItemIsFocusable);   //可以获得焦点
    item->setZValue(++frontZ);      //叠放顺序号

    quint32 v1=QRandomGenerator::global()->bounded(boundValue);
    quint32 v2=QRandomGenerator::global()->bounded(boundValue);
    item->setPos(v1,v2);    //在场景中的位置

    item->setData(ItemId,++seqNum);             //图形项编号
    item->setData(ItemDesciption,desciption);   //图形项描述

    scene->addItem(item);       //添加到场景
    scene->clearSelection();
    item->setSelected(true);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    labViewCord=new QLabel("View 坐标："); //创建状态栏上的标签
    labViewCord->setMinimumWidth(150);
    ui->statusBar->addWidget(labViewCord);

    labSceneCord=new QLabel("Scene 坐标：");
    labSceneCord->setMinimumWidth(150);
    ui->statusBar->addWidget(labSceneCord);

    labItemCord=new QLabel("Item 坐标：");
    labItemCord->setMinimumWidth(150);
    ui->statusBar->addWidget(labItemCord);

    labItemInfo=new QLabel("ItemInfo: ");
    labItemInfo->setMinimumWidth(200);
    ui->statusBar->addWidget(labItemInfo);

    scene=new QGraphicsScene(-300,-200,600,400);    //创建QGraphicsScene
    //    scene->setBackgroundBrush(QBrush(Qt::white));

    ui->view->setScene(scene);          //scene与view关联
    ui->view->setCursor(Qt::CrossCursor);   //设置鼠标光标
    ui->view->setMouseTracking(true);       //设置鼠标跟踪
    ui->view->setDragMode(QGraphicsView::RubberBandDrag);   //设置拖动模式
    this->setCentralWidget(ui->view);


    connect(ui->view,&TGraphicsView::mouseMovePoint,this, &MainWindow::do_mouseMovePoint);
    connect(ui->view,&TGraphicsView::mouseClicked,this, &MainWindow::do_mouseClicked);
    connect(ui->view,&TGraphicsView::keyPress, this, &MainWindow::do_keyPress);
    connect(ui->view,&TGraphicsView::mouseDoubleClick,this, &MainWindow::do_mouseDoubleClick);


    //	QObject::connect(ui->view,SIGNAL(mouseMovePoint(QPoint)),
    //					 this, SLOT(do_mouseMovePoint(QPoint)));

    //	QObject::connect(ui->view,SIGNAL(mouseClicked(QPoint)),
    //                     this, SLOT(do_mouseClicked(QPoint)));

    //	QObject::connect(ui->view,SIGNAL(mouseDoubleClick(QPoint)),
    //                     this, SLOT(do_mouseDoubleClick(QPoint)));

    //	QObject::connect(ui->view,SIGNAL(keyPress(QKeyEvent*)),
    //                     this, SLOT(do_keyPress(QKeyEvent*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//鼠标移动，point是视图的坐标,物理坐标
void MainWindow::do_mouseMovePoint(QPoint point)
{
    labViewCord->setText(QString::asprintf(
                             "View 坐标：%d,%d", point.x(),point.y()));
    QPointF pointScene=ui->view->mapToScene(point);     //转换到Scene坐标
    labSceneCord->setText(QString::asprintf(
                              "Scene 坐标：%.0f,%.0f", pointScene.x(),pointScene.y()));
}

//鼠标单击事件
void MainWindow::do_mouseClicked(QPoint point)
{
    QPointF pointScene=ui->view->mapToScene(point);     //转换到Scene坐标
    QGraphicsItem  *item=NULL;
    item=scene->itemAt(pointScene,ui->view->transform());   //获取光标下的图形项
    if (item != NULL)       //有图形项
    {
        QPointF pointItem=item->mapFromScene(pointScene); //转换为图形项的局部坐标
        labItemCord->setText(QString::asprintf(
                                 "Item 坐标：%.0f,%.0f",pointItem.x(),pointItem.y()));
        labItemInfo->setText(item->data(ItemDesciption).toString()+", ItemId="+
                             item->data(ItemId).toString());
    }
}

//鼠标双击事件,调用相应的对话框，设置填充颜色、线条颜色或字体
void MainWindow::do_mouseDoubleClick(QPoint point)
{
    QPointF pointScene=ui->view->mapToScene(point);    //转换到Scene坐标
    QGraphicsItem  *item=NULL;
    item=scene->itemAt(pointScene,ui->view->transform());    //获取光标下的图形项

    if (item == NULL)
        return;

    switch (item->type())  //图形项的类型
    {
    case    QGraphicsRectItem::Type:    //矩形框，QGraphicsRectItem的枚举值Type
    {
        QGraphicsRectItem *theItem =qgraphicsitem_cast<QGraphicsRectItem*>(item);
        setBrushColor(theItem);
        break;
    }
    case    QGraphicsEllipseItem::Type: //椭圆或圆
    {
        QGraphicsEllipseItem *theItem =qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
        setBrushColor(theItem);
        break;
    }

    case    QGraphicsPolygonItem::Type: //梯形或三角形
    {
        QGraphicsPolygonItem *theItem=qgraphicsitem_cast<QGraphicsPolygonItem*>(item);
        setBrushColor(theItem);
        break;
    }
    case    QGraphicsLineItem::Type:    //直线，设置线条颜色
    {
        QGraphicsLineItem *theItem=qgraphicsitem_cast<QGraphicsLineItem*>(item);
        QPen    pen=theItem->pen();
        QColor  color=theItem->pen().color();
        color=QColorDialog::getColor(color,this,"选择线条颜色");
        if (color.isValid())
        {
            pen.setColor(color);
            theItem->setPen(pen);
        }
        break;
    }
    case    QGraphicsTextItem::Type:    //文字，设置字体
    {
        QGraphicsTextItem *theItem=qgraphicsitem_cast<QGraphicsTextItem*>(item);
        QFont font=theItem->font();
        bool ok=false;
        font=QFontDialog::getFont(&ok,font,this,"设置字体");
        if (ok)
            theItem->setFont(font);
        break;
    }
    }
}

//按键事件
void MainWindow::do_keyPress(QKeyEvent *event)
{
    if (scene->selectedItems().count()!=1)
        return;     //没有选中的图形项，或选中的多于1个
    QGraphicsItem   *item=scene->selectedItems().at(0);

    if (event->key()==Qt::Key_Delete)       //删除
        scene->removeItem(item);
    else if (event->key()==Qt::Key_Space)   //顺时针旋转90度
        item->setRotation(90+item->rotation());
    else if (event->key()==Qt::Key_PageUp)  //放大
        item->setScale(0.1+item->scale());
    else if (event->key()==Qt::Key_PageDown)//缩小
        item->setScale(-0.1+item->scale());
    else if (event->key()==Qt::Key_Left)    //左移
        item->setX(-1+item->x());
    else if (event->key()==Qt::Key_Right)   //右移
        item->setX(1+item->x());
    else if (event->key()==Qt::Key_Up)      //上移
        item->setY(-1+item->y());
    else if (event->key()==Qt::Key_Down)    //下移
        item->setY(1+item->y());
}

void MainWindow::on_actItem_Rect_triggered()
{ //添加一个矩形
    //x,y 为左上角的图元局部坐标，图元中心点为0,0
    QGraphicsRectItem   *item=new QGraphicsRectItem(-50,-25,100,50);
    item->setBrush(QBrush(Qt::yellow));
    setItemProperty(item, "矩形");
}

void MainWindow::on_actItem_Ellipse_triggered()
{ //添加一个椭圆
    QGraphicsEllipseItem   *item=new QGraphicsEllipseItem(-50,-30,100,60);
    item->setBrush(QBrush(Qt::blue));   //填充颜色
    setItemProperty(item, "椭圆");
}

void MainWindow::on_actItem_Polygon_triggered()
{ //添加一个梯形
    QGraphicsPolygonItem   *item=new QGraphicsPolygonItem;
    QPolygonF   points;
    points.append(QPointF(-40,-40));    //添加顶点坐标
    points.append(QPointF(40,-40));
    points.append(QPointF(100,40));
    points.append(QPointF(-100,40));
    item->setPolygon(points);           //创建多边形

    item->setBrush(QBrush(Qt::green));
    setItemProperty(item,"梯形");
}

void MainWindow::on_actEdit_Delete_triggered()
{ //删除所有选中的图形项
    int cnt=scene->selectedItems().count();
    for (int i=0;i<cnt;i++)
    {
        QGraphicsItem*  item=scene->selectedItems().at(0);
        scene->removeItem(item);    //移除图形项
        delete item;    //删除对象，释放内存
    }
}

void MainWindow::on_actZoomIn_triggered()
{ //放大
    int cnt=scene->selectedItems().count(); //选中图形项的个数
    if (cnt==1) //缩放单个图形项
    {
        QGraphicsItem   *item;
        item=scene->selectedItems().at(0);
        item->setScale(0.1+item->scale());
    }
    else        //缩放视图
        ui->view->scale(1.1, 1.1);
}

void MainWindow::on_actZoomOut_triggered()
{//缩小
    int cnt=scene->selectedItems().count(); //选中图形项的个数
    if (cnt==1) //缩放单个图形项
    {
        QGraphicsItem   *item;
        item=scene->selectedItems().at(0);
        item->setScale(item->scale()-0.1);
    }
    else        //缩放视图
        ui->view->scale(0.9,0.9);
}

void MainWindow::on_actRestore_triggered()
{//取消所有变换
    int cnt=scene->selectedItems().count(); //选中图形项的个数
    if (cnt==1) //针对单个图形项
    {
        QGraphicsItem* item=scene->selectedItems().at(0);
        //        item->resetTransform();   //不起作用
        item->setRotation(0);
        item->setScale(1.0);
    }
    else        //针对视图
        ui->view->resetTransform();
}

void MainWindow::on_actRotateLeft_triggered()
{//逆时针旋转, 做旋转
    int cnt=scene->selectedItems().count();
    if (cnt==1) //单个图形项旋转
    {
        QGraphicsItem* item=scene->selectedItems().at(0);
        item->setRotation(-30+item->rotation());
    }
    else        //视图旋转
        ui->view->rotate(-30);
}

void MainWindow::on_actRotateRight_triggered()
{//顺时针旋转，右旋转
    int cnt=scene->selectedItems().count();
    if (cnt==1) //单个图形项旋转
    {
        QGraphicsItem* item=scene->selectedItems().at(0);
        item->setRotation(+30+item->rotation());
    }
    else        //视图旋转
        ui->view->rotate(+30);
}

void MainWindow::on_actEdit_Front_triggered()
{ //bring to front,前置
    int cnt=scene->selectedItems().count();
    if (cnt>0)
    { //只处理选中的第1个图形项
        QGraphicsItem* item=scene->selectedItems().at(0);
        item->setZValue(++frontZ);
    }
}

void MainWindow::on_actEdit_Back_triggered()
{//bring to back，后置
    int cnt=scene->selectedItems().count();
    if (cnt>0)
    {//只处理选中的第1个图形项
        QGraphicsItem* item=scene->selectedItems().at(0);
        item->setZValue(--backZ);
    }

}

void MainWindow::on_actItem_Line_triggered()
{//添加直线
    QGraphicsLineItem   *item=new QGraphicsLineItem(-100,0,100,0);
    QPen    pen(Qt::red);
    pen.setWidth(3);
    item->setPen(pen);
    setItemProperty(item,"直线");
}

void MainWindow::on_actItem_Text_triggered()
{ //添加文字
    QString str=QInputDialog::getText(this,"输入文字","请输入文字");
    if (str.isEmpty())
        return;

    QGraphicsTextItem   *item=new QGraphicsTextItem(str);
    QFont   font=this->font();
    font.setPointSize(20);
    font.setBold(true);
    item->setFont(font);    //设置字体
    setItemProperty(item,"文字");
}

void MainWindow::on_actGroup_triggered()
{ //组合
    int cnt=scene->selectedItems().count();
    if (cnt>1)
    {
        QGraphicsItemGroup* group =new QGraphicsItemGroup;  //创建组合
        scene->addItem(group);      //添加到场景中

        for (int i=0;i<cnt;i++)     //将选择的图形项添加到组合中
        {
            QGraphicsItem* item=scene->selectedItems().at(0);
            item->setSelected(false);    //取消选择
            item->clearFocus();          //清除焦点状态
            group->addToGroup(item);     //添加到组合
        }
        setItemProperty(group, "组合");   //设置特性
    }
}

void MainWindow::on_actGroupBreak_triggered()
{ //break group,打散组合
    int cnt=scene->selectedItems().count();
    if (cnt==1)
    {
        QGraphicsItemGroup  *group;
        group=(QGraphicsItemGroup*)scene->selectedItems().at(0);
        scene->destroyItemGroup(group); //打散组合
    }
}

void MainWindow::on_actItem_Circle_triggered()
{ //添加圆形
    QGraphicsEllipseItem   *item=new QGraphicsEllipseItem(-50,-50,100,100);
    item->setBrush(QBrush(Qt::cyan));
    setItemProperty(item,"圆形");
}

void MainWindow::on_actItem_Triangle_triggered()
{ //添加三角形
    QGraphicsPolygonItem   *item=new QGraphicsPolygonItem;
    QPolygonF   points;
    points.append(QPointF(0,-40));  //添加顶点坐标
    points.append(QPointF(60,40));
    points.append(QPointF(-60,40));
    //    points.append(QPointF(0,0));  //添加顶点坐标
    //    points.append(QPointF(50,0));
    //    points.append(QPointF(0,50));

    item->setPolygon(points);       //三角形就是一种多边形

    item->setBrush(QBrush(Qt::magenta));
    setItemProperty(item,"三角形");
}


//void MainWindow::on_actBackBrush_triggered()
//{
//    QBrush brush=scene->backgroundBrush();
//    QColor color=brush.color();

//    color=QColorDialog::getColor(color,this,"设置背景颜色");
//    if (color.isValid())
//    {
//        brush.setColor(color);
//        scene->setBackgroundBrush(brush);
//    }
//}


void MainWindow::on_actHelp_triggered()
{
    QString   helpFile=QCoreApplication::applicationDirPath()+"/samp10_5说明文档.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(helpFile));
}

