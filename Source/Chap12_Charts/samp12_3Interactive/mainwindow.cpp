#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    "tchartview.h"

void MainWindow::createChart()
{ //创建图表
    chart = new QChart();
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartView->setCursor(Qt::CrossCursor);  //设置鼠标指针为十字星

    QLineSeries *series0 = new QLineSeries();
    series0->setName("LineSeries曲线");
    series0->setPointsVisible(true);        //显示数据点
    series0->setMarkerSize(4);              //数据点大小
    series0->setSelectedColor(Qt::blue);    //选中点的颜色
    connect(series0,&QLineSeries::clicked, this, &MainWindow::do_series_clicked);
    connect(series0,&QLineSeries::hovered, this, &MainWindow::do_series_hovered);

    QSplineSeries *series1 = new QSplineSeries();
    series1->setName("SplineSeries曲线");
    series1->setPointsVisible(true);
    series1->setMarkerSize(4);
    series1->setSelectedColor(Qt::blue);
    connect(series1,&QSplineSeries::clicked, this, &MainWindow::do_series_clicked);
    connect(series1,&QSplineSeries::hovered, this, &MainWindow::do_series_hovered);

    QPen pen(Qt::black);
    pen.setStyle(Qt::DotLine);
    pen.setWidth(2);
    series0->setPen(pen);

    pen.setStyle(Qt::SolidLine);
    series1->setPen(pen);

    chart->addSeries(series0);
    chart->addSeries(series1);

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 10);         //设置坐标轴范围
    axisX->setLabelFormat("%.1f");  //标签格式
    axisX->setTickCount(11);        //主刻度个数
    axisX->setMinorTickCount(2);
    axisX->setTitleText("time(secs)"); //标题
    //    axisX->setGridLineVisible(false);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-2, 2);
    axisY->setLabelFormat("%.2f");      //标签格式
    axisY->setTickCount(5);
    axisY->setMinorTickCount(2);
    axisY->setTitleText("value");
    //    axisY->setGridLineVisible(false);
//    chart->createDefaultAxes();

    //添加坐标轴
    chart->addAxis(axisX,Qt::AlignBottom); //坐标轴添加到图表，并指定方向
    chart->addAxis(axisY,Qt::AlignLeft);

    series0->attachAxis(axisX); //序列 series0 附加坐标轴
    series0->attachAxis(axisY);

    series1->attachAxis(axisX); //序列 series1 附加坐标轴
    series1->attachAxis(axisY);

    foreach (QLegendMarker* marker, chart->legend()->markers())
        connect(marker, SIGNAL(clicked()), this, SLOT(do_legendMarkerClicked()));
}

void MainWindow::prepareData()
{//为序列生成数据
    QLineSeries *series0=(QLineSeries *)chart->series().at(0);
    QSplineSeries *series1=(QSplineSeries *)chart->series().at(1);
//    series0->clear();
//    series1->clear();

    qreal   t=0,y1,y2,intv=0.5;
    int cnt=21;
    for(int i=0;i<cnt;i++)
    {
        int rd=QRandomGenerator::global()->bounded(-5,6);   //随机整数,[-5, +5]
        y1=qSin(2*t)+rd/50.0;
        series0->append(t,y1);

        rd=QRandomGenerator::global()->bounded(-5,6);   //随机整数,[-5, +5]
        y2=qSin(2*t+20)+rd/50.0;
        series1->append(t,y2);

        t+=intv;
    }
}

int MainWindow::getIndexFromX(QXYSeries *series, qreal xValue, qreal tol)
{
    QList<QPointF> points= series->points();    //返回点的列表
    int index=-1;
    for (int i=0; i<points.count();i++)
    {
        qreal dx=xValue - points.at(i).x();
        if (qAbs(dx)<=tol)
        {
            index =i;
            break;
        }
    }
    return index;   //-1表示没有找到
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->setCentralWidget(ui->chartView);

    lab_chartXY = new QLabel("Chart X=,  Y=  ");
    lab_chartXY->setMinimumWidth(200);
    ui->statusBar->addWidget(lab_chartXY);

    lab_hoverXY = new QLabel("Hovered X=,  Y=  ");
    lab_hoverXY->setMinimumWidth(200);
    ui->statusBar->addWidget(lab_hoverXY);

    lab_clickXY = new QLabel("Clicked X=,  Y=  ");
    lab_clickXY->setMinimumWidth(200);
    ui->statusBar->addWidget(lab_clickXY);

    createChart();  //创建图表
    prepareData();  //生成数据
    connect(ui->chartView,SIGNAL(mouseMovePoint(QPoint)),
                     this, SLOT(do_mouseMovePoint(QPoint)));  //鼠标移动事件
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::do_comboZoomMode_changed(int index)
//{
//    ui->chartView->setCustomZoomRect(index == 4);   //自定义

//    switch(index){
//    case 0:
//        ui->chartView->setRubberBand(QChartView::NoRubberBand);
//        break;

//    case 1: {
////        ui->chartView->setRubberBand(QChartView::VerticalRubberBand | QChartView::ClickThroughRubberBand);    //无法直接设置，Qt bug
//        ui->chartView->setRubberBand(QChartView::VerticalRubberBand);
//        QChartView::RubberBands flags=ui->chartView->rubberBand();
//        flags |= QChartView::ClickThroughRubberBand;    //ClickThroughRubberBand或运算，才能将clicked()信号传递给序列
//        ui->chartView->setRubberBand(flags);
//        break;
//    }

//    case 2: {
//        ui->chartView->setRubberBand(QChartView::HorizontalRubberBand);
//        QChartView::RubberBands flags=ui->chartView->rubberBand();
//        flags |= QChartView::ClickThroughRubberBand;
//        ui->chartView->setRubberBand(flags);
//        break;
//    }

//    case 3:
//    case 4: {
//        ui->chartView->setRubberBand(QChartView::RectangleRubberBand);
//        QChartView::RubberBands flags=ui->chartView->rubberBand();
//        flags |= QChartView::ClickThroughRubberBand;
//        ui->chartView->setRubberBand(flags);
//        break;
//    }
//    }

//}

void MainWindow::do_legendMarkerClicked()
{
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());

    marker->series()->setVisible(!marker->series()->isVisible()); //关联序列的可见性
    marker->setVisible(true);       //图例总是可见的
    qreal alpha = 1.0;
    if (!marker->series()->isVisible())
        alpha = 0.5;                //用半透明表示序列不可见

    QBrush brush = marker->labelBrush();
    QColor color = brush.color();
    color.setAlphaF(alpha);
    brush.setColor(color);
    marker->setLabelBrush(brush);   //设置文字的brush

    brush = marker->brush();
    color = brush.color();
    color.setAlphaF(alpha);
    brush.setColor(color);
    marker->setBrush(brush);        //设置图例标记的brush


    //    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());

    //    switch (marker->type()) //marker的类型
    //    {
    //        case QLegendMarker::LegendMarkerTypeXY:  //QLineSeries序列的图例marker
    //        {
    //            marker->series()->setVisible(!marker->series()->isVisible()); //可见性
    //            marker->setVisible(true);
    //            qreal alpha = 1.0;
    //            if (!marker->series()->isVisible())
    //                alpha = 0.5;        //用半透明表示 invisible

    //            QBrush brush = marker->labelBrush();
    //            QColor color = brush.color();
    //            color.setAlphaF(alpha);
    //            brush.setColor(color);
    //            marker->setLabelBrush(brush);   //设置文字的brush

    //            brush = marker->brush();
    //            color = brush.color();
    //            color.setAlphaF(alpha);
    //            brush.setColor(color);
    //            marker->setBrush(brush);        //设置图例标记的brush

    ////            QPen pen = marker->pen();
    ////            color = pen.color();
    ////            color.setAlphaF(alpha);
    ////            pen.setColor(color);
    ////            marker->setPen(pen);
    //            break;
    //        }
    //        default:
    //            break;
    //    }
}

void MainWindow::do_mouseMovePoint(QPoint point)
{ //鼠标移动响应
    QPointF pt=chart->mapToValue(point);    //转换为图表的数值
    QString str=QString::asprintf("Chart X=%.1f,Y=%.2f",pt.x(),pt.y());
    lab_chartXY->setText(str);  //状态栏显示
}

void MainWindow::do_series_clicked(const QPointF &point)
{
    QString str=QString::asprintf("Clicked X=%.1f,Y=%.2f",point.x(),point.y());
    lab_clickXY->setText(str); //状态栏显示

    QLineSeries *series = qobject_cast<QLineSeries*> (sender());
    int index= getIndexFromX(series, point.x());

    if (index<0)
        return;

    bool isSelected = series->isPointSelected(index);

    series->setPointSelected(index,!isSelected);
//    if (isSelected)
//        series->deselectPoint(index);
//    else
//        series->selectPoint(index);
}

void MainWindow::do_series_hovered(const QPointF &point, bool state)
{
    QString str="Series X=, Y=";
    if (state)
        str=QString::asprintf("Hovered X=%.1f,Y=%.2f",point.x(),point.y());
    lab_hoverXY->setText(str); //状态栏显示

    QLineSeries *series = qobject_cast<QLineSeries*> (sender());
    QPen pen=series->pen();
    if (state)
        pen.setColor(Qt::red);
    else
        pen.setColor(Qt::black);
    series->setPen(pen);
}


void MainWindow::on_actZoomIn_triggered()
{//放大
    ui->chartView->chart()->zoom(1.2);
}

void MainWindow::on_actZoomOut_triggered()
{//缩小
    ui->chartView->chart()->zoom(0.8);
}

void MainWindow::on_actZoomReset_triggered()
{ //恢复原始大小
    ui->chartView->chart()->zoomReset();
}

void MainWindow::on_actDeselectAll_triggered()
{//取消选点
    QXYSeries *series0=qobject_cast<QXYSeries*> (chart->series().at(0));    //都作为QXYSeries来操作，没问题
    QXYSeries *series1=qobject_cast<QXYSeries*> (chart->series().at(1));
    series0->deselectAllPoints();
    series1->deselectAllPoints();
}

void MainWindow::on_comboRubberBand_currentIndexChanged(int index)
{//设置框选方式, rubberBand
    ui->chartView->setCustomZoomRect(index == 4);   //是否自定义模式
//必须有ClickThroughRubberBand，才能将clicked()信号传递给序列
    QFlags<QChartView::RubberBand> flags=QChartView::ClickThroughRubberBand;

    switch(index)
    {
    case 0:
        ui->chartView->setRubberBand(QChartView::NoRubberBand);
        return;
    case 1:
        flags |= QChartView::VerticalRubberBand;    //垂直方向选择
        break;
    case 2:
        flags |= QChartView::HorizontalRubberBand;  //水平方向选择
        break;
    case 3:
    case 4:
        flags |= QChartView::RectangleRubberBand;   //举行框选
    }

    ui->chartView->setRubberBand(flags);

//    switch(index)
//    {
//    case 0:
//        ui->chartView->setRubberBand(QChartView::NoRubberBand);
//        break;

//    case 1: {
////        ui->chartView->setRubberBand(QChartView::VerticalRubberBand | QChartView::ClickThroughRubberBand);    //无法直接设置，Qt bug
//        QFlags<QChartView::RubberBand> flags=QChartView::VerticalRubberBand;
//        flags |= QChartView::ClickThroughRubberBand;    //这样才能将clicked()信号传递给序列
//        ui->chartView->setRubberBand(flags);

////        ui->chartView->setRubberBand(QChartView::VerticalRubberBand);
////        QChartView::RubberBands flags=ui->chartView->rubberBand();
////        flags |= QChartView::ClickThroughRubberBand;    //ClickThroughRubberBand或运算，才能将clicked()信号传递给序列
////        ui->chartView->setRubberBand(flags);
//        break;
//    }

//    case 2: {
//        QFlags<QChartView::RubberBand> flags=QChartView::HorizontalRubberBand;
//        flags |= QChartView::ClickThroughRubberBand;    //这样才能将clicked()信号传递给序列
//        ui->chartView->setRubberBand(flags);

////        ui->chartView->setRubberBand(QChartView::HorizontalRubberBand);
////        QChartView::RubberBands flags=ui->chartView->rubberBand();
////        flags |= QChartView::ClickThroughRubberBand;
////        ui->chartView->setRubberBand(flags);
//        break;
//    }

//    case 3:
//    case 4: {
//        QFlags<QChartView::RubberBand> flags=QChartView::RectangleRubberBand;
//        flags |= QChartView::ClickThroughRubberBand;    //这样才能将clicked()信号传递给序列
//        ui->chartView->setRubberBand(flags);
////        ui->chartView->setRubberBand(QChartView::RectangleRubberBand);
////        QChartView::RubberBands flags=ui->chartView->rubberBand();
////        flags |= QChartView::ClickThroughRubberBand;
////        ui->chartView->setRubberBand(flags);
//        break;
//    }
//    }



}


void MainWindow::on_comboDragMode_currentIndexChanged(int index)
{// 设置拖动模式dragMode
    ui->chartView->setDragMode(QGraphicsView::DragMode(index));

//    switch (index)
//    {
//    case 0:
//        ui->chartView->setDragMode(QGraphicsView::NoDrag);
//        break;
//    case 1:
//        ui->chartView->setDragMode(QGraphicsView::ScrollHandDrag);
//        break;
//    case 2:
//        ui->chartView->setDragMode(QGraphicsView::RubberBandDrag);
//        break;
//    }
}

