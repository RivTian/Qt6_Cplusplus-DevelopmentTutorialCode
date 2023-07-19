#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QMessageBox>
#include    <QCloseEvent>

#include    "tdialogsize.h"


void MainWindow::closeEvent(QCloseEvent *event)
{ //窗口关闭时询问是否退出
    QMessageBox::StandardButton result;
//    result= QMessageBox::question(this, "确认", "确定要退出本程序吗？",
//                                  QMessageBox::Yes|QMessageBox::No |QMessageBox::Cancel,
//                                  QMessageBox::No);
    result= QMessageBox::question(this, "确认", "确定要退出本程序吗？");
    if (result==QMessageBox::Yes)
        event->accept();    //退出
    else
        event->ignore();    //不退出
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_model = new QStandardItemModel(6,4,this);    //创建数据模型
    QStringList header;
    header<<"姓名"<<"性别"<<"学位"<<"部门";
    m_model->setHorizontalHeaderLabels(header);    //设置表头标题

    m_selection = new QItemSelectionModel(m_model);   //创建选择模型
    connect(m_selection, &QItemSelectionModel::currentChanged,
            this, &MainWindow::do_model_currentChanged);
//    connect(m_selection,SIGNAL(currentChanged(QModelIndex,QModelIndex)),
//            this,SLOT(do_model_currentChanged(QModelIndex,QModelIndex)));

    ui->tableView->setModel(m_model);  //设置数据模型
    ui->tableView->setSelectionModel(m_selection);     //设置选择模型

    setCentralWidget(ui->tableView);

    //创建状态栏组件
    labCellPos = new QLabel("当前单元格：",this);
    labCellPos->setMinimumWidth(180);
    labCellPos->setAlignment(Qt::AlignHCenter);
    ui->statusBar->addWidget(labCellPos);

    labCellText = new QLabel("单元格内容：",this);
    labCellText->setMinimumWidth(200);
    ui->statusBar->addWidget(labCellText);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::do_setActLocateEnable(bool enable)
//{ //设置actTab_Locate的enable属性
//    ui->actTab_Locate->setEnabled(enable);
//}

void MainWindow::selectCell(int row, int column)
{
    QModelIndex index=m_model->index(row,column);
    m_selection->clearSelection();
    m_selection->setCurrentIndex(index,QItemSelectionModel::Select);
}

void MainWindow::do_setCellText(int row, int column, QString &text)
{//定位到单元格，并设置文字
    QModelIndex index=m_model->index(row,column);//获取模型索引
    m_selection->clearSelection(); //清除现有选择
    m_selection->setCurrentIndex(index,QItemSelectionModel::Select); //定位到单元格
    m_model->setData(index,text,Qt::DisplayRole);//设置单元格文字
}

void MainWindow::do_model_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    if (current.isValid()) //当前模型索引有效
    {
        labCellPos->setText(QString::asprintf("当前单元格：%d行，%d列",
                            current.row(),current.column())); //显示模型索引的行和列号
        QStandardItem   *aItem;
        aItem=m_model->itemFromIndex(current); //从模型索引获得Item
        this->labCellText->setText("单元格内容："+aItem->text()); //显示item的文字内容
    }
}

void MainWindow::on_actTab_SetSize_triggered()
{ //模态对话框，动态创建，用过后删除
    TDialogSize    *dlgTableSize=new TDialogSize(this); //创建对话框
    //   dlgTableSize->setAttribute(Qt::WA_DeleteOnClose);
    //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
    //如果需要获取对话框的返回值，不能设置该属性，可以在调用完对话框后删除对话框
//    Qt::WindowFlags    flags=dlgTableSize->windowFlags();
//    dlgTableSize->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小

    dlgTableSize->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小
    dlgTableSize->setRowColumn(m_model->rowCount(),m_model->columnCount()); //对话框数据初始化

    int ret=dlgTableSize->exec();   //以模态方式显示对话框，用户关闭对话框时返回 DialogCode值
    if (ret==QDialog::Accepted)     //OK键被按下,对话框关闭，若设置了setAttribute(Qt::WA_DeleteOnClose)，对话框被释放，无法获得返回值
    { //OK键被按下，获取对话框上的输入，设置行数和列数
        int cols=dlgTableSize->columnCount();
        m_model->setColumnCount(cols);
        int rows=dlgTableSize->rowCount();
        m_model->setRowCount(rows);
    }
    delete dlgTableSize;    //删除对话框
}

void MainWindow::on_actTab_SetHeader_triggered()
{//一次创建，多次调用，对话框关闭时只是隐藏
    if (dlgSetHeaders==nullptr) //如果对象没有被创建过，就创建对象
        dlgSetHeaders = new TDialogHeaders(this);

    if (dlgSetHeaders->headerList().size()!=m_model->columnCount())
    {
        //只需在创建时传递参数给对话框,由于对话框只是隐藏，界面内容保存
        QStringList strList;
        for (int i=0;i<m_model->columnCount();i++)//获取现有的表头标题
            strList.append(m_model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
        dlgSetHeaders->setHeaderList(strList);  //用于对话框初始化显示
    }

    int ret=dlgSetHeaders->exec();  //以模态方式显示对话框
    if (ret==QDialog::Accepted)     //OK键被按下
    {
        QStringList strList=dlgSetHeaders->headerList();    //获取对话框上修改后的StringList
        m_model->setHorizontalHeaderLabels(strList);       // 设置模型的表头标题
    }
}

void MainWindow::on_actTab_Locate_triggered()
{//创建 StayOnTop的对话框，对话框关闭时自动删除
    TDialogLocate *dlgLocate = new TDialogLocate(this);
    dlgLocate->setAttribute(Qt::WA_DeleteOnClose);  //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
    dlgLocate->setWindowFlag(Qt::WindowStaysOnTopHint); //设置对话框,StayOnTop

    //对话框初始化设置
    dlgLocate->setSpinRange(m_model->rowCount(),m_model->columnCount());
    QModelIndex curIndex=m_selection->currentIndex();
    if (curIndex.isValid())
        dlgLocate->setSpinValue(curIndex.row(),curIndex.column());

    //对话框发射changeCellText()信号，用于定位单元格并设置文字
    connect(dlgLocate,&TDialogLocate::changeCellText,this,&MainWindow::do_setCellText);
//    connect(dlgLocate,SIGNAL(changeCellText(int,int,QString&)),
//            this,SLOT(do_setCellText(int,int,QString&)));

    //对话框发射changeActionEnable()信号，设置actTab_Locate的enabled属性
    connect(dlgLocate, &TDialogLocate::changeActionEnable,
            ui->actTab_Locate, &QAction::setEnabled);

    //主窗口发射cellIndexChanged()信号，修改对话框上的spinBox的值
    connect(this,&MainWindow::cellIndexChanged,dlgLocate,&TDialogLocate::setSpinValue);
//    connect(this,SIGNAL(cellIndexChanged(int,int)),
//            dlgLocate,SLOT(setSpinValue(int,int)));

    dlgLocate->setModal(false);
    dlgLocate->show();  //非模态显示对话框
}

//单击单元格时，将单元格的行号、列号设置到对话框上
void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    emit cellIndexChanged(index.row(),index.column());
}
