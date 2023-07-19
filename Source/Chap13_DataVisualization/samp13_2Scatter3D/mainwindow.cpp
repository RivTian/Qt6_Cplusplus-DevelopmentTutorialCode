#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QSplitter>
#include    <QInputDialog>
#include    <QColorDialog>
#include    <QMessageBox>

void MainWindow::iniGraph3D()
{
    graph3D = new Q3DScatter();
    graphContainer = QWidget::createWindowContainer(graph3D,this); //Q3DBars继承自QWindow，必须如此创建

    QScatterDataProxy *proxy = new QScatterDataProxy(); //数据代理
    series = new QScatter3DSeries(proxy);  //创建序列
    series->setItemLabelFormat("(@xLabel, @zLabel, @yLabel)");    //散点标签的格式
    series->setMeshSmooth(true);
    series->setBaseColor(Qt::yellow);
    graph3D->addSeries(series);


    //使用内置的坐标轴
    graph3D->axisX()->setTitle("axis X");
    graph3D->axisX()->setLabelFormat("%.2f");
    graph3D->axisX()->setTitleVisible(true);

    graph3D->axisY()->setTitle("axis Y");
    graph3D->axisY()->setLabelFormat("%.2f");
    graph3D->axisY()->setTitleVisible(true);

    graph3D->axisZ()->setTitle("axis Z");
    graph3D->axisZ()->setLabelFormat("%.2f");
    graph3D->axisZ()->setTitleVisible(true);
    graph3D->activeTheme()->setLabelBackgroundEnabled(false);   //不显示轴标签背景

    series->setMesh(QAbstract3DSeries::MeshSphere); //设置散点形状
    series->setItemSize(0.2);   //设置散点大小，默认值为0，取值范围 0~1

    int N=41;
    int itemCount=N*N;
    QScatterDataArray *dataArray = new QScatterDataArray();
    dataArray->resize(itemCount);   //设置数组大小
    QScatterDataItem *ptrToDataArray = &dataArray->first();     //首地址
    //墨西哥草帽,-10:0.5:10， N=41
    float x,y,z;
    x=-10;
    for (int i=1 ; i <=N; i++)
    {
        y=-10;
        for ( int j =1; j <=N; j++)
        {
            z=qSqrt(x*x+y*y);
            if (z!=0)
                z=10*qSin(z)/z;
            else
                z=10;

            ptrToDataArray->setPosition(QVector3D(x,z,y));
            ptrToDataArray++;
            y+=0.5;
        }
        x+=0.5;
    }

    series->dataProxy()->resetArray(dataArray);

    connect(series, &QScatter3DSeries::selectedItemChanged,
            this, &MainWindow::do_itemSelected);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sliderZoom->setRange(10,500);   //设置slider的数据范围
    ui->sliderH->setRange(-180,180);
    ui->sliderV->setRange(-180,180);

    iniGraph3D();       //创建图表
    QSplitter  *splitter=new QSplitter(Qt::Horizontal);
    splitter->addWidget(ui->frameSetup);
    splitter->addWidget(graphContainer);
    this->setCentralWidget(splitter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_itemSelected(int index)
{
    ui->actPoint_ChangeValue->setEnabled(index>=0);
    ui->actData_Delete->setEnabled(index>=0);
    if(index>=0)
    {
        QScatterDataItem item= *(series->dataProxy()->itemAt(index));
        QString str=QString::asprintf("选中点的坐标（x,z,y）=(%.2f, %.2f, %.2f)",
                                      item.x(), item.z(), item.y());
        ui->statusBar->showMessage(str);
    }
}

void MainWindow::on_chkBoxGrid_clicked(bool checked)
{ //图表的网格
    graph3D->activeTheme()->setGridEnabled(checked);
}

void MainWindow::on_chkBoxSmooth_clicked(bool checked)
{ //光滑效果 CheckBox
    series->setMeshSmooth(checked);
}

void MainWindow::on_chkBoxAxisTitle_clicked(bool checked)
{//轴标题
    graph3D->axisX()->setTitleVisible(checked);
    graph3D->axisY()->setTitleVisible(checked);
    graph3D->axisZ()->setTitleVisible(checked);
}

void MainWindow::on_chkBoxAxisBackground_clicked(bool checked)
{//轴标题背景
    graph3D->activeTheme()->setLabelBackgroundEnabled(checked);
}

void MainWindow::on_chkBoxReverse_clicked(bool checked)
{//Z轴反向
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
{ //显示选中散点的标签
    //    series->setItemLabelFormat("value at (@rowLabel,@colLabel): %.1f");
    series->setItemLabelVisible(checked);
}

void MainWindow::on_cBoxBarStyle_currentIndexChanged(int index)
{ //散点形状 ComboBox
    QAbstract3DSeries::Mesh aMesh;
    aMesh=QAbstract3DSeries::Mesh(index+1);
    series->setMesh(aMesh);
}

void MainWindow::on_cBoxSelectionMode_currentIndexChanged(int index)
{//选择模式
    graph3D->setSelectionMode(QAbstract3DGraph::SelectionFlags(index));
}

void MainWindow::on_spinItemSize_valueChanged(double arg1)
{//散点大小
    series->setItemSize(arg1);//default 0. value 0~1
}

void MainWindow::on_chkBoxShadow_clicked(bool checked)
{//显示阴影 CheckBox
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

void MainWindow::on_comboCamera_currentIndexChanged(int index)
{//预设视角
    Q3DCamera::CameraPreset  cameraPos=Q3DCamera::CameraPreset(index);
    graph3D->scene()->activeCamera()->setCameraPreset(cameraPos);
}

void MainWindow::on_actSeries_BaseColor_triggered()
{
    QColor  color=series->baseColor();
    color=QColorDialog::getColor(color);
    if (color.isValid())
        series->setBaseColor(color);
}

void MainWindow::on_actPoint_ChangeValue_triggered()
{//修改当前散点的坐标
    int index=series->selectedItem();       //当前选中点的序号
    if (index<0)
        return;

    QScatterDataItem item =*(series->dataProxy()->itemAt(index));
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

    item.setX(xzy[0].toFloat());    //设置散点坐标
    item.setZ(xzy[1].toFloat());
    item.setY(xzy[2].toFloat());
    series->dataProxy()->setItem(index,item);   //重新设置散点数据项
}

void MainWindow::on_actData_Delete_triggered()
{//删除当前点
    int index=series->selectedItem();   //当前选中点的序号
    if (index<0)
        return;
    int removeCount=1;    //删除点个数
    series->dataProxy()->removeItems(index,removeCount);
}

void MainWindow::on_actData_Add_triggered()
{//添加随机点
    int x=QRandomGenerator::global()->bounded(-10,10);
    int z=QRandomGenerator::global()->bounded(-10,10);
    int y=QRandomGenerator::global()->bounded(5,10);
    QScatterDataItem item;
    item.setX(x);
    item.setY(y);
    item.setZ(z);
    series->dataProxy()->addItem(item);
}

void MainWindow::on_btnResetCamera_clicked()
{//复位到FrontHigh视角
    graph3D->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFrontHigh);
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

void MainWindow::on_btnMoveUp_clicked()
{//上移
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    qreal z=target3D.z();
    target3D.setZ(z-0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}

void MainWindow::on_btnMoveDown_clicked()
{//下移
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    qreal z=target3D.z();
    target3D.setZ(z+0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}
