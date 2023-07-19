#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QFileDialog>
#include    <QMessageBox>


//打开数据表
void MainWindow::openTable()
{
    tabModel=new QSqlRelationalTableModel(this,DB);
    tabModel->setTable("studInfo");     //设置数据表
    tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);  //编辑策略
    tabModel->setSort(tabModel->fieldIndex("studID"),Qt::AscendingOrder);

    selModel=new QItemSelectionModel(tabModel,this);     //创建选择模型
    connect(selModel,&QItemSelectionModel::currentChanged,this, &MainWindow::do_currentChanged);
//    connect(selModel,SIGNAL(currentChanged(QModelIndex,QModelIndex)),
//            this,SLOT(do_currentChanged(QModelIndex,QModelIndex)));

    ui->tableView->setModel(tabModel);
    ui->tableView->setSelectionModel(selModel);

    tabModel->setHeaderData(tabModel->fieldIndex("studID"),  Qt::Horizontal, "学号");
    tabModel->setHeaderData(tabModel->fieldIndex("name"),    Qt::Horizontal, "姓名");
    tabModel->setHeaderData(tabModel->fieldIndex("gender"),  Qt::Horizontal, "性别");
    tabModel->setHeaderData(tabModel->fieldIndex("departID"),Qt::Horizontal, "学院");
    tabModel->setHeaderData(tabModel->fieldIndex("majorID"), Qt::Horizontal, "专业");

    //设置代码字段的关系
    tabModel->setRelation(tabModel->fieldIndex("departID"),
                          QSqlRelation("departments","departID","department")); //学院
    tabModel->setRelation(tabModel->fieldIndex("majorID"),
                          QSqlRelation("majors","majorID","major"));            //专业

    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));  //为关系型字段设置默认代理组件
//    ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("departID"),
//                                            new QSqlRelationalDelegate(ui->tableView)); //为关系型字段设置缺省代理组件
//    ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("majorID"),
//                                   new QSqlRelationalDelegate(ui->tableView)); //为关系型字段设置缺省代理组件

    tabModel->select(); //查询数据表的数据

    ui->actOpenDB->setEnabled(false);
    ui->actRecAppend->setEnabled(true);
    ui->actRecInsert->setEnabled(true);
    ui->actRecDelete->setEnabled(true);
    ui->actFields->setEnabled(true);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setCentralWidget(ui->tableView);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::openDatabase(QString aFile)
{
    DB=QSqlDatabase::addDatabase("QSQLITE"); //添加SQLITE数据库驱动
    DB.setDatabaseName(aFile);      //设置数据库名称
    if (DB.open())
    {
        openTable();    //打开数据表
        return true;
    }
    else
        return false;
}

//更新actPost和actCancel 的状态
void MainWindow::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    ui->actSubmit->setEnabled(tabModel->isDirty()); //有未保存的修改时可用
    ui->actRevert->setEnabled(tabModel->isDirty());
}

void MainWindow::on_actOpenDB_triggered()
{
    QString aFile=QFileDialog::getOpenFileName(this,"选择文件","","SQLite数据库(*.db3)");
    if (aFile.isEmpty())
        return;

    //打开数据库
    DB=QSqlDatabase::addDatabase("QSQLITE"); //添加SQLITE数据库驱动
    DB.setDatabaseName(aFile);      //设置数据库名称
    //    DB.setHostName();
    //    DB.setUserName();
    //    DB.setPassword();
    if (DB.open())
        openTable();    //打开数据表
    else
        QMessageBox::warning(this, "错误", "打开数据库失败");

}

void MainWindow::on_actRecAppend_triggered()
{//添加记录
    tabModel->insertRow(tabModel->rowCount(),QModelIndex());    //在末尾添加一行
    QModelIndex curIndex=tabModel->index(tabModel->rowCount()-1,1);
    selModel->clearSelection();     //清空选择项
    selModel->setCurrentIndex(curIndex,QItemSelectionModel::Select); //设置当前行
}

void MainWindow::on_actRecInsert_triggered()
{//插入记录
    QModelIndex curIndex=ui->tableView->currentIndex(); //当前行的模型索引
    tabModel->insertRow(curIndex.row(),QModelIndex());
    selModel->clearSelection();
    selModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);
}

void MainWindow::on_actRecDelete_triggered()
{//删除当前行
    tabModel->removeRow(selModel->currentIndex().row());
    tabModel->submitAll();      //立即提交修改
}



void MainWindow::on_actSubmit_triggered()
{//保存修改
    bool res=tabModel->submitAll();
    if (!res)
        QMessageBox::information(this, "消息", "数据保存错误,错误信息\n"
                                 +tabModel->lastError().text());
    else
    {
        ui->actSubmit->setEnabled(false);
        ui->actRevert->setEnabled(false);
    }
}

void MainWindow::on_actRevert_triggered()
{//取消修改
    tabModel->revertAll();
    ui->actSubmit->setEnabled(false);
    ui->actRevert->setEnabled(false);
}

void MainWindow::on_actFields_triggered()
{//获取字段列表
    QSqlRecord  emptyRec=tabModel->record();//获取空记录，只有字段名
    QString  str;
    for (int i=0;i<emptyRec.count();i++)
        str=str+emptyRec.fieldName(i)+'\n';

    QMessageBox::information(this, "所有字段名", str);
}
