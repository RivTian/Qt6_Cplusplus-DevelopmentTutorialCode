#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QSplitter>

#include    <QColorDialog>
#include    <QInputDialog>
#include    <QVector3D>

void MainWindow::iniGraph3D()
{
    graph3D = new Q3DBars();
    graphContainer = QWidget::createWindowContainer(graph3D, this);     //Q3DBars继承自QWindow
    graph3D->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFrontHigh); //设置视角

    //创建坐标轴
    QValue3DAxis *axisV=new QValue3DAxis;   //数值坐标
    axisV->setTitle("销量");
    axisV->setTitleVisible(true);
    axisV->setLabelFormat("%d");
    graph3D->setValueAxis(axisV);   //设置数值坐标轴

    QCategory3DAxis *axisRow=new QCategory3DAxis;
    axisRow->setTitle("row axis");
//    QStringList rowLabs;    //行坐标标签
//    rowLabs << "Week1" << "Week2"<<"Week3";
//    axisRow->setLabels(rowLabs);
    axisRow->setTitleVisible(true);
    graph3D->setRowAxis(axisRow);   //设置行坐标轴

    QCategory3DAxis *axisCol=new QCategory3DAxis;
    axisCol->setTitle("column axis");
//    QStringList colLabs;    //列坐标标签
//    colLabs << "Mon" << "Tue" << "Wed" << "Thur"<<"Fri";
//    axisCol->setLabels(colLabs);
    axisCol->setTitleVisible(true);
    graph3D->setColumnAxis(axisCol);    //设置列坐标轴

    //创建序列
    series = new QBar3DSeries;
    series->setMesh(QAbstract3DSeries::MeshCylinder);           //棒柱形状
    series->setItemLabelFormat("(@rowLabel,@colLabel): %d");    //项的标签显示格式
    graph3D->addSeries(series);

    //设置数据代理的数据
    QBarDataArray *dataArray = new QBarDataArray;   //棒柱数据数组，typedef QList<QBarDataRow *> QBarDataArray;
    for (int i=0; i<3; i++)     //行
    {
        QBarDataRow *dataRow= new QBarDataRow;      //棒柱数据行,   typedef QList<QBarDataItem> QBarDataRow;
        for(int j=1; j<=5; j++) //列
        {
            quint32 value=QRandomGenerator::global()->bounded(5,15);  //随机整数[5,15)
            QBarDataItem *dataItem= new QBarDataItem;   //创建棒柱数据对象
            dataItem->setValue(value);      //设置棒柱的数据
            dataRow->append(*dataItem);     //添加到棒柱数据行
        }
        dataArray->append(dataRow);         //棒柱数据数组添加一个棒柱数据行
    }

    QStringList rowLabs;    //行坐标标签
    rowLabs << "Week1" << "Week2"<<"Week3";
    series->dataProxy()->setRowLabels(rowLabs); //设置数据代理的行标签

    QStringList colLabs;    //列坐标标签
    colLabs << "Mon" << "Tue" << "Wed" << "Thur"<<"Fri";
    series->dataProxy()->setColumnLabels(colLabs);//设置数据代理的列标签

    series->dataProxy()->resetArray(dataArray);     //重设数据代理的数据

    connect(series,&QBar3DSeries::selectedBarChanged,
            this,&MainWindow::do_barSelected);
}

//void MainWindow::resetData()
//{
//    QBarDataProxy  *dataProxy=new QBarDataProxy;    //新建数据代理
//    int rowCount= series->dataProxy()->rowCount();  //数据代理的行数
//    for (int i=0; i<rowCount; i++) //行
//    {
//        QBarDataRow *dataRow= new QBarDataRow;        //typedef QList<QBarDataItem> QBarDataRow;
//        for(int j=1; j<=5; j++) //列
//        {
//            quint32 value=QRandomGenerator::global()->bounded(5,15);
//            QBarDataItem *dataItem= new QBarDataItem;   //数据项
//            dataItem->setValue(value);
//            dataRow->append(*dataItem);
//        }
//        QString rowStr=QString("第%1周").arg(i+1);
//        dataProxy->addRow(dataRow, rowStr);     //添加行数据和标签
//    }

//    QStringList colLabs=series->dataProxy()->columnLabels();    //原来的列坐标轴标签
//    dataProxy->setColumnLabels(colLabs);

//    series->dataProxy()->resetArray();  //清除数据代理和坐标轴标签，必须清除，否则不能重新设置坐标轴标签
//    series->setDataProxy(dataProxy);    //重新设置数据代理，会删除之前的数据代理
//}

//void MainWindow::redrawGraph3D()
//{
//    QBarDataProxy  *dataProxy=series->dataProxy();
//    int rowCount=dataProxy->rowCount();
//    int colCount=dataProxy->columnLabels().count();
//    for (int i=0; i<rowCount; i++) //行
//    {
//        for(int j=0; j<colCount; j++) //列
//        {
//            QBarDataItem dataItem= *(dataProxy->itemAt(i,j));
//            quint32 value=QRandomGenerator::global()->bounded(5,15);
//            dataItem.setValue(value);
//            dataProxy->setItem(i,j,dataItem);
//        }
//    }
//}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sliderZoom->setRange(10,500);   //设置slider的数据范围
    ui->sliderH->setRange(-180,180);    //水平旋转角度范围
    ui->sliderV->setRange(-180,180);    //垂直旋转角度范围

    iniGraph3D();   //创建三维柱状图，会创建一个容器对象graphContainer
    QSplitter   *splitter=new QSplitter(Qt::Horizontal);
    splitter->addWidget(ui->frameSetup);    //左侧控制面板
    splitter->addWidget(graphContainer);    //右侧三维图
    this->setCentralWidget(splitter);       //设置主窗口中心组建
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_barSelected(const QPoint &position)
{
    if (position.x()<0 || position.y()<0)  //必须加此判断
    {
        ui->actBar_ChangeValue->setEnabled(false);
        return;
    }

    ui->actBar_ChangeValue->setEnabled(true);
    const QBarDataItem *bar =series->dataProxy()->itemAt(position);
    QString info=QString::asprintf("选中的棒柱，Row=%d, Column=%d, Value=%.1f",
                                   position.x(),position.y(),bar->value());
    ui->statusBar->showMessage(info);
}

void MainWindow::on_chkBoxGrid_clicked(bool checked)
{ //显示背景网格 CheckBox
    graph3D->activeTheme()->setGridEnabled(checked);
}

void MainWindow::on_chkBoxSmooth_clicked(bool checked)
{ //光滑效果 CheckBox
    series->setMeshSmooth(checked);
}

void MainWindow::on_chkBoxReflection_clicked(bool checked)
{//显示倒影 CheckBox
    graph3D->setReflection(checked);
}

void MainWindow::on_chkBoxAxisTitle_clicked(bool checked)
{//显示轴标题 CheckBox
    graph3D->valueAxis()->setTitleVisible(checked);
    graph3D->rowAxis()->setTitleVisible(checked);
    graph3D->columnAxis()->setTitleVisible(checked);
}

void MainWindow::on_chkBoxAxisBackground_clicked(bool checked)
{//显示轴标签背景 CheckBox
    graph3D->activeTheme()->setLabelBackgroundEnabled(checked);
}

void MainWindow::on_chkBoxReverse_clicked(bool checked)
{//数值坐标轴反向 CheckBox
    graph3D->valueAxis()->setReversed(checked);
}

void MainWindow::on_chkBoxBackground_clicked(bool checked)
{//显示背景 CheckBox
    graph3D->activeTheme()->setBackgroundEnabled(checked);
}

void MainWindow::on_spinFontSize_valueChanged(int arg1)
{//轴标签字体大小
    QFont font = graph3D->activeTheme()->font();
    font.setPointSize(arg1);
    graph3D->activeTheme()->setFont(font);
}

void MainWindow::on_cBoxTheme_currentIndexChanged(int index)
{//三维图主题
    Q3DTheme *currentTheme = graph3D->activeTheme();
    currentTheme->setType(Q3DTheme::Theme(index));
}

void MainWindow::on_chkBoxItemLabel_clicked(bool checked)
{ //显示选中棒柱的标签 CheckBox
    series->setItemLabelFormat("value at (@rowLabel,@colLabel): %.1f");
    series->setItemLabelVisible(checked);
}

void MainWindow::on_cBoxBarStyle_currentIndexChanged(int index)
{ //棒柱样式 ComboBox
    QAbstract3DSeries::Mesh aMesh;
    aMesh=QAbstract3DSeries::Mesh(index+1);  //0=MeshUserDefined
    series->setMesh(aMesh);
}

void MainWindow::on_cBoxSelectionMode_currentIndexChanged(int index)
{//选择模式
    graph3D->setSelectionMode(QAbstract3DGraph::SelectionFlags(index));
}

void MainWindow::on_comboCamera_currentIndexChanged(int index)
{ //预设视角
    Q3DCamera::CameraPreset  cameraPos=Q3DCamera::CameraPreset(index);
    graph3D->scene()->activeCamera()->setCameraPreset(cameraPos);
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

void MainWindow::on_actBar_ChangeValue_triggered()
{
    QPoint position=series->selectedBar();
    if (position.x()<0 || position.y()<0)   //必须加此判断
        return;

    QBarDataItem bar=*(series->dataProxy()->itemAt(position));
    qreal value=bar.value();    //原来的值

    bool ok;
    value = QInputDialog::getInt(this, "输入数值","更改棒柱数值" , value, 0, 50, 1, &ok);
    if (ok)
    {
        bar.setValue(value);
        series->dataProxy()->setItem(position, bar);
    }
}

void MainWindow::on_actSeries_BaseColor_triggered()
{//设置序列基本颜色
    QColor  color=series->baseColor();
    color=QColorDialog::getColor(color);
    if (color.isValid())
        series->setBaseColor(color);
}

void MainWindow::on_actData_Add_triggered()
{//添加行
    QString rowLabel = QInputDialog::getText(this, "输入字符串","请输入行标签");
    if (rowLabel.isEmpty())
        return;

    QBarDataRow *dataRow= new QBarDataRow;       //棒柱数据行
    for(int j=1; j<=5; j++)     //固定5列
    {
        quint32 value=QRandomGenerator::global()->bounded(15,25);
        QBarDataItem *dataItem= new QBarDataItem;
        dataItem->setValue(value);
        dataRow->append(*dataItem);
    }
    series->dataProxy()->addRow(dataRow, rowLabel);     //添加棒柱数据行和标签
}

void MainWindow::on_actData_Insert_triggered()
{ //插入行
    QString rowLabel = QInputDialog::getText(this, "输入字符串","请输入行标签");
    if (rowLabel.isEmpty())
        return;
    QPoint position=series->selectedBar();
    int index=position.x(); //当前行号
    if (index<0)
        index=0;

    QBarDataRow *dataRow= new QBarDataRow;       //棒柱数据行
    for(int j=1; j<=5; j++)     //固定5列
    {
        quint32 value=QRandomGenerator::global()->bounded(20,30);
        QBarDataItem *dataItem= new QBarDataItem;
        dataItem->setValue(value);
        dataRow->append(*dataItem);
    }
    series->dataProxy()->insertRow(index, dataRow, rowLabel);    //插入棒柱数据行和标签
}

void MainWindow::on_actData_Delete_triggered()
{//删除行
    QPoint position=series->selectedBar();
    if (position.x()<0 || position.y()<0)
        return;

    int rowIndex=position.x();  //当前行号
    int removeCount=1;          //删除的行数
    int removeLabels=true;      //是否删除行标签
    series->dataProxy()->removeRows(rowIndex, removeCount, removeLabels);
}

void MainWindow::on_btnMoveDown_clicked()
{//下移
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    qreal z=target3D.z();
    target3D.setZ(z+0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
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

void MainWindow::on_btnResetCamera_clicked()
{//复位到FrontHigh视角
    graph3D->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFrontHigh);
}

void MainWindow::on_actRedraw_triggered()
{
    QBarDataProxy  *dataProxy=new QBarDataProxy;    //新建数据代理
    int rowCount= series->dataProxy()->rowCount();  //数据代理的行数
    for (int i=0; i<rowCount; i++)  //行
    {
        QBarDataRow *dataRow= new QBarDataRow;       //棒柱数据行
        for(int j=1; j<=5; j++)     //列
        {
            quint32 value=QRandomGenerator::global()->bounded(5,15);
            QBarDataItem *dataItem= new QBarDataItem;   //数据项
            dataItem->setValue(value);
            dataRow->append(*dataItem);
        }
        QString rowStr=QString("第%1周").arg(i+1);
        dataProxy->addRow(dataRow, rowStr);     //添加行数据和标签
    }

    QStringList colLabs=series->dataProxy()->columnLabels();    //原来的列坐标轴标签
    dataProxy->setColumnLabels(colLabs);

    series->dataProxy()->resetArray();  //清除数据代理和坐标轴标签，必须清除，否则不能重新设置坐标轴标签
    series->setDataProxy(dataProxy);    //重新设置数据代理，会删除之前的数据代理
}

