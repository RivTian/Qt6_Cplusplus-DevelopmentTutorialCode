#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QPainter>
#include    <QFileDialog>


//绘制窗口背景图片
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);     //获取窗口的画笔
    painter.drawPixmap(0,ui->mainToolBar->height(),this->width(),
                       this->height()-ui->mainToolBar->height()-ui->statusBar->height(),
                       QPixmap(":/images/images/back2.jpg"));
    event->accept();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setVisible(false);
    ui->tabWidget->clear();                 //清除所有页面
    ui->tabWidget->setTabsClosable(true);   //各页面有关闭按钮，可被关闭
    ui->tabWidget->setDocumentMode(false);  //不能设置为稳定模式，否则工具栏显示背景图片

    this->setCentralWidget(ui->tabWidget);

    //    this->setWindowState(Qt::WindowFullScreen); //窗口全屏显示,没有边框，没有标题栏
    this->setWindowState(Qt::WindowMaximized); //窗口最大化显示

    //    this->setAutoFillBackground(false);
    //如果设置为true，会在调用paint事件之前，用 QPalette::Window 颜色绘制窗口背景。我们绘制背景图片，就没必要绘制背景颜色
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_changeTabTitle(QString title)
{
    int index=ui->tabWidget->currentIndex();
    ui->tabWidget->setTabText(index,title);
}


//关闭Tab
void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    if (index<0)
        return;

    QWidget* aForm=ui->tabWidget->widget(index);
    aForm->close();
}


//创建TFormDoc窗口，并在tabWidget中显示
void MainWindow::on_actWidgetInsite_triggered()
{
    TFormDoc *formDoc = new TFormDoc(this);       //指定主窗口为父容器
    formDoc->setAttribute(Qt::WA_DeleteOnClose);  //关闭时自动删除

    int cur= ui->tabWidget->addTab(formDoc,
                                  QString::asprintf("Doc %d",ui->tabWidget->count()));
    ui->tabWidget->setCurrentIndex(cur);
    ui->tabWidget->setVisible(true);
    connect(formDoc, &TFormDoc::titleChanged,this, &MainWindow::do_changeTabTitle);
}

void MainWindow::on_actWindowInsite_triggered()
{
    TFormTable *formTable = new TFormTable(this);
    formTable->setAttribute(Qt::WA_DeleteOnClose); //关闭时自动删除
    int cur=ui->tabWidget->addTab(formTable,
                                  QString::asprintf("Table %d",ui->tabWidget->count()));
    ui->tabWidget->setCurrentIndex(cur);
    ui->tabWidget->setVisible(true);
}

void MainWindow::on_actWindow_triggered()
{
    TFormTable* formTable = new TFormTable(this);
    //    formTable = new TFormTable(); //无父窗体,在windows任务栏上有显示
    formTable->setAttribute(Qt::WA_DeleteOnClose); //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
    formTable->setWindowTitle("基于QMainWindow的窗口");
    formTable->statusBar(); //如果没有状态栏，就创建状态栏
    formTable->show();
}


void MainWindow::on_actWidget_triggered()
{
    TFormDoc *formDoc = new TFormDoc();             //不指定父窗口，用show()显示
    formDoc->setAttribute(Qt::WA_DeleteOnClose);    //关闭时自动删除
    formDoc->setWindowTitle("基于QWidget的窗口，无父窗口，关闭时删除");
    formDoc->setWindowFlag(Qt::Window, true);       //设置window标识

    //    formDoc->setWindowFlag(Qt::CustomizeWindowHint,true);
    //    formDoc->setWindowFlag(Qt::WindowMinMaxButtonsHint,true);
    //    formDoc->setWindowFlag(Qt::WindowCloseButtonHint,true);
    //    formDoc->setWindowFlag(Qt::WindowStaysOnTopHint,true);

    //    formDoc->setWindowState(Qt::WindowMaximized);
    formDoc->setWindowOpacity(0.9);     //设置透明度
    //    formDoc->setWindowModality(Qt::WindowModal);

    formDoc->show(); //显示为单独的窗口
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index);
    bool  en=ui->tabWidget->count()>0; //再无页面时
    ui->tabWidget->setVisible(en);
}

