#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QSplitter>

#include    <QColorDialog>
#include    <QFileDialog>
#include    <QInputDialog>
#include    <QMessageBox>

void MainWindow::iniGraph3D()
{
    graph3D = new Q3DSurface();
    graphContainer = QWidget::createWindowContainer(graph3D,this); //Q3DBars继承自QWindow，必须如此创建

    //创建坐标轴
    QValue3DAxis *axisX=new QValue3DAxis;
    axisX->setTitle("Axis X");
    axisX->setTitleVisible(true);
    axisX->setLabelFormat("%.2f");
    axisX->setRange(-11,11);
    //    axisX->setAutoAdjustRange(true);
    graph3D->setAxisX(axisX);

    QValue3DAxis *axisY=new QValue3DAxis;
    axisY->setTitle("Axis Y");
    axisY->setTitleVisible(true);
    axisY->setLabelFormat("%.2f");
    //    axisY->setRange(-10,10);
    axisY->setAutoAdjustRange(true);    //自动调整范围
    graph3D->setAxisY(axisY);

    QValue3DAxis *axisZ=new QValue3DAxis;
    axisZ->setTitle("Axis Z");
    axisZ->setTitleVisible(true);
    axisZ->setLabelFormat("%.2f");
    axisZ->setRange(-11,11);
    //    axisZ->setAutoAdjustRange(true);
    graph3D->setAxisZ(axisZ);

    //创建数据代理
    QSurfaceDataProxy *dataProxy = new QSurfaceDataProxy();
    series = new QSurface3DSeries(dataProxy);
    series->setItemLabelFormat("(@xLabel,@zLabel,@yLabel)");
    series->setMeshSmooth(true);
    series->setBaseColor(Qt::cyan);
    series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    series->setFlatShadingEnabled(false);
    graph3D->addSeries(series);
    graph3D->activeTheme()->setLabelBackgroundEnabled(false);

    //创建数据, 墨西哥草帽
    int N=41;       //-10:0.5:10, N个数据点
    QSurfaceDataArray *dataArray = new QSurfaceDataArray; //数组
    dataArray->reserve(N);

    float x=-10,y,z;
    for (int i =1 ; i <=N; i++)
    {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(N); //一行的数据
        y=-10;
        int index=0;
        for (int j = 1; j <=N; j++)
        {
            z=qSqrt(x*x+y*y);
            if (z!=0)
                z=10*qSin(z)/z;
            else
                z=10;
            (*newRow)[index++].setPosition(QVector3D(x, z, y));
            y += 0.5;
        }
        x += 0.5;
        //        *dataArray << newRow;
        dataArray->append(newRow); //添加一行数据
    }

    dataProxy->resetArray(dataArray);
    connect(series, &QSurface3DSeries::selectedPointChanged,
            this, &MainWindow::do_pointSelected);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sliderZoom->setRange(10,500);   //设置slider的数据范围
    ui->sliderH->setRange(-180,180);
    ui->sliderV->setRange(-180,180);

    iniGraph3D();
    QSplitter   *splitter=new QSplitter(Qt::Horizontal);
    splitter->addWidget(ui->frameSetup);
    splitter->addWidget(graphContainer);
    this->setCentralWidget(splitter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_pointSelected(const QPoint &position)
{
    if((position.x()<0) || (position.y()<0))
    {
        ui->actPoint_Modify->setEnabled(false);
        ui->actPoint_DeleteRow->setEnabled(false);
        return;
    }

    ui->actPoint_Modify->setEnabled(true);
    ui->actPoint_DeleteRow->setEnabled(true);

    QSurfaceDataItem item= *(series->dataProxy()->itemAt(position));
    QString str=QString::asprintf("选中点的坐标（x,z,y）=(%.2f, %.2f, %.2f)",
                                  item.x(), item.z(), item.y());
    ui->statusBar->showMessage(str);
}

void MainWindow::on_chkBoxGrid_clicked(bool checked)
{ //显示背景网格
    graph3D->activeTheme()->setGridEnabled(checked);
}

void MainWindow::on_chkBoxSmooth_clicked(bool checked)
{ //单点光滑效果 CheckBox
    series->setMeshSmooth(checked);
}

void MainWindow::on_chkBoxReflection_clicked(bool checked)
{//反射
    graph3D->setReflection(checked);
}

void MainWindow::on_chkBoxAxisTitle_clicked(bool checked)
{//显示轴标题
    graph3D->axisX()->setTitleVisible(checked);
    graph3D->axisY()->setTitleVisible(checked);
    graph3D->axisZ()->setTitleVisible(checked);
}

void MainWindow::on_chkBoxAxisBackground_clicked(bool checked)
{//轴标题背景
    graph3D->activeTheme()->setLabelBackgroundEnabled(checked);
}

void MainWindow::on_chkBoxReverse_clicked(bool checked)
{//Y轴（垂直）反向
    graph3D->axisY()->setReversed(checked);
}

void MainWindow::on_chkBoxBackground_clicked(bool checked)
{//图表的背景
    graph3D->activeTheme()->setBackgroundEnabled(checked);
}

void MainWindow::on_spinFontSize_valueChanged(int arg1)
{//轴标签字体大小
    QFont font = graph3D->activeTheme()->font();
    font.setPointSize(arg1);
    graph3D->activeTheme()->setFont(font);
}

void MainWindow::on_cBoxTheme_currentIndexChanged(int index)
{//设置主题
    Q3DTheme *currentTheme = graph3D->activeTheme();
    currentTheme->setType(Q3DTheme::Theme(index));
}

void MainWindow::on_chkBoxItemLabel_clicked(bool checked)
{ //显示单点标签 CheckBox
    series->setItemLabelVisible(checked);
}

void MainWindow::on_cBoxBarStyle_currentIndexChanged(int index)
{ //单点形状 ComboBox
    QAbstract3DSeries::Mesh aMesh;
    aMesh=QAbstract3DSeries::Mesh(index+1);
    series->setMesh(aMesh);
}

void MainWindow::on_cBoxSelectionMode_currentIndexChanged(int index)
{//选择模式
    switch(index)
    {
    case 0:
        graph3D->setSelectionMode(QAbstract3DGraph::SelectionNone);
        break;
    case 1:
        graph3D->setSelectionMode(QAbstract3DGraph::SelectionItem);
        break;
    case 2:
        graph3D->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow
                                  | QAbstract3DGraph::SelectionSlice);
        break;
    case 3:
        graph3D->setSelectionMode(QAbstract3DGraph::SelectionItemAndColumn
                                  | QAbstract3DGraph::SelectionSlice);
    default:
        break;
    }
}


void MainWindow::on_chkBoxShadow_clicked(bool checked)
{//显示阴影
    if (checked)
        graph3D->setShadowQuality(QAbstract3DGraph::ShadowQualityMedium);
    else
        graph3D->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
}

void MainWindow::on_sliderH_valueChanged(int value)
{//水平旋转
    graph3D->scene()->activeCamera()->setXRotation(value);
}

void MainWindow::on_sliderV_valueChanged(int value)
{//垂直旋转
    graph3D->scene()->activeCamera()->setYRotation(value);
}

void MainWindow::on_sliderZoom_valueChanged(int value)
{//缩放
    graph3D->scene()->activeCamera()->setZoomLevel(value);
}

void MainWindow::on_comboDrawMode_currentIndexChanged(int index)
{//曲面样式 ComboBox
    if (index==0)
        series->setDrawMode(QSurface3DSeries::DrawWireframe);
    else if (index==1)
        series->setDrawMode(QSurface3DSeries::DrawSurface);
    else
        series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
}

void MainWindow::on_comboCamera_currentIndexChanged(int index)
{
    graph3D->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPreset(index));
}

void MainWindow::on_chkBoxFlatShading_clicked(bool checked)
{//平面着色 CheckBox
    series->setFlatShadingEnabled(checked);
}

void MainWindow::on_actSurf_Color_triggered()
{//设置曲面颜色
    QColor  color=series->baseColor();
    color=QColorDialog::getColor(color);
    if (color.isValid())
    {
        series->setBaseColor(color);
        series->setColorStyle(Q3DTheme::ColorStyleUniform); //单一颜色
    }
}

void MainWindow::on_actSurf_GradColor1_triggered()
{//渐变颜色1
    QLinearGradient gr;
    gr.setColorAt(0.0,  Qt::blue);
    //    gr.setColorAt(0.33, Qt::blue);
    //    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0,  Qt::yellow);

    //    gr.setColorAt(0.0,  Qt::black);
    //    gr.setColorAt(0.33, Qt::blue);
    //    gr.setColorAt(0.67, Qt::red);
    //    gr.setColorAt(1.0,  Qt::yellow);

    series->setBaseGradient(gr);
    series->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void MainWindow::on_actSurf_GradColor2_triggered()
{//渐变颜色2
    QLinearGradient grGtoR;
    //    grGtoR.setColorAt(0.5, Qt::yellow);
    //    grGtoR.setColorAt(0.2, Qt::red);
    grGtoR.setColorAt(0.0, Qt::cyan);
    grGtoR.setColorAt(1.0, Qt::red);

    //    grGtoR.setColorAt(1.0, Qt::darkGreen);
    //    grGtoR.setColorAt(0.5, Qt::yellow);
    //    grGtoR.setColorAt(0.2, Qt::red);
    //    grGtoR.setColorAt(0.0, Qt::darkRed);


    series->setBaseGradient(grGtoR);
    series->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void MainWindow::on_actPoint_Modify_triggered()
{//修改点坐标
    QPoint point=series->selectedPoint();
    if ((point.x()<0) || point.y()<0)
        return;

    QSurfaceDataItem item =*(series->dataProxy()->itemAt(point));
    QString coord=QString::asprintf("%.2f, %.2f, %.2f",item.x(),item.z(),item.y());
    bool ok=false;
    QString newText = QInputDialog::getText(this,"修改点坐标",
                         "按格式输入点的坐标（x,z,y）",QLineEdit::Normal,coord,&ok);
    if (!ok)
        return;
    newText =newText.simplified();	//去除前后和中间的空格
    QStringList xzy=newText.split(QLatin1Char(','),Qt::SkipEmptyParts);	//按逗号分割
    if(xzy.size() != 3)
    {
        QMessageBox::critical(this,"错误","输入坐标数据格式错误");
        return;
    }

    item.setX(xzy[0].toFloat());
    item.setZ(xzy[1].toFloat());
    item.setY(xzy[2].toFloat());
    series->dataProxy()->setItem(point,item);
}

void MainWindow::on_actPoint_DeleteRow_triggered()
{//删除行
    QPoint point=series->selectedPoint();
    if ((point.x()<0) || point.y()<0)
        return;
    int removeCount=1;    //删除行的个数
    series->dataProxy()->removeRows(point.x(),removeCount);
}


void MainWindow::on_btnResetCamera_clicked()
{//复位到FrontHigh视角
    graph3D->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFrontHigh);
}


void MainWindow::on_btnMoveUp_clicked()
{//上移
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    qreal z=target3D.z();
    target3D.setZ(z-0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}


void MainWindow::on_btnMoveLeft_clicked()
{//左移
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    qreal x=target3D.x();
    target3D.setX(x+0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}


void MainWindow::on_btnMoveRight_clicked()
{//右移
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    qreal x=target3D.x();
    target3D.setX(x-0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}

void MainWindow::on_btnMoveDown_clicked()
{//下移
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    qreal z=target3D.z();
    target3D.setZ(z+0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}

