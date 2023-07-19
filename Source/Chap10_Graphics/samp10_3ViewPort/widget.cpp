#include "widget.h"
#include "ui_widget.h"

#include    <QPainter>
#include	<QPaintEvent>

void Widget::myDrawWire()
{
	QPainter    painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	int W=width();
	int H=height();
	int side=qMin(W,H);         //取长和宽的小值
	QRect rect((W-side)/2, (H-side)/2,side,side);   //viewport矩形区
	painter.drawRect(rect);     //绘制Viewport边界
	painter.setViewport(rect);  //设置Viewport
	painter.setWindow(-100,-100,200,200);   //设置窗口坐标系


	//设置画笔
	QPen    pen;
	pen.setWidth(1);        //线宽
	pen.setColor(Qt::red);  //划线颜色
	pen.setStyle(Qt::SolidLine);    //线的类型
	pen.setCapStyle(Qt::FlatCap);   //线端点样式
	pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式
	painter.setPen(pen);

	for(int i=0; i<36; i++)
	{
		painter.drawEllipse(QPoint(50,0),50,50);
		painter.rotate(10);
	}

}

void Widget::myDrawComposite()
{
	QPainter    painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	int W=width();
	int H=height();
	int side=qMin(W,H);         //取长和宽的小值
	QRect rect((W-side)/2, (H-side)/2,side,side);   //viewport矩形区
	painter.drawRect(rect);     //绘制Viewport边界
	painter.setViewport(rect);  //设置Viewport
	painter.setWindow(-100,-100,200,200);   //设置窗口坐标系

	QPen    pen;
	pen.setWidth(1);
	pen.setColor(Qt::red);  //划线颜色
	pen.setStyle(Qt::SolidLine);    //线的类型
	pen.setCapStyle(Qt::FlatCap);   //线端点样式
	pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式
	painter.setPen(pen);    //设置画笔

	//    ////线性渐变
	QLinearGradient  linearGrad(0,0,100,0); //从左到右
	linearGrad.setColorAt(0,Qt::yellow);    //起点颜色
	linearGrad.setColorAt(1,Qt::green);     //终点颜色
	linearGrad.setSpread(QGradient::PadSpread);  //展布模式
	//QGradient::PadSpread ,QGradient::RepeatSpread, QGradient::ReflectSpread
	painter.setBrush(linearGrad);

	//    //径向渐变
	//    //    QRadialGradient  radialGrad(50,0,50,50,0);
	//    ////    radialGrad.setColorAt(0,Qt::white);
	//    //    radialGrad.setColorAt(0,Qt::yellow);
	//    //    radialGrad.setColorAt(1,Qt::blue);
	//    //    radialGrad.setSpread(QGradient::ReflectSpread); //QGradient::PadSpread ,QGradient::RepeatSpread, QGradient::ReflectSpread
	//    //    p.setBrush(radialGrad);

	//设置复合模式
	painter.setCompositionMode(QPainter::CompositionMode_Difference);
//	painter.setCompositionMode(QPainter::RasterOp_NotSourceXorDestination);
	//        painter.setCompositionMode(QPainter::CompositionMode_Exclusion);

	// ,CompositionMode_Xor,RasterOp_NotSourceXorDestination
	// CompositionMode_Difference,CompositionMode_Exclusion,CompositionMode_ColorBurn,CompositionMode_Multiply
	// ,RasterOp_SourceOrNotDestination
	for(int i=0; i<36; i++)
	{
		painter.drawEllipse(QPoint(50,0),50,50);
		painter.rotate(10);
	}

}

void Widget::paintEvent(QPaintEvent *event)
{
//	myDrawWire();   //绘制线网
	myDrawComposite();  //叠加显示
	event->accept();
}

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);
	setPalette(QPalette(Qt::white));
	setAutoFillBackground(true);
	this->resize(300,300);
}

Widget::~Widget()
{
	delete ui;
}
