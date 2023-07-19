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
    graphContainer = QWidget::createWindowContainer(graph3D); //Q3DBars继承自QWindow，必须如此创建
    graph3D->activeTheme()->setLabelBackgroundEnabled(false);

//创建坐标轴
    QValue3DAxis *axisX=new QValue3DAxis;   //X,东西
    axisX->setTitle("AxisX:西--东");
    axisX->setTitleVisible(true);
    axisX->setLabelFormat("%.1f 米");
    axisX->setRange(-5000,5000);
    graph3D->setAxisX(axisX);


    QValue3DAxis *axisY=new QValue3DAxis; //Y，高度
    axisY->setTitle("axisY:高度");
    axisY->setTitleVisible(true);
    axisY->setAutoAdjustRange(true);
//    axisX->setRange(200,500);
//    axisX->setReversed(true);
    graph3D->setAxisY(axisY);

    QValue3DAxis *axisZ=new QValue3DAxis;   //Z, 南北
    axisZ->setTitle("axisZ:南--北");
    axisZ->setLabelFormat("%.1f 米");
    axisZ->setTitleVisible(true);
    axisZ->setRange(-5000,5000);
    graph3D->setAxisZ(axisZ);


//创建数据代理
    QImage heightMapImage(":/map/mountain.png");
//    QImage heightMapImage(":/map/seagray.png");
//    QImage heightMapImage(":/map/sea.png");
    QHeightMapSurfaceDataProxy *proxy = new QHeightMapSurfaceDataProxy(heightMapImage);
    proxy->setValueRanges(-5000, 5000, -5000, 5000);

    series = new QSurface3DSeries(proxy);
    series->setItemLabelFormat("(@xLabel, @zLabel, @yLabel");
    series->setFlatShadingEnabled(false);
    series->setMeshSmooth(true);
    series->setDrawMode(QSurface3DSeries::DrawSurface);

    graph3D->addSeries(series);
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
        return;
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
    case 0:     //None
        graph3D->setSelectionMode(QAbstract3DGraph::SelectionNone);
        break;
    case 1:     //Item
        graph3D->setSelectionMode(QAbstract3DGraph::SelectionItem);
        break;
    case 2:     //Row Slice
        graph3D->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow
                                  | QAbstract3DGraph::SelectionSlice);
        break;
    case 3:     //Column Slice
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
    gr.setColorAt(0.0,  Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0,  Qt::yellow);

    series->setBaseGradient(gr);
    series->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void MainWindow::on_actSurf_GradColor2_triggered()
{//渐变颜色2
    QLinearGradient grGtoR;
    grGtoR.setColorAt(1.0, Qt::red);
//    grGtoR.setColorAt(0.5, Qt::red);
//    grGtoR.setColorAt(0.2, Qt::yellow);
    grGtoR.setColorAt(0.0, Qt::green);

//	grGtoR.setColorAt(1.0, Qt.darkGreen)
//	grGtoR.setColorAt(0.5, Qt.yellow)
//	grGtoR.setColorAt(0.2, Qt.red)
//	grGtoR.setColorAt(0.0, Qt.darkRed)

    series->setBaseGradient(grGtoR);
    series->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

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

