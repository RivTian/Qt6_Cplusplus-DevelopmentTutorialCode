#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QFileDialog>
#include    <QMessageBox>


//打开数据表
void MainWindow::openTable()
{
    //1. 创建数据模型，打开数据表
    tabModel=new QSqlTableModel(this,DB);	//数据模型
    tabModel->setTable("employee");			//设置数据表
    tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);	//数据保存方式，OnManualSubmit , OnRowChange
    tabModel->setSort(tabModel->fieldIndex("empNo"),Qt::AscendingOrder); //排序
    if (!(tabModel->select()))	//查询数据失败
    {
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel->lastError().text());
        return;
    }
    showRecordCount();      //显示记录条数

    //2. 设置字段显示标题
    tabModel->setHeaderData(tabModel->fieldIndex("empNo"),  Qt::Horizontal, "工号");
    tabModel->setHeaderData(tabModel->fieldIndex("Name"),   Qt::Horizontal, "姓名");
    tabModel->setHeaderData(tabModel->fieldIndex("Gender"), Qt::Horizontal, "性别");
    tabModel->setHeaderData(tabModel->fieldIndex("Birthday"),   Qt::Horizontal, "出生日期");
    tabModel->setHeaderData(tabModel->fieldIndex("Province"),   Qt::Horizontal, "省份");
    tabModel->setHeaderData(tabModel->fieldIndex("Department"), Qt::Horizontal, "部门");
    tabModel->setHeaderData(tabModel->fieldIndex("Salary"), Qt::Horizontal, "工资");
    //这两个字段不在tableView中显示
    tabModel->setHeaderData(tabModel->fieldIndex("Memo"),Qt::Horizontal,"备注");
    tabModel->setHeaderData(tabModel->fieldIndex("Photo"),Qt::Horizontal,"照片");

    //3. 创建选择模型
    selModel=new QItemSelectionModel(tabModel,this);		//关联选择模型

    //当前行或列变化时发射currentChanged信号
    connect(selModel,&QItemSelectionModel::currentChanged,
            this,&MainWindow::do_currentChanged);
    //当前行变化时发射currentRowChanged()信号
    connect(selModel,&QItemSelectionModel::currentRowChanged,
            this,&MainWindow::do_currentRowChanged);

    //4. Model/View结构
    ui->tableView->setModel(tabModel);	//设置数据模型
    ui->tableView->setSelectionModel(selModel); //设置选择模型
    ui->tableView->setColumnHidden(tabModel->fieldIndex("Memo"),true);	//隐藏列
    ui->tableView->setColumnHidden(tabModel->fieldIndex("Photo"),true);	//隐藏列


    //5. 为tableView中的“性别”和“部门”两个字段设置自定义代理组件
    QStringList strList;
    strList<<"男"<<"女";
    bool isEditable=false;
    delegateSex.setItems(strList,isEditable);
    ui->tableView->setItemDelegateForColumn(
                tabModel->fieldIndex("Gender"),&delegateSex);   //设置代理组件

    strList.clear();
    strList<<"销售部"<<"技术部"<<"生产部"<<"行政部";
    isEditable=true;
    delegateDepart.setItems(strList,isEditable);
    ui->tableView->setItemDelegateForColumn(
                tabModel->fieldIndex("Department"),&delegateDepart); //设置代理组件

    //6. 创建界面组件与数据模型的字段之间的数据映射
    dataMapper= new QDataWidgetMapper(this);
    dataMapper->setModel(tabModel);		//设置数据模型
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit); //数据更新方式
    //界面组件与数据模型的具体字段之间的联系
    dataMapper->addMapping(ui->dbSpinEmpNo,tabModel->fieldIndex("empNo"));
    dataMapper->addMapping(ui->dbEditName,tabModel->fieldIndex("Name"));
    dataMapper->addMapping(ui->dbComboSex,tabModel->fieldIndex("Gender"));
    dataMapper->addMapping(ui->dbEditBirth,tabModel->fieldIndex("Birthday"));
    dataMapper->addMapping(ui->dbComboProvince,tabModel->fieldIndex("Province"));
    dataMapper->addMapping(ui->dbComboDep,tabModel->fieldIndex("Department"));
    dataMapper->addMapping(ui->dbSpinSalary,tabModel->fieldIndex("Salary"));
    dataMapper->addMapping(ui->dbEditMemo,tabModel->fieldIndex("Memo"));
    dataMapper->toFirst();  //移动到首记录

    //7. 获取字段名称列表，填充groupBoxSort
    getFieldNames();

    //8.更新actions和界面组件的使能状态
    ui->actOpenDB->setEnabled(false);	//不能再打开数据库

    ui->actRecAppend->setEnabled(true);
    ui->actRecInsert->setEnabled(true);
    ui->actRecDelete->setEnabled(true);
    ui->actScan->setEnabled(true);

    ui->groupBoxSort->setEnabled(true);		//"排序"分租框
    ui->groupBoxFilter->setEnabled(true);	//"数据过滤"分组框
}

//获取所有字段名称
void MainWindow::getFieldNames()
{
    QSqlRecord  emptyRec=tabModel->record();    //获取空记录，只有字段名
    for (int i=0;i<emptyRec.count();i++)
        ui->comboFields->addItem(emptyRec.fieldName(i));
}

void MainWindow::showRecordCount()
{//在状态栏上显示数据模型的记录条数
    ui->statusBar->showMessage(QString("记录条数：%1").arg(tabModel->rowCount()));
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setCentralWidget(ui->splitter);

    //   tableView显示属性设置
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);	//项选择
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);	//单项选择
    ui->tableView->setAlternatingRowColors(true);	//交错行底色
    //    ui->tableView->resizeColumnsToContents();
    //    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::openDatabase(QString aFile)
{
    DB=QSqlDatabase::addDatabase("QSQLITE");	//添加 SQLITE数据库驱动
    DB.setDatabaseName(aFile);	//设置数据库名称
    if (!DB.open())     //打开数据库失败
        return false;
    else
    {
        openTable();	//打开数据表
        return true;
    }
}


//数据发生修改，更新actPost和actCancel 的状态
void MainWindow::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    ui->actSubmit->setEnabled(tabModel->isDirty());		//有未保存修改时可用
    ui->actRevert->setEnabled(tabModel->isDirty());
}

void MainWindow::do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    // 行切换时的状态控制
    ui->actRecDelete->setEnabled(current.isValid());    //"删除"按钮
    ui->actPhoto->setEnabled(current.isValid());        //"设置照片"按钮
    ui->actPhotoClear->setEnabled(current.isValid());   //"清除照片"按钮

    if (!current.isValid())
    {
        ui->dbLabPhoto->clear();
        return;
    }

    dataMapper->setCurrentIndex(current.row());     //更细数据映射的行号

    int curRecNo=current.row();     //获取行号
    QSqlRecord  curRec=tabModel->record(curRecNo); //获取当前记录

    if (curRec.isNull("Photo"))     //图片字段内容为空
        ui->dbLabPhoto->clear();
    else
    {
        QByteArray data=curRec.value("Photo").toByteArray();
        QPixmap pic;
        pic.loadFromData(data);
        ui->dbLabPhoto->setPixmap(pic.scaledToWidth(ui->dbLabPhoto->size().width()));
    }
}

//打开数据库和数据表
void MainWindow::on_actOpenDB_triggered()
{//"打开"按钮
    QString aFile=QFileDialog::getOpenFileName(this,"选择文件","","SQLite数据库(*.db3)");
    if (aFile.isEmpty())
        return;

    //打开SQLite数据库
    DB=QSqlDatabase::addDatabase("QSQLITE");	//添加 SQLITE数据库驱动
    DB.setDatabaseName(aFile);	//设置数据库名称
    //	DB.setHostName();
    //	DB.setUserName();
    //	DB.setPassword();
    if (!DB.open())
        QMessageBox::warning(this, "错误", "打开数据库失败");
    else
        openTable();	//打开数据表
}


void MainWindow::on_actRecAppend_triggered()
{//添加一条记录
    //// 使用QSqlRecord
    QSqlRecord rec=tabModel->record();      //获取一个空记录，只有字段定义
    rec.setValue(tabModel->fieldIndex("empNo"),2000+tabModel->rowCount());
    rec.setValue(tabModel->fieldIndex("Gender"),"男");  //设置数据
    tabModel->insertRecord(tabModel->rowCount(),rec);   //插入到数据模型的最后

    selModel->clearSelection();
    QModelIndex curIndex=tabModel->index(tabModel->rowCount()-1,1);     //创建最后一行的ModelIndex
    selModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);

    showRecordCount();

    ////项数据模型操作方式,功能相同，但是不符合数据库的操作习惯
    //    tabModel->insertRow(tabModel->rowCount(),QModelIndex());    //在末尾添加一行
    //    QModelIndex curIndex=tabModel->index(tabModel->rowCount()-1,1); //创建最后一行的ModelIndex
    //    theSelection->clearSelection();     //清空选择项
    //    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);    //设置刚插入的行为当前行

    //    int currow=curIndex.row();  //获得当前行编号
    //    tabModel->setData(tabModel->index(currow,tabModel->fieldIndex("empNo")),
    //                      2000+tabModel->rowCount());       //自动生成编号
    //    tabModel->setData(tabModel->index(currow,tabModel->fieldIndex("Gender")),"男");  //必填字段，需要赋值
    //    showRecordCount();

    // 插入行时设置缺省值，需要在primeInsert()信号里去处理
}


void MainWindow::on_actRecInsert_triggered()
{//插入一条记录
    //// 使用QSqlRecord
    QModelIndex curIndex=ui->tableView->currentIndex();
    QSqlRecord rec=tabModel->record();      //获取一个空记录，只有字段定义
    tabModel->insertRecord(curIndex.row(),rec);     //在当前行前面插入一条记录

    selModel->clearSelection();
    selModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);    //设置当前行
    showRecordCount();

    //项数据模型操作方式，功能相同，但是不符合数据库的操作习惯
    //    QModelIndex curIndex=ui->tableView->currentIndex();     //当前行的模型索引
    //    tabModel->insertRow(curIndex.row(),QModelIndex());      //插入一行
    //    theSelection->clearSelection(); //清除已有选择
    //    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);    //选择新插入的一行
    //    showRecordCount();

}

void MainWindow::on_actRevert_triggered()
{//取消修改
    tabModel->revertAll();
    ui->actSubmit->setEnabled(false);
    ui->actRevert->setEnabled(false);
    showRecordCount();
}

void MainWindow::on_actSubmit_triggered()
{//保存修改
    bool res=tabModel->submitAll();
    if (!res)
        QMessageBox::information(this, "消息", "数据保存错误,错误信息\n"+tabModel->lastError().text());
    else
    {
        ui->actSubmit->setEnabled(false);
        ui->actRevert->setEnabled(false);
    }
    showRecordCount();
}

void MainWindow::on_actRecDelete_triggered()
{//删除当前记录
    QModelIndex curIndex=selModel->currentIndex();  //获取当前选择单元格的模型索引
    tabModel->removeRow(curIndex.row()); //删除当前行
    showRecordCount();
}

void MainWindow::on_actPhoto_triggered()
{//设置照片
    QString aFile=QFileDialog::getOpenFileName(this,"选择图片文件","","照片(*.jpg)");
    if (aFile.isEmpty())
        return;

    QByteArray data;
    QFile* file=new QFile(aFile);
    file->open(QIODevice::ReadOnly);    //打开文件
    data = file->readAll();     //读取图片数据
    file->close();
    delete file;

    int curRecNo=selModel->currentIndex().row();    //当前记录的行号
    QSqlRecord  curRec=tabModel->record(curRecNo);      //获取当前记录
    curRec.setValue("Photo",data);  //设置字段数据
    tabModel->setRecord(curRecNo,curRec);   //修改记录

    QPixmap pic;
    pic.load(aFile);  //在界面上显示
    ui->dbLabPhoto->setPixmap(pic.scaledToWidth(ui->dbLabPhoto->width()));
}

void MainWindow::on_actPhotoClear_triggered()
{//清除照片
    int curRecNo=selModel->currentIndex().row();
    QSqlRecord  curRec=tabModel->record(curRecNo);  //获取当前记录

    curRec.setNull("Photo");        //设置为空值
    tabModel->setRecord(curRecNo,curRec);   //修改当前记录

    ui->dbLabPhoto->clear();        //清除QLabel组件的显示
}

void MainWindow::on_radioBtnAscend_clicked()
{//升序排序
    tabModel->setSort(ui->comboFields->currentIndex(),Qt::AscendingOrder);
    tabModel->select();     //setSort()之后需要执行select()才会刷新数据
}

void MainWindow::on_radioBtnDescend_clicked()
{//降序排序
    tabModel->sort(ui->comboFields->currentIndex(),Qt::DescendingOrder);
    //    tabModel->setSort(ui->comboFields->currentIndex(),Qt::DescendingOrder);
    //    tabModel->select();
}

void MainWindow::on_radioBtnMan_clicked()
{
    tabModel->setFilter(" Gender='男' ");
    showRecordCount();
}

void MainWindow::on_radioBtnWoman_clicked()
{
    tabModel->setFilter(" Gender='女' ");
    showRecordCount();
}

void MainWindow::on_radioBtnBoth_clicked()
{
    tabModel->setFilter("");
    showRecordCount();
}

void MainWindow::on_comboFields_currentIndexChanged(int index)
{//选择字段进行排序
    if (ui->radioBtnAscend->isChecked())
        tabModel->setSort(index,Qt::AscendingOrder);
    else
        tabModel->setSort(index,Qt::DescendingOrder);

    tabModel->select();
}

void MainWindow::on_actScan_triggered()
{//涨工资，记录遍历
    if (tabModel->rowCount()==0)
        return;

    for (int i=0;i<tabModel->rowCount();i++)
    {
        QSqlRecord aRec=tabModel->record(i); //获取一条记录
        float salary=aRec.value("Salary").toFloat();
        salary=salary*1.1;
        aRec.setValue("Salary",salary);     //更新记录数据
        tabModel->setRecord(i,aRec);        //更新记录
    }

    // 索引方式刷新记录,速度一样
    //    float   salary;
    //    for (int i=0;i<tabModel->rowCount();i++)
    //    {
    //        salary=tabModel->data(tabModel->index(i,10)).toFloat();
    //        salary=salary*1.1;
    //        tabModel->setData(tabModel->index(i,10),salary);
    //    }

    if (tabModel->submitAll())
        QMessageBox::information(this, "消息", "涨工资数据计算完毕");
}
