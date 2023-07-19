#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include    <QPainter>
#include    <QFileDialog>
#include    <QCloseEvent>

#include    "tformdoc.h"

void MainWindow::closeEvent(QCloseEvent *event)
{
    ui->mdiArea->closeAllSubWindows();  //关闭所有子窗口
    event->accept();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->mdiArea);
    setWindowState(Qt::WindowMaximized);  //窗口最大化显示
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
//    this->setAutoFillBackground(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actDoc_Open_triggered()
{//打开文件
//必须先获取当前MDI子窗口，再使用打开文件对话框，否则无法获得活动的子窗口
    bool needNew=false;     // 是否需要新建子窗口

    TFormDoc    *formDoc;
    if (ui->mdiArea->subWindowList().size()>0) //如果有打开的主窗口，获取活动窗口
    {
        formDoc=(TFormDoc*)ui->mdiArea->activeSubWindow()->widget();
        needNew=formDoc->isFileOpened();        //文件已经打开，需要新建窗口
    }
    else
        needNew=true;

    QString curPath=QDir::currentPath();
    QString aFileName=QFileDialog::getOpenFileName(this,tr("打开一个文件"),curPath,
                 "C程序文件(*.h *cpp);;文本文件(*.txt);;所有文件(*.*)");
    if (aFileName.isEmpty())
        return;     //如果未选择文件，退出

    if (needNew)    //需要新建子窗口
    {
        formDoc = new TFormDoc(this);   //指定父窗口，必须在父窗口为Widget窗口提供一个显示区域
        ui->mdiArea->addSubWindow(formDoc);
    }

    formDoc->loadFromFile(aFileName);   //打开文件
    formDoc->show();

    ui->actCut->setEnabled(true);
    ui->actCopy->setEnabled(true);
    ui->actPaste->setEnabled(true);
    ui->actFont->setEnabled(true);
}


void MainWindow::on_actDoc_New_triggered()
{ //新建文件
    TFormDoc *formDoc = new TFormDoc(this);
    ui->mdiArea->addSubWindow(formDoc);     //文档窗口添加到MDI
    formDoc->show();    //在单独的窗口中显示

    ui->actCut->setEnabled(true);
    ui->actCopy->setEnabled(true);
    ui->actPaste->setEnabled(true);
    ui->actFont->setEnabled(true);
}

void MainWindow::on_actCut_triggered()
{ //cut
    TFormDoc* formDoc=(TFormDoc*)ui->mdiArea->activeSubWindow()->widget();
    formDoc->textCut();
}

void MainWindow::on_actFont_triggered()
{//设置字体
    TFormDoc* formDoc=(TFormDoc*)ui->mdiArea->activeSubWindow()->widget();
    formDoc->setEditFont();
}

void MainWindow::on_actCopy_triggered()
{//copy
    TFormDoc* formDoc=(TFormDoc*)ui->mdiArea->activeSubWindow()->widget();
    formDoc->textCopy();
}

void MainWindow::on_actPaste_triggered()
{//paste
    TFormDoc* formDoc=(TFormDoc*)ui->mdiArea->activeSubWindow()->widget();
    formDoc->textPaste();
}

void MainWindow::on_mdiArea_subWindowActivated(QMdiSubWindow *arg1)
{//当前活动子窗口切换时
    Q_UNUSED(arg1);
    if (ui->mdiArea->subWindowList().size()==0)
    { //若子窗口个数为零
        ui->actCut->setEnabled(false);
        ui->actCopy->setEnabled(false);
        ui->actPaste->setEnabled(false);
        ui->actFont->setEnabled(false);
        ui->statusBar->clearMessage();
    }
    else
    {
        TFormDoc *formDoc=static_cast<TFormDoc*>(ui->mdiArea->activeSubWindow()->widget());
        ui->statusBar->showMessage(formDoc->currentFileName()); //显示主窗口的文件名
    }
}

void MainWindow::on_actViewMode_triggered(bool checked)
{//MDI 显示模式
    if (checked) //Tab多页显示模式
        ui->mdiArea->setViewMode(QMdiArea::TabbedView); //Tab多页显示模式
    else //子窗口模式
        ui->mdiArea->setViewMode(QMdiArea::SubWindowView); //子窗口模式
    ui->mdiArea->setTabsClosable(checked);  //切换到多页模式下需重新设置
    ui->actCascade->setEnabled(!checked);   //子窗口模式下才有用
    ui->actTile->setEnabled(!checked);
}

void MainWindow::on_actCascade_triggered()
{ //窗口级联展开
    ui->mdiArea->cascadeSubWindows();
}

void MainWindow::on_actTile_triggered()
{//平铺展开
    ui->mdiArea->tileSubWindows();
}

void MainWindow::on_actCloseALL_triggered()
{//关闭全部子窗口
    ui->mdiArea->closeAllSubWindows();
}


void MainWindow::on_actDoc_Save_triggered()
{//保存
    TFormDoc* formDoc=(TFormDoc*)ui->mdiArea->activeSubWindow()->widget();
    formDoc->saveToFile();
}



