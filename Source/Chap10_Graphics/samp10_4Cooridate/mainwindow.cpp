#include    "mainwindow.h"
#include    "ui_mainwindow.h"
#include    <QGraphicsEllipseItem>

#include	<QResizeEvent>

//创建Graphics View结构中的各项
void MainWindow::iniGraphicsSystem()
{
	QRectF   rect(-200,-100,400,200);
    scene=new QGraphicsScene(rect,this); //scene逻辑坐标系定义
	ui->view->setScene(scene);      //为视图设置场景

	//画一个矩形框，大小等于scene
	QGraphicsRectItem   *item=new QGraphicsRectItem(rect);  //矩形框正好等于scene的大小

	//    item->setFlag(QGraphicsItem::ItemIsSelectable);         //可选中
	//    item->setFlag(QGraphicsItem::ItemIsFocusable);          //可以获得焦点
	//    item->setFlag(QGraphicsItem::ItemIsMovable,false);      //不能移动
	//可选，可以有焦点，但是不能移动
	item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
	QPen    pen;
	pen.setWidth(2);
	item->setPen(pen);
	//    item->setPos(500,0);//缺省位置在scene的（0,0）
	scene->addItem(item);

	//一个位于scene中心的椭圆，测试局部坐标
	QGraphicsEllipseItem   *item2=new QGraphicsEllipseItem(-100,-50,200,100); //矩形框内创建椭圆,绘图项的局部坐标，左上角(-100,-50)，宽200，高100
	item2->setPos(0,0);     //图形项在场景中的坐标
	item2->setBrush(QBrush(Qt::blue));
	item2->setFlag(QGraphicsItem::ItemIsMovable);      //能移动
	item2->setFlag(QGraphicsItem::ItemIsSelectable);   //可选中
	item2->setFlag(QGraphicsItem::ItemIsFocusable);    //可以获得焦点
	//    item2->setFlags(QGraphicsItem::ItemIsMovable
	//                    | QGraphicsItem::ItemIsSelectable
	//                    | QGraphicsItem::ItemIsFocusable);
	scene->addItem(item2);

	//一个圆，中心位于scene的边缘
	QGraphicsEllipseItem   *item3=new QGraphicsEllipseItem(-50,-50,100,100); //矩形框内创建椭圆,绘图项的局部坐标，左上角(-100,-50)，宽200，高100
	item3->setPos(rect.right(),rect.bottom());        //图形项在场景中的坐标
	item3->setBrush(QBrush(Qt::red));
	item3->setFlag(QGraphicsItem::ItemIsMovable);      //能移动
	item3->setFlag(QGraphicsItem::ItemIsSelectable);   //可选中
	item3->setFlag(QGraphicsItem::ItemIsFocusable);    //可以获得焦点
	//    item3->setFlags(QGraphicsItem::ItemIsMovable
	//                    | QGraphicsItem::ItemIsSelectable
	//                    | QGraphicsItem::ItemIsFocusable);
	scene->addItem(item3);

	scene->clearSelection();
}

//窗口变化大小时的事件
void MainWindow::resizeEvent(QResizeEvent *event)
{
	//    Q_UNUSED(event);
	//Graphics View坐标，左上角总是（0,0），宽度=，长度=
	QString str=QString::asprintf("Graphics View坐标，左上角总是（0,0），宽度=%d，高度=%d",
								  ui->view->width(),ui->view->height());
	ui->labViewSize->setText(str);

	//    ui->labViewSize->setText(QString::asprintf("Graphics View坐标，左上角总是（0,0），宽度=%d，高度=%d",
	//                             ui->view->width(),ui->view->height()));

	QRectF  rectF=ui->view->sceneRect(); //场景的矩形区
	QString str2=QString::asprintf("QGraphicsView::sceneRect=(Left,Top,Width,Height)"
								   "=%.0f,%.0f,%.0f,%.0f", rectF.left(),rectF.top(),rectF.width(),rectF.height());
	ui->labSceneRect->setText(str2);


	//    ui->labSceneRect->setText(QString::asprintf("QGraphicsView::sceneRect=(Left,Top,Width,Height)=%.0f,%.0f,%.0f,%.0f",
	//                              rectF.left(),rectF.top(),rectF.width(),rectF.height()));
	event->accept();
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	//状态栏设计
	labViewCord=new QLabel("View 坐标：", this);
	labViewCord->setMinimumWidth(150);
	ui->statusBar->addWidget(labViewCord);

	labSceneCord=new QLabel("Scene 坐标：", this);
	labSceneCord->setMinimumWidth(150);
	ui->statusBar->addWidget(labSceneCord);

	labItemCord=new QLabel("Item 坐标：", this);
	labItemCord->setMinimumWidth(150);
	ui->statusBar->addWidget(labItemCord);

	ui->view->setCursor(Qt::CrossCursor);   //十字光标
	ui->view->setMouseTracking(true);       //鼠标跟踪
	ui->view->setDragMode(QGraphicsView::RubberBandDrag);   //矩形选择框

	connect(ui->view,SIGNAL(mouseMovePoint(QPoint)),
			this, SLOT(do_mouseMovePoint(QPoint)));
	connect(ui->view,SIGNAL(mouseClicked(QPoint)),
			this, SLOT(do_mouseClicked(QPoint)));

	iniGraphicsSystem();    //Graphics View结构初始化
}

MainWindow::~MainWindow()
{
	delete ui;
}

//鼠标移动，point是 GraphicsView的坐标,物理坐标
void MainWindow::do_mouseMovePoint(QPoint point)
{
	labViewCord->setText(QString::asprintf("View 坐标：%d,%d",point.x(),point.y()));
	QPointF pointScene=ui->view->mapToScene(point);     //转换到Scene坐标
	labSceneCord->setText(QString::asprintf("Scene 坐标：%.0f,%.0f", pointScene.x(),pointScene.y()));
}

//鼠标单击
void MainWindow::do_mouseClicked(QPoint point)
{
	QPointF pointScene=ui->view->mapToScene(point);         //转换到Scene坐标
	QGraphicsItem  *item=NULL;
	item=scene->itemAt(pointScene,ui->view->transform());   //获取光标下的绘图项
	if (item != NULL)   //有绘图项
	{
		QPointF pointItem=item->mapFromScene(pointScene);   //转换为绘图项的局部坐标
		labItemCord->setText(QString::asprintf("Item 坐标：%.0f,%.0f", pointItem.x(),pointItem.y()));
	}
}
