#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    "tpendialog.h"


void MainWindow::createChart()
{//创建图表
    chart = new QChart();
    chart->setTitle(tr("简单函数曲线"));
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    QLineSeries *series0 = new QLineSeries();
    QLineSeries *series1 = new QLineSeries();
    series0->setName("Sin曲线");
    series1->setName("Cos曲线");

    curSeries=series0;  //当前序列

    QPen  pen;
    pen.setStyle(Qt::DotLine);  //Qt::SolidLine, Qt::DashLine, Qt::DotLine, Qt::DashDotLine
    pen.setWidth(2);
    pen.setColor(Qt::red);
    series0->setPen(pen);       //序列series0的线条设置

    pen.setStyle(Qt::SolidLine);//Qt::SolidLine, Qt::DashLine, Qt::DotLine, Qt::DashDotLine
    pen.setColor(Qt::blue);
    series1->setPen(pen);       //序列series1的线条设置

    chart->addSeries(series0);  //将序列添加到图表
    chart->addSeries(series1);

    QValueAxis *axisX = new QValueAxis;
    curAxis=axisX;          //当前坐标轴
    axisX->setRange(0, 10); //设置坐标轴范围
    axisX->setLabelFormat("%.1f");  //标签格式
    axisX->setTickCount(11);        //主刻度个数
    axisX->setMinorTickCount(2);    //次刻度个数
    axisX->setTitleText("time(secs)");  //轴标题
    //    axisX->setGridLineVisible(false);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-2, 2);
    axisY->setLabelFormat("%.2f");  //标签格式
    axisY->setTickCount(5);
    axisY->setMinorTickCount(2);
    axisY->setTitleText("value");
    //    axisY->setGridLineVisible(false);

    //为chart和序列设置坐标轴
    chart->addAxis(axisX,Qt::AlignBottom); //坐标轴添加到图表，并指定方向
    chart->addAxis(axisY,Qt::AlignLeft);

    series0->attachAxis(axisX); //序列series0 附加坐标轴
    series0->attachAxis(axisY);

    series1->attachAxis(axisX); //序列series1 附加坐标轴
    series1->attachAxis(axisY);
}

void MainWindow::prepareData()
{//为序列生成数据
    QLineSeries *series0=static_cast<QLineSeries *>(chart->series().at(0));
    QLineSeries *series1=static_cast<QLineSeries *>(chart->series().at(1));
    series0->clear();   //清除数据
    series1->clear();

    qreal   t=0,y1,y2,intv=0.1;
    int cnt=100;
    for(int i=0;i<cnt;i++)
    {
        int rd =QRandomGenerator::global()->bounded(-5,6);    //随机整数,[-5,+5]
        y1 =qSin(t)+rd/50.0;
        //        *series0<<QPointF(t,y1);  //序列添加数据点，也可以
        series0->append(t,y1);  //序列添加数据点

        rd =QRandomGenerator::global()->bounded(-5,6);    //随机数,[-5,+5]
        y2 =qCos(t)+rd/50.0;
        series1->append(t,y2); //序列添加数据点
        //        *series1<<QPointF(t,y2); //序列添加数据点，也可以

        t +=intv;
    }
}

//从图表上获取数据更新界面显示
void MainWindow::updateFromChart()
{
    QChart *curChart =ui->chartView->chart();  //获取视图组件关联的chart
    ui->editTitle->setText(curChart->title()); //图表标题

    QMargins  mg =curChart->margins();         //图表的边距
    ui->spinMarginLeft->setValue(mg.left());
    ui->spinMarginRight->setValue(mg.right());
    ui->spinMarginTop->setValue(mg.top());
    ui->spinMarginBottom->setValue(mg.bottom());
}

//void MainWindow::mouseMoveEvent(QMouseEvent *event)
//{
////    QPointF  point;
////    point=chart->mapToValue(event->pos());

////    this->labXYValue->setText(QString::asprintf("X=%.2f,Y=%.2f",
////                                point.x(), point.y()));
////    QChartView::mouseMoveEvent(event);

////    m_coordX->setText(QString("X: %1").arg(m_chart->mapToValue(event->pos()).x()));
////    m_coordY->setText(QString("Y: %1").arg(m_chart->mapToValue(event->pos()).y()));
//    //    QGraphicsView::mouseMoveEvent(event);
//}

//bool MainWindow::eventFilter(QObject *watched, QEvent *event)
//{
//    if (watched==ui->chartView)
//        if (event->type()==QEvent::MouseMove)
//        {
//            QMouseEvent *msEvent;
//            msEvent=(QMouseEvent *)event;

//            QPointF  point;
//            point=chart->mapToValue(msEvent->pos());

//            this->labXYValue->setText(QString::asprintf("X=%.2f,Y=%.2f",
//                                        point.x(), point.y()));
//            return true;
////            QChartView::mouseMoveEvent(event);

//        }

//    return QMainWindow::eventFilter(watched,event);
//}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createChart();       //创建图表
    prepareData();       //为序列生成数据
    updateFromChart();   //从图表获得属性值，刷新界面显示

    this->setCentralWidget(ui->splitter);
    //    ui->chartView->setMouseTracking(true);
    //    this->setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::do_LegendMarkerClicked()
//{
//    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());

//    switch (marker->type())
//    {
//        case QLegendMarker::LegendMarkerTypeXY:
//        {
//            marker->series()->setVisible(!marker->series()->isVisible());
//            marker->setVisible(true);
//            qreal alpha = 1.0;
//            if (!marker->series()->isVisible())
//                alpha = 0.5;

//            QColor color;
//            QBrush brush = marker->labelBrush();
//            color = brush.color();
//            color.setAlphaF(alpha);
//            brush.setColor(color);
//            marker->setLabelBrush(brush);

//            brush = marker->brush();
//            color = brush.color();
//            color.setAlphaF(alpha);
//            brush.setColor(color);
//            marker->setBrush(brush);

//            QPen pen = marker->pen();
//            color = pen.color();
//            color.setAlphaF(alpha);
//            pen.setColor(color);
//            marker->setPen(pen);
//            break;
//        }
//        default:
//            break;
//    }


//}

void MainWindow::on_actDraw_triggered()
{ //重新生成数据
    prepareData();//重新生成数据
    //    chart->update();//更新绘图区
}

void MainWindow::on_btnTitleSetText_clicked()
{ //设置图表标题文字
    QString str=ui->editTitle->text();
    chart->setTitle(str);
}

void MainWindow::on_btnTitleFont_clicked()
{ //设置图表标题文字的字体
    QFont font=chart->titleFont();
    bool  ok=false;
    font=QFontDialog::getFont(&ok,font);
    if (ok)
        chart->setTitleFont(font);
}

void MainWindow::on_btnSetMargin_clicked()
{//设置图表的4个边距
    QMargins    mgs;
    mgs.setLeft(ui->spinMarginLeft->value());
    mgs.setRight(ui->spinMarginRight->value());
    mgs.setTop(ui->spinMarginTop->value());
    mgs.setBottom(ui->spinMarginBottom->value());
    chart->setMargins(mgs);
}

//void MainWindow::on_chkBoxPointVisible_clicked(bool checked)
//{//序列的数据点是否可见
//    curSeries->setPointsVisible(checked);
//}

void MainWindow::on_btnSeriesName_clicked()
{ //设置序列名称
    curSeries->setName(ui->editSeriesName->text());
    if (ui->radioSeries0->isChecked())
        ui->radioSeries0->setText(ui->editSeriesName->text());
    else
        ui->radioSeries1->setText(ui->editSeriesName->text());
}

void MainWindow::on_btnSeriesColor_clicked()
{ //设置序列的曲线颜色
    QColor  color=curSeries->color();
    color=QColorDialog::getColor(color);
    if (color.isValid())
        curSeries->setColor(color);
}

void MainWindow::on_btnSetAxisRange_clicked()
{ //设置坐标轴的坐标范围
    curAxis->setRange(ui->spinAxisMin->value(),ui->spinAxisMax->value());
}


void MainWindow::on_spinMinorTickCount_valueChanged(int arg1)
{//次刻度个数
    curAxis->setMinorTickCount(arg1);
}

void MainWindow::on_radioAxisX_clicked()
{ //获取当前坐标轴， X轴
    if (ui->radioAxisX->isChecked())
        curAxis=static_cast<QValueAxis*>(chart->axes(Qt::Horizontal).at(0));    //获取X轴对象
    else
        curAxis=static_cast<QValueAxis*>(chart->axes(Qt::Vertical).at(0));      //获取Y轴对象

    //获取坐标轴的各种属性，显示到界面上
    ui->groupBox_Axis->setChecked(curAxis->isVisible());        //坐标轴可见
    ui->chkBoxAxisReverse->setChecked(curAxis->isReverse());    //坐标轴反向
    ui->spinAxisMin->setValue(curAxis->min());  //坐标轴最小值
    ui->spinAxisMax->setValue(curAxis->max());  //坐标轴最大值

    ui->groupBox_AxisTitle->setChecked(curAxis->isTitleVisible());  //轴标题可见
    ui->editAxisTitle->setText(curAxis->titleText());   //轴标题文字

    ui->groupBox_AxisLabel->setChecked(curAxis->labelsVisible());   //轴刻度标签可见
    ui->editAxisLabelFormat->setText(curAxis->labelFormat());       //标签格式
//主刻度和次刻度
    ui->groupBox_Ticks->setChecked(curAxis->isLineVisible());   //主刻度线可见
    if (curAxis->tickType() == QValueAxis::TicksFixed)          //主刻度类型
        ui->radioTick_Fixed->setChecked(true);      //固定刻度
    else
        ui->radioTick_Dynamic->setChecked(true);    //动态刻度
    ui->spinTick_Anchor->setValue(curAxis->tickAnchor());   //动态刻度起点
    ui->spinTick_Intv->setValue(curAxis->tickInterval());   //动态刻度间隔
    ui->spinTick_Count->setValue(curAxis->tickCount());     //主刻度个数
//主网格线和次网格线
    ui->groupBox_GridLine->setChecked(curAxis->isGridLineVisible()); //主网格线可见
    ui->groupBox_MinorGrid->setChecked(curAxis->isMinorGridLineVisible()); //次网格线可见
    ui->spinMinorTickCount->setValue(curAxis->minorTickCount());    //次刻度个数
}

void MainWindow::on_radioAxisY_clicked()
{//获取当前坐标轴， Y轴
    on_radioAxisX_clicked();
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    curAxis->setLineVisible(checked);
}


//void MainWindow::on_chkGridLineVisible_clicked(bool checked)
//{ //轴的网格线是否可见
//    m_curAxis->setGridLineVisible(checked);
//}

//void MainWindow::on_chkMinorTickVisible_clicked(bool checked)
//{ //次级刻度是否可见
//    m_curAxis->setMinorGridLineVisible(checked);
//}


void MainWindow::on_radioButton_clicked()
{//图例的位置，上
    chart->legend()->setAlignment(Qt::AlignTop);
}

void MainWindow::on_radioButton_2_clicked()
{//图例的位置，下
    chart->legend()->setAlignment(Qt::AlignBottom);
}

void MainWindow::on_radioButton_3_clicked()
{//图例的位置，左
    chart->legend()->setAlignment(Qt::AlignLeft);
}

void MainWindow::on_radioButton_4_clicked()
{//图例的位置，右
    chart->legend()->setAlignment(Qt::AlignRight);
}

void MainWindow::on_btnLegendFont_clicked()
{ //图例的字体设置
    QFont font=chart->legend()->font();
    bool  ok=false;
    font=QFontDialog::getFont(&ok,font);
    if (ok)
        chart->legend()->setFont(font);
}

void MainWindow::on_btnLegendlabelColor_clicked()
{//图例的文字颜色设置
    QColor  color=chart->legend()->labelColor();
    color=QColorDialog::getColor(color);
    if (color.isValid())
        chart->legend()->setLabelColor(color);
}

void MainWindow::on_btnAxisSetTitle_clicked()
{ //设置坐标轴的标题文字
    curAxis->setTitleText(ui->editAxisTitle->text());
}

void MainWindow::on_btnAxisTitleFont_clicked()
{ //设置坐标轴的标题的字体
    QFont   font=curAxis->titleFont();
    bool    ok=false;
    font=QFontDialog::getFont(&ok,font);
    if (ok)
        curAxis->setTitleFont(font);
}

//void MainWindow::on_chkBoxAxisTitle_clicked(bool checked)
//{ //轴标题是否可见
//    m_curAxis->setTitleVisible(checked);
//}

void MainWindow::on_btnAxisLabelFormat_clicked()
{//设置轴刻度标签的文字格式
    curAxis->setLabelFormat(ui->editAxisLabelFormat->text());
}

void MainWindow::on_btnAxisLabelColor_clicked()
{//设置坐标轴刻度标签的文字颜色
    QColor   color=curAxis->labelsColor();
    color=QColorDialog::getColor(color);
    if (color.isValid())
        curAxis->setLabelsColor(color);
}

void MainWindow::on_btnAxisLabelFont_clicked()
{//设置坐标轴刻度标签的文字字体
    QFont   font=curAxis->labelsFont();
    bool    ok=false;
    font=QFontDialog::getFont(&ok,font);
    if (ok)
        curAxis->setLabelsFont(font);
}

//void MainWindow::on_chkBoxLabelsVisible_clicked(bool checked)
//{//轴的刻度标签是否可见
//   m_curAxis->setLabelsVisible(checked);
//}

void MainWindow::on_btnGridLineColor_clicked()
{ //网格线的颜色设置
    QColor   color=curAxis->gridLineColor();
    color=QColorDialog::getColor(color);
    if (color.isValid())
        curAxis->setGridLineColor(color);
}

void MainWindow::on_btnGridLinePen_clicked()
{ //主网格线的Pen设置
    QPen  pen=curAxis->gridLinePen();
    bool  ok=false;
    pen=TPenDialog::getPen(pen, &ok);
    if (ok)
        curAxis->setGridLinePen(pen);
}

//void MainWindow::on_chkAxisLineVisible_clicked(bool checked)
//{//刻度是否可见
//    m_curAxis->setLineVisible(checked);
//}

void MainWindow::on_btnAxisLinePen_clicked()
{
    QPen  pen=curAxis->linePen();
    bool  ok=false;
    pen=TPenDialog::getPen(pen, &ok);

    if (ok)
        curAxis->setLinePen(pen);
}


void MainWindow::on_btnAxisLinePenColor_clicked()
{//主刻度线条颜色
    QColor   color=curAxis->linePenColor();
    color=QColorDialog::getColor(color);
    if (color.isValid())
        curAxis->setLinePenColor(color);
}

void MainWindow::on_btnMinorColor_clicked()
{//次级刻度网格线颜色
    QColor   color=curAxis->minorGridLineColor();

    color=QColorDialog::getColor(color);
    if (color.isValid())
        curAxis->setMinorGridLineColor(color);
}

void MainWindow::on_btnMinorPen_clicked()
{//次级刻度线Pen设置
    QPen  pen=curAxis->minorGridLinePen();

    bool  ok=false;
    pen=TPenDialog::getPen(pen, &ok);

    if (ok)
        curAxis->setMinorGridLinePen(pen);
}

void MainWindow::on_radioSeries0_clicked()
{//获取当前数据序列
    if (ui->radioSeries0->isChecked())
        curSeries=static_cast<QLineSeries *>(chart->series().at(0));
    else
        curSeries=static_cast<QLineSeries *>(chart->series().at(1));
    //获取序列的属性值，并显示到界面上
    ui->editSeriesName->setText(curSeries->name());     //序列名称
    ui->groupBox_Series->setChecked(curSeries->isVisible());     //序列可见
    ui->groupBox_Points->setChecked(curSeries->pointsVisible()); //数据点可见
    ui->chkkBoxUseOpenGL->setChecked(curSeries->useOpenGL());    //使用openGL
    ui->sliderOpacity->setValue(curSeries->opacity()*10);   //透明度
    ui->groupBox_PointLabel->setChecked(curSeries->pointLabelsVisible());   //数据点标签可见
}

void MainWindow::on_radioSeries1_clicked()
{
    on_radioSeries0_clicked();
}


//void MainWindow::on_chkSeriesVisible_clicked(bool checked)
//{//序列是否可见
//    this->m_curSeries->setVisible(checked);
//}

void MainWindow::on_btnSeriesPen_clicked()
{//序列线条的Pen设置
    QPen  pen=curSeries->pen();
    bool  ok=false;
    pen=TPenDialog::getPen(pen, &ok);
    if (ok)
        curSeries->setPen(pen);
}

void MainWindow::on_btnSeriesLabColor_clicked()
{//序列数据点标签颜色
    QColor  color=curSeries->pointLabelsColor();

    color=QColorDialog::getColor(color);
    if (color.isValid())
        curSeries->setPointLabelsColor(color);

}

void MainWindow::on_btnSeriesLabFont_clicked()
{//序列数据点标签字体
    QFont   font;
    font=curSeries->pointLabelsFont();
    bool    ok=false;
    font=QFontDialog::getFont(&ok,font);
    if (ok)
        curSeries->setPointLabelsFont(font);
}

void MainWindow::on_radioSeriesLabFormat0_clicked()
{ //序列数据点标签的显示格式
    curSeries->setPointLabelsFormat("@yPoint");
}

void MainWindow::on_radioSeriesLabFormat1_clicked()
{//序列数据点标签的显示格式
    curSeries->setPointLabelsFormat("(@xPoint,@yPoint)");
}

void MainWindow::on_comboAnimation_currentIndexChanged(int index)
{//动画效果
    chart->setAnimationOptions(QChart::AnimationOptions(index));
    //    chart->setAnimationOptions(QChart::SeriesAnimations);
    //    chart->setAnimationOptions(QChart::AllAnimations);
}


void MainWindow::on_comboTheme_currentIndexChanged(int index)
{ //图表的主题
    chart->setTheme(QChart::ChartTheme(index));
}

void MainWindow::on_actZoomIn_triggered()
{//放大
    chart->zoom(1.2);//zoomIn();
}

void MainWindow::on_actZoomOut_triggered()
{//缩小
    chart->zoom(0.8);//zoomOut();
}

void MainWindow::on_actZoomReset_triggered()
{//复位
    chart->zoomReset();
}

void MainWindow::on_groupBox_Legend_clicked(bool checked)
{//图例是否可见
    chart->legend()->setVisible(checked);
}

void MainWindow::on_groupBox_PointLabel_clicked(bool checked)
{ //是否显示数据点标签
    curSeries->setPointLabelsVisible(checked);
}

void MainWindow::on_groupBox_Series_clicked(bool checked)
{//序列是否可见
    this->curSeries->setVisible(checked);
}

void MainWindow::on_sliderOpacity_sliderMoved(int position)
{//改变序列的透明度
    curSeries->setOpacity(position/10.0);
}

void MainWindow::on_groupBox_GridLine_clicked(bool checked)
{//主网格线可见性
    curAxis->setGridLineVisible(checked);
}

void MainWindow::on_groupBox_MinorGrid_clicked(bool checked)
{//次网格线可见性
    curAxis->setMinorGridLineVisible(checked);
}

void MainWindow::on_groupBox_Axis_clicked(bool checked)
{//坐标轴可见性
    curAxis->setVisible(checked);
}

void MainWindow::on_groupBox_AxisTitle_clicked(bool checked)
{//轴标题的可见性
    curAxis->setTitleVisible(checked);
}

void MainWindow::on_groupBox_AxisLabel_clicked(bool checked)
{//轴刻度标签可见性
    curAxis->setLabelsVisible(checked);
}

void MainWindow::on_groupBox_Ticks_clicked(bool checked)
{//轴线和主刻度的可见性
    curAxis->setLineVisible(checked);
}

void MainWindow::on_btnTitleColor_clicked()
{//设置图表标题文字颜色
    QColor  color=chart->titleBrush().color();
    color=QColorDialog::getColor(color);
    if (color.isValid())
        chart->setTitleBrush(QBrush(color));
}

void MainWindow::on_btnAxisTitleColor_clicked()
{//设置轴标题文字颜色
    QColor  color=this->curAxis->titleBrush().color();
    color=QColorDialog::getColor(color);
    if (color.isValid())
        curAxis->setTitleBrush(QBrush(color));
}

void MainWindow::on_chkBoxAxisReverse_clicked(bool checked)
{//坐标反向
    curAxis->setReverse(checked);
}

void MainWindow::on_chkkBoxUseOpenGL_clicked(bool checked)
{ //设置是否使用OpenGL加速
    curSeries->setUseOpenGL(checked);
}


void MainWindow::on_groupBox_Points_clicked(bool checked)
{//序列的数据点是否可见
    curSeries->setPointsVisible(checked);
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{//设置数据点大小
    curSeries->setMarkerSize(arg1);
}

void MainWindow::on_btnPointColor_clicked()
{//设置数据点填充颜色
    QColor color=QColorDialog::getColor();
    if (color.isValid())
        curSeries->setBrush(QBrush(color));
//    {
//        QBrush brush= curSeries->brush();
//        brush.setColor(color);
//        curSeries->setBrush(brush);
//        curSeries->setBrush(QBrush(color));
//    }
}

void MainWindow::on_spinTick_Count_valueChanged(int arg1)
{//设置主刻度个数，固定刻度时有效
    curAxis->setTickCount(arg1);
}

void MainWindow::on_radioTick_Fixed_toggled(bool checked)
{//“固定刻度”单选按钮复选状态变化，更新组件的enabled属性
    if (checked)
    {
        ui->spinTick_Count->setEnabled(true);
        ui->spinTick_Anchor->setEnabled(false);
        ui->spinTick_Intv->setEnabled(false);
    }
    else
    {
        ui->spinTick_Count->setEnabled(false);
        ui->spinTick_Anchor->setEnabled(true);
        ui->spinTick_Intv->setEnabled(true);
    }
}

void MainWindow::on_radioTick_Dynamic_clicked()
{//"动态刻度"单选按钮
    curAxis->setTickType(QValueAxis::TicksDynamic);
}

void MainWindow::on_spinTick_Anchor_valueChanged(double arg1)
{//设置动态刻度的起点值
    curAxis->setTickAnchor(arg1);
}

void MainWindow::on_spinTick_Intv_valueChanged(double arg1)
{//设置动态刻度的间隔值
    curAxis->setTickInterval(arg1);
}

void MainWindow::on_radioTick_Fixed_clicked()
{//"固定刻度"单选按钮
    curAxis->setTickType(QValueAxis::TicksFixed);
}

