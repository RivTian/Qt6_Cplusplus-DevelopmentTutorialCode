#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QFileDialog>
#include    <QMessageBox>


//查询数据
void MainWindow::selectData()
{
    //1. 创建数据模型，查询数据
    qryModel=new QSqlQueryModel(this);
    qryModel->setQuery("SELECT empNo, Name, Gender,  Birthday,  Province,  Department, "
                       " Salary FROM employee ORDER BY empNo");
    if (qryModel->lastError().isValid())
    {
        QMessageBox::critical(this, "错误", "数据表查询错误,错误信息\n"
                              +qryModel->lastError().text());
        return;
    }
    ui->statusBar->showMessage(QString("记录条数：%1").arg(qryModel->rowCount()));//显示记录条数

    //2. 设置字段显示标题
    QSqlRecord rec=qryModel->record();  //获取一个空记录，为了获取字段序号
    qryModel->setHeaderData(rec.indexOf("empNo"),  Qt::Horizontal, "工号");
    qryModel->setHeaderData(rec.indexOf("Name"),   Qt::Horizontal, "姓名");
    qryModel->setHeaderData(rec.indexOf("Gender"), Qt::Horizontal, "性别");
    qryModel->setHeaderData(rec.indexOf("Birthday"),   Qt::Horizontal, "出生日期");
    qryModel->setHeaderData(rec.indexOf("Province"),   Qt::Horizontal, "省份");
    qryModel->setHeaderData(rec.indexOf("Department"), Qt::Horizontal, "部门");
    qryModel->setHeaderData(rec.indexOf("Salary"),     Qt::Horizontal, "工资");

    //3. 创建选择模型
    selModel=new QItemSelectionModel(qryModel,this);
    connect(selModel,&QItemSelectionModel::currentRowChanged,
            this, &MainWindow::do_currentRowChanged);
    ui->tableView->setModel(qryModel);
    ui->tableView->setSelectionModel(selModel);
    //    ui->tableView->resizeColumnsToContents();
    //    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    //4. 创建数据映射
    dataMapper= new QDataWidgetMapper(this);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    dataMapper->setModel(qryModel);

    //界面组件与数据模型的具体字段之间的映射
    dataMapper->addMapping(ui->dbSpinEmpNo, rec.indexOf("empNo"));
    dataMapper->addMapping(ui->dbEditName,  rec.indexOf("Name"));
    dataMapper->addMapping(ui->dbComboSex,  rec.indexOf("Gender"));
    dataMapper->addMapping(ui->dbEditBirth, rec.indexOf("Birthday"));
    dataMapper->addMapping(ui->dbComboProvince, rec.indexOf("Province"));
    dataMapper->addMapping(ui->dbComboDep,      rec.indexOf("Department"));
    dataMapper->addMapping(ui->dbSpinSalary,    rec.indexOf("Salary"));

    dataMapper->toFirst();  //移动到首记录
    ui->actOpenDB->setEnabled(false);
}

//刷新tableView的当前行
void MainWindow::refreshTableView()
{
    int index=dataMapper->currentIndex();   //dataMapper的当前行号
    QModelIndex curIndex=qryModel->index(index,1);  //为当前行创建模型索引
    selModel->clearSelection();         //清空选择项
    selModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);    //设置当前行
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->splitter);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);      //不能编辑
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);     //行选择
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);    //单行选择
    ui->tableView->setAlternatingRowColors(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::openDatabase(QString aFile)
{
    DB=QSqlDatabase::addDatabase("QSQLITE");  //添加 SQLITE数据库驱动
    DB.setDatabaseName(aFile);   //设置数据库文件
    if (DB.open())       //成功打开数据库
    {
        selectData();
        return true;
    }
    else
        return false;

}

void MainWindow::do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if (!current.isValid())
    {
        ui->dbLabPhoto->clear();
        ui->dbEditMemo->clear();
        return;
    }

    dataMapper->setCurrentModelIndex(current);      //设置当前行
    //    dataMapper->setCurrentIndex(current.row());   //这个也可以

    bool first=(current.row()==0);  //是否首记录
    bool last=(current.row()==qryModel->rowCount()-1);  //是否尾记录

    ui->actRecFirst->setEnabled(!first);    //更新使能状态
    ui->actRecPrevious->setEnabled(!first);
    ui->actRecNext->setEnabled(!last);
    ui->actRecLast->setEnabled(!last);

    int curRecNo=selModel->currentIndex().row();
    QSqlRecord  curRec=qryModel->record(curRecNo);  //获取当前记录
    int empNo=curRec.value("EmpNo").toInt();    //主键字段

    QSqlQuery query;    //查询某个empNo的Memo和Photo字段的数据
    query.prepare("select EmpNo, Memo, Photo from employee where EmpNo = :ID");
    query.bindValue(":ID",empNo);
    query.exec();
    query.first();

    QVariant    va=query.value("Photo");
    if (!va.isValid())  //图片字段内容为空
        ui->dbLabPhoto->clear();
    else    //显示图片
    {
        QByteArray data=va.toByteArray();
        QPixmap pic;
        pic.loadFromData(data);
        ui->dbLabPhoto->setPixmap(pic.scaledToWidth(ui->dbLabPhoto->size().width()));
    }

    QVariant    va2=query.value("Memo");    //显示备注
    ui->dbEditMemo->setPlainText(va2.toString());
}

void MainWindow::on_actOpenDB_triggered()
{//打开数据库
    QString aFile=QFileDialog::getOpenFileName(this,"选择文件","","SQLite数据库(*.db3)");
    if (aFile.isEmpty())
        return;

    DB=QSqlDatabase::addDatabase("QSQLITE");  //添加 SQLITE数据库驱动
    DB.setDatabaseName(aFile);   //设置数据库文件
    if (DB.open())       //成功打开数据库
        selectData();
    else
        QMessageBox::warning(this, "错误", "打开数据库失败");
}

void MainWindow::on_actRecFirst_triggered()
{ //首记录
    dataMapper->toFirst();
    refreshTableView();
}

void MainWindow::on_actRecPrevious_triggered()
{ //前一记录
    dataMapper->toPrevious();
    refreshTableView();
}

void MainWindow::on_actRecNext_triggered()
{//后一记录
    dataMapper->toNext();
    refreshTableView();
}

void MainWindow::on_actRecLast_triggered()
{//尾记录
    dataMapper->toLast();
    refreshTableView();
}
