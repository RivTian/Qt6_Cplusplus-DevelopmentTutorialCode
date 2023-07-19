#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QFileDialog>
#include    <QTextStream>
#include    <QRegularExpression>       //这个类在Qt 6中已经过时了，换用 QRegularExpression


//从一个StringList 获取数据，初始化数据Model
void MainWindow::openDataFile(QString aFileName)
{
    QStringList aFileContent;
    QFile aFile(aFileName);
    if (aFile.open(QIODevice::ReadOnly | QIODevice::Text)) //以只读文本方式打开文件
    {
        QTextStream aStream(&aFile);  //用文本流读取文件
        ui->plainTextEdit->clear();
        while (!aStream.atEnd())
        {
            QString str=aStream.readLine();     //读取文件的一行
            ui->plainTextEdit->appendPlainText(str);
            aFileContent.append(str);
        }
        aFile.close();

        labCurFile->setText("当前文件："+aFileName);   //状态栏显示
        ui->actAppend->setEnabled(true);
        ui->actInsert->setEnabled(true);
        ui->actDelete->setEnabled(true);
        ui->actSave->setEnabled(true);

        iniModelData(aFileContent);   //从StringList的内容初始化数据模型
    }
}

void MainWindow::iniModelData(QStringList &aFileContent)
{
    int rowCnt=aFileContent.size();    //文本行数，第1行是标题
    m_model->setRowCount(rowCnt-1);    //实际数据行数

    //设置表头
    QString header=aFileContent.at(0);  //第1行是表头
    QStringList headerList=header.split(QRegularExpression("\\s+"),Qt::SkipEmptyParts);
    m_model->setHorizontalHeaderLabels(headerList);    //设置表头文字

    //设置表格数据
    int j;
    QStandardItem   *aItem;
    for (int i=1;i<rowCnt;i++)
    {
        QString aLineText=aFileContent.at(i);   //获取 数据区 的一行
        //一个或多个空格、TAB等分隔符隔开的字符串， 分解为一个StringList
        QStringList tmpList=aLineText.split(QRegularExpression("\\s+"),Qt::SkipEmptyParts);
        for (j=0;j<FixedColumnCount-1;j++)
        { //不包含最后一列
            aItem=new QStandardItem(tmpList.at(j)); //创建item
            m_model->setItem(i-1,j,aItem);          //设置Item
        }

        aItem=new QStandardItem(headerList.at(j));  //最后一列是Checkable
        aItem->setCheckable(true);                  //设置为Checkable
        aItem->setBackground(QBrush(Qt::yellow));
        if (tmpList.at(j)=="0")
            aItem->setCheckState(Qt::Unchecked);    //根据数据设置check状态
        else
            aItem->setCheckState(Qt::Checked);
        m_model->setItem(i-1,j,aItem);     //设置Item
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_model = new QStandardItemModel(2,FixedColumnCount,this); //创建数据模型
    m_selection = new QItemSelectionModel(m_model,this);           //创建选择模型

    //选择当前单元格变化时的信号与槽
    connect(m_selection,&QItemSelectionModel::currentChanged,
            this,&MainWindow::do_currentChanged);
    //    connect(theSelection,SIGNAL(currentChanged(QModelIndex,QModelIndex)),
    //            this,SLOT(do_currentChanged(QModelIndex,QModelIndex)));

    //为tableView设置数据模型
    ui->tableView->setModel(m_model);  //设置数据模型
    ui->tableView->setSelectionModel(m_selection); //设置选择模型
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);

    setCentralWidget(ui->splitter);

    //创建状态栏组件
    labCurFile = new QLabel("当前文件：",this);
    labCurFile->setMinimumWidth(200);

    labCellPos = new QLabel("当前单元格：",this);
    labCellPos->setMinimumWidth(180);
    labCellPos->setAlignment(Qt::AlignHCenter);

    labCellText = new QLabel("单元格内容：",this);
    labCellText->setMinimumWidth(150);

    ui->statusBar->addWidget(labCurFile);
    ui->statusBar->addWidget(labCellPos);
    ui->statusBar->addWidget(labCellText);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{ //选择单元格变化时的响应
    Q_UNUSED(previous);

    if (current.isValid())
    {
        labCellPos->setText(QString::asprintf("当前单元格：%d行，%d列",
                           current.row(),current.column()));    //显示模型索引的行和列号
        QStandardItem *aItem=m_model->itemFromIndex(current);   //从模型索引获得Item
        labCellText->setText("单元格内容："+aItem->text());       //显示item的文字

        QFont   font=aItem->font();
        ui->actFontBold->setChecked(font.bold());

//   为了简化难度，没有设置ActionGroup，不能互斥选择
//        Qt::Alignment align=aItem->textAlignment();
//        ui->actAlignLeft->setChecked(align == Qt::AlignLeft);
//        ui->actAlignCenter->setChecked(align == Qt::AlignHCenter);
//        ui->actAlignRight->setChecked(align == Qt::AlignRight);
    }
}


//打开文件, 文件需要使用UTF-8编码，否则显示汉字乱码
void MainWindow::on_actOpen_triggered()
{
    QString curPath=QCoreApplication::applicationDirPath();
    QString aFileName=QFileDialog::getOpenFileName(this,"打开一个文件",curPath,
                                                   "井数据文件(*.txt);;所有文件(*.*)");
    if (aFileName.isEmpty())
        return;

    openDataFile(aFileName);

//    QStringList aFileContent;  //文件内容字符串列表
//    QFile aFile(aFileName);    //以文件方式读出
//    if (aFile.open(QIODevice::ReadOnly | QIODevice::Text)) //以只读文本方式打开文件
//    {
//        QTextStream aStream(&aFile);  //用文本流读取文件
//        ui->plainTextEdit->clear();   //清空
//        while (!aStream.atEnd())
//        {
//            QString str=aStream.readLine();     //读取文件的一行
//            ui->plainTextEdit->appendPlainText(str);    //添加到文本框显示
//            aFileContent.append(str);   //添加到 StringList
//        }
//        aFile.close();  //关闭文件

//        labCurFile->setText("当前文件："+aFileName);   //状态栏显示
//        ui->actAppend->setEnabled(true);    //更新Actions的enable属性
//        ui->actInsert->setEnabled(true);
//        ui->actDelete->setEnabled(true);
//        ui->actSave->setEnabled(true);

//        iniModelData(aFileContent);   //从StringList的内容初始化数据模型
//    }
}

void MainWindow::on_actAppend_triggered()
{ //在表格最后添加行
    QList<QStandardItem*>    aItemList;     //列表
    QStandardItem   *aItem;
    for(int i=0;i<FixedColumnCount-1;i++)   //不包含最后1列
    {
        aItem=new QStandardItem("0");       //创建Item
        aItemList<<aItem;   //添加到列表
    }

    //获取最后一列的表头文字
    QString str=m_model->headerData(m_model->columnCount()-1,Qt::Horizontal,Qt::DisplayRole).toString();
    aItem=new QStandardItem(str);   //创建 "测井取样"Item
    aItem->setCheckable(true);
    aItemList<<aItem;               //添加到列表

    m_model->insertRow(m_model->rowCount(),aItemList);    //插入一行
    QModelIndex curIndex=m_model->index(m_model->rowCount()-1,0); //创建最后一行的ModelIndex
    m_selection->clearSelection(); //清空选择项
    m_selection->setCurrentIndex(curIndex,QItemSelectionModel::Select);    //设置刚插入的行为当前选择行
}

void MainWindow::on_actInsert_triggered()
{//插入行
    QList<QStandardItem*>    aItemList;  //QStandardItem的列表类
    QStandardItem   *aItem;
    for(int i=0;i<FixedColumnCount-1;i++)
    {
        aItem=new QStandardItem("0"); //新建一个QStandardItem
        aItemList<<aItem;//添加到列表类
    }
//    aItem=new QStandardItem("优"); //新建一个QStandardItem
//    aItemList<<aItem;//添加到列表类

    QString str;    //获取表头文字
    str=m_model->headerData(m_model->columnCount()-1,Qt::Horizontal,Qt::DisplayRole).toString();
    aItem=new QStandardItem(str); //创建Item
    aItem->setCheckable(true);//设置为可使用CheckBox
    aItemList<<aItem;//添加到列表类


    QModelIndex curIndex=m_selection->currentIndex(); //获取当前选中项的模型索引
    m_model->insertRow(curIndex.row(),aItemList);  //在当前行的前面插入一行
    m_selection->clearSelection();//清除已有选择
    m_selection->setCurrentIndex(curIndex,QItemSelectionModel::Select);
}

void MainWindow::on_actDelete_triggered()
{ //删除行
    QModelIndex curIndex=m_selection->currentIndex();//获取当前选择单元格的模型索引

    if (curIndex.row()==m_model->rowCount()-1)//最后一行
        m_model->removeRow(curIndex.row()); //删除最后一行
    else
    {
        m_model->removeRow(curIndex.row());//删除一行，并重新设置当前选择行
        m_selection->setCurrentIndex(curIndex,QItemSelectionModel::Select);
    }
}

void MainWindow::on_actModelData_triggered()
{//模型数据导出到PlainTextEdit显示
    ui->plainTextEdit->clear(); //清空
    QStandardItem   *aItem;
    QString str;
//    int i,j;

    //获取表头文字
    for (int i=0;i<m_model->columnCount();i++)
    {
        aItem=m_model->horizontalHeaderItem(i); //获取表头的一个项数据
        str=str+aItem->text()+"\t"; //用TAB间隔文字
    }
    ui->plainTextEdit->appendPlainText(str); //添加为文本框的一行

    //获取数据区的每行
    for (int i=0;i<m_model->rowCount();i++)
    {
        str="";
        for(int j=0; j<m_model->columnCount()-1;j++)
        {
            aItem=m_model->item(i,j);
            str=str+aItem->text()+QString::asprintf("\t"); //以 TAB分隔
        }

        aItem=m_model->item(i,FixedColumnCount-1); //最后一行是逻辑型
        if (aItem->checkState()==Qt::Checked)
            str=str+"1";
        else
            str=str+"0";

        ui->plainTextEdit->appendPlainText(str);
    }
}

void MainWindow::on_actSave_triggered()
{ //保存为文件
    QString curPath=QCoreApplication::applicationDirPath(); //获取应用程序的路径
//调用打开文件对话框选择一个文件
    QString aFileName=QFileDialog::getSaveFileName(this,tr("选择一个文件"),curPath,
                 "井斜数据文件(*.txt);;所有文件(*.*)");

    if (aFileName.isEmpty()) //未选择文件，退出
        return;

    QFile aFile(aFileName);
    if (!(aFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)))
        return; //以读写、覆盖原有内容方式打开文件

    QTextStream aStream(&aFile); //用文本流读取文件

    QStandardItem   *aItem;
    int i,j;
    QString str;

    ui->plainTextEdit->clear();

//获取表头文字
    for (i=0;i<m_model->columnCount();i++)
    {
        aItem=m_model->horizontalHeaderItem(i); //获取表头的项数据
        str=str+aItem->text()+"\t\t";  //以TAB见隔开
    }
    aStream<<str<<"\n";  //文件里需要加入换行符 \n
    ui->plainTextEdit->appendPlainText(str);

//获取数据区文字
    for ( i=0;i<m_model->rowCount();i++)
    {
        str="";
        for( j=0;j<m_model->columnCount()-1;j++)
        {
            aItem=m_model->item(i,j);
            str=str+aItem->text()+QString::asprintf("\t\t");
        }

        aItem=m_model->item(i,j); //最后一列是逻辑型
        if (aItem->checkState()==Qt::Checked)
            str=str+"1";
        else
            str=str+"0";

         ui->plainTextEdit->appendPlainText(str);
         aStream<<str<<"\n";
    }
}

void MainWindow::on_actAlignCenter_triggered()
{//文字居中对齐
    if (!m_selection->hasSelection())
        return;

    QModelIndexList selectedIndex=m_selection->selectedIndexes();

    QModelIndex aIndex;
    QStandardItem   *aItem;

    for (int i=0;i<selectedIndex.count();i++)
    {
        aIndex=selectedIndex.at(i);
        aItem=m_model->itemFromIndex(aIndex);
        aItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
}

void MainWindow::on_actFontBold_triggered(bool checked)
{//设置字体粗体
    if (!m_selection->hasSelection())
        return;

    //获取选择单元格的模型索引列表
    QModelIndexList selectedIndex=m_selection->selectedIndexes();

    for (int i=0;i<selectedIndex.count();i++)
    {
        QModelIndex aIndex=selectedIndex.at(i); //获取一个模型索引
        QStandardItem* aItem=m_model->itemFromIndex(aIndex);//获取项数据
        QFont font=aItem->font(); //获取字体
        font.setBold(checked); //设置字体是否粗体
        aItem->setFont(font); //重新设置字体
    }
}

void MainWindow::on_actAlignLeft_triggered()
{//设置文字居左对齐
    if (!m_selection->hasSelection()) //没有选择的项
        return;

//获取选择的单元格的模型索引列表，可以是多选
    QModelIndexList IndexList=m_selection->selectedIndexes();

    for (int i=0;i<IndexList.count();i++)
    {
        QModelIndex aIndex=IndexList.at(i); //获取其中的一个模型索引
        QStandardItem* aItem=m_model->itemFromIndex(aIndex);//获取一个单元格的项数据对象
        aItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);//设置文字对齐方式
    }
}

void MainWindow::on_actAlignRight_triggered()
{//设置文字居右对齐
    if (!m_selection->hasSelection())
        return;

    QModelIndexList selectedIndex=m_selection->selectedIndexes();

    QModelIndex aIndex;
    QStandardItem   *aItem;

    for (int i=0;i<selectedIndex.count();i++)
    {
        aIndex=selectedIndex.at(i);
        aItem=m_model->itemFromIndex(aIndex);
        aItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }
}

