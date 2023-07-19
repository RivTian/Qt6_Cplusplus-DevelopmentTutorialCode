#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QTreeWidgetItem>
#include    <QFileDialog>


//重新构造TreeWidget的表头
void MainWindow::buildTreeHeader()
{
    ui->treeFiles->clear();     //清除所有节点，但是不改变表头

    QTreeWidgetItem*  header=new QTreeWidgetItem();     //创建节点
    header->setText(MainWindow::colItem,     "目录和文件");
    header->setText(MainWindow::colItemType, "节点类型");
    header->setText(MainWindow::colDate,     "最后修改日期");

    header->setTextAlignment(colItem,       Qt::AlignHCenter | Qt::AlignVCenter);
    header->setTextAlignment(colItemType,   Qt::AlignHCenter | Qt::AlignVCenter);

    ui->treeFiles->setHeaderItem(header);
}


//初始化目录树，创建一个顶层节点
void MainWindow::iniTree()
{
    QIcon  icon(":/images/icons/15.ico");
    QTreeWidgetItem*  item=new QTreeWidgetItem(MainWindow::itTopItem); //节点类型为itTopItem
    item->setIcon(MainWindow::colItem,icon);          //设置第1列的图标
    item->setText(MainWindow::colItem, "图片");     //设置第1列的文字
    item->setText(MainWindow::colItemType, "Top Item");     //设置第2列的文字
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    item->setCheckState(MainWindow::colItem, Qt::Checked);  //设置为选中

//    item->setData(MainWindow::colItem,Qt::UserRole,QVariant(dataStr)); //设置节点第1列的Qt::UserRole的Data
    ui->treeFiles->addTopLevelItem(item);//添加顶层节点
}

//添加一个分组节点
void MainWindow::addFolderItem(QTreeWidgetItem *parItem, QString dirName)
{
    QIcon   icon(":/images/icons/open3.bmp");
    QString NodeText=getFinalFolderName(dirName);   //从一个完整目录名称里，获取最后的文件夹名称

    QTreeWidgetItem *item=new QTreeWidgetItem(MainWindow::itGroupItem); //设定type为itGroupItem
    item->setIcon(colItem,icon); //设置图标
    item->setText(colItem,NodeText); //最后的文件夹名称，第1列
    item->setText(colItemType,"Group Item"); //完整目录名称，第2列

    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable
                   | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate); //设置节点选项
    item->setCheckState(colItem,Qt::Checked); //节点选中
    item->setData(colItem,Qt::UserRole,QVariant(dirName)); //设置角色为Qt::UserRole的Data,存储完整目录名称

    parItem->addChild(item);  //在父节点下面添加子节点
}

//从一个完整目录名称里，获取最后的文件夹名称
QString MainWindow::getFinalFolderName(const QString &fullPathName)
{
    int cnt=fullPathName.length();              //字符串长度
    int i=fullPathName.lastIndexOf("/");        //最后一次出现的位置
    QString str=fullPathName.right(cnt-i-1);    //获得最后的文件夹的名称
    return str;
}

//添加一个图片文件节点
void MainWindow::addImageItem(QTreeWidgetItem *parItem, QString aFilename)
{
    QIcon   icon(":/images/icons/31.ico");

    QFileInfo fileInfo(aFilename);      //QFileInfo对象
    QString NodeText=fileInfo.fileName();          //不带路径的文件名
    QDateTime birthDate=fileInfo.lastModified();   //文件的创建日期

//    QTreeWidgetItem *item;
    QTreeWidgetItem *item=new QTreeWidgetItem(MainWindow::itImageItem); //节点类型为itImageItem
    item->setIcon(colItem,icon);
    item->setText(colItem,NodeText);            //第1列文字
    item->setText(colItemType,"Image Item");    //第2列文字
    item->setText(colDate,birthDate.toString("yyyy-MM-dd"));    //第3列文字
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable
                   | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    item->setCheckState(colItem,Qt::Checked);

    item->setData(colItem,Qt::UserRole,QVariant(aFilename)); //设置用户数据,存储完整文件名称
    parItem->addChild(item);  //在父节点下面添加子节点
}

//显示图片,节点item的data()存储了图片文件名
void MainWindow::displayImage(QTreeWidgetItem *item)
{
    QString filename=item->data(colItem,Qt::UserRole).toString();   //获取节点data里存的文件名
    labFileName->setText(filename);             //状态栏显示
    labNodeText->setText(item->text(colItem));  //状态栏显示
    m_pixmap.load(filename);       //从文件载入图片
    ui->actZoomFitH->trigger();     //触发Action的trigged()信号，执行其关联的槽函数

    ui->actZoomFitH->setEnabled(true);
    ui->actZoomFitW->setEnabled(true);
    ui->actZoomIn->setEnabled(true);
    ui->actZoomOut->setEnabled(true);
    ui->actZoomRealSize->setEnabled(true);
}

//改变节点的标题文字
void MainWindow::changeItemCaption(QTreeWidgetItem *item)
{
    QString str="*"+item->text(colItem);    //节点文字前加“*”
    item->setText(colItem,str);             //设置节点文字

    if (item->childCount()>0)   //如果有子节点
        for (int i=0;i<item->childCount();i++)  //遍历子节点
            changeItemCaption(item->child(i));  //递归调用自己
}

//彻底删除一个节点及其子节点，递归调用的函数
void MainWindow::deleteItem(QTreeWidgetItem *parItem, QTreeWidgetItem *item)
{
    if (item->childCount()>0)   //如果有子节点,需要先删除所有子节点
    {
        int count=item->childCount();       //子节点个数
        QTreeWidgetItem *tempParItem=item;  //临时父节点
        for (int i=count-1; i>=0; i--)      //遍历子节点
            deleteItem(tempParItem, tempParItem->child(i));   //递归调用自己
    }

    //删除完子节点之后，再删除自己
    parItem->removeChild(item); //移除节点
    delete  item;               //从内存中删除节点
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//创建状态栏上的组件
    labNodeText=new QLabel("节点标题",this);
    labNodeText->setMinimumWidth(200);
    ui->statusBar->addWidget(labNodeText);

    spinRatio=new QSpinBox(this);   //用于显示图片缩放比例的QSpinBox组件
    spinRatio->setRange(0,2000);
    spinRatio->setValue(100);
    spinRatio->setSuffix(" %");
    spinRatio->setReadOnly(true);
    spinRatio->setButtonSymbols(QAbstractSpinBox::NoButtons);  //不显示上下调节按钮
    ui->statusBar->addPermanentWidget(spinRatio);

    labFileName=new QLabel("文件名",this);
    ui->statusBar->addPermanentWidget(labFileName);

//初始化目录树
    buildTreeHeader();  //重新构建目录树表头
    iniTree();          //初始化目录树

    setCentralWidget(ui->scrollArea); //设置中心布局组件
}

MainWindow::~MainWindow()
{
   delete ui;
}

//当前节点选择变化时触发
void MainWindow::on_treeFiles_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    qDebug("currentItemChanged() is emitted");
    if (current==nullptr)         //当前节点为空
        return;

    if (current == previous)     //没有切换节点，只是列变化
        return;

    int var=current->type();    //节点的类型
    switch(var)
    {
    case  itTopItem:        //顶层节点
        ui->actAddFolder->setEnabled(true);
        ui->actAddFiles->setEnabled(true);
        ui->actDeleteItem->setEnabled(false);    //顶层节点不允许删除
        break;

    case  itGroupItem:      //文件组节点
        ui->actAddFolder->setEnabled(true);
        ui->actAddFiles->setEnabled(true);
        ui->actDeleteItem->setEnabled(true);
        break;

    case  itImageItem:      //图片文件节点
        ui->actAddFolder->setEnabled(false);    //图片节点下不能添加目录节点
        ui->actAddFiles->setEnabled(true);
        ui->actDeleteItem->setEnabled(true);
        displayImage(current);                  //显示图片
    }
}

// 选择一个文件夹,作为当前节点的子节点加入
void MainWindow::on_actAddFolder_triggered()
{
    QString dir= QFileDialog::getExistingDirectory();    //选择目录
    if (dir.isEmpty())  //目录名称为空
        return;

    QTreeWidgetItem *parItem= ui->treeFiles->currentItem();
    if (parItem == nullptr) //当前节点为空
        return;

    if (parItem->type() != itImageItem)   //图片节点不能添加分组节点
        addFolderItem(parItem,dir);       //在父节点下面添加一个分组节点
}

//添加图片文件节点
void MainWindow::on_actAddFiles_triggered()
{
    QStringList files= QFileDialog::getOpenFileNames(this,"选择文件","","Images(*.jpg)");
    if (files.isEmpty())    //一个文件都没选
        return;

    QTreeWidgetItem *parItem, *item;
    item= ui->treeFiles->currentItem();  //当前节点

    if (item == nullptr)    //如果是空节点
        item= ui->treeFiles->topLevelItem(0);   //取顶层节点

    if (item->type() == itImageItem)  //若当前节点是图片节点，取其父节点作为父节点
        parItem= item->parent();
    else                            //否则取当前节点为父节点
        parItem= item;

    for (int i = 0; i < files.size(); ++i)  //文件列表
    {
        QString aFilename= files.at(i);      //获取一个文件名
        addImageItem(parItem,aFilename);    //添加一个图片节点
    }

    parItem->setExpanded(true);     //展开父节点
}

//缩小显示
void MainWindow::on_actZoomOut_triggered()
{
    m_ratio=m_ratio*0.8;      //在当前比例基础上乘以0.8
    spinRatio->setValue(100*m_ratio);

    int w=m_ratio*m_pixmap.width();   //显示宽度
    int h=m_ratio*m_pixmap.height();  //显示高度

    QPixmap pix=m_pixmap.scaled(w,h);  //图片缩放到指定高度和宽度，保持长宽比例

    ui->labPic->setPixmap(pix);
}

//放大显示
void MainWindow::on_actZoomIn_triggered()
{
    m_ratio=m_ratio*1.2;          //在当前比例基础上乘以0.8
    spinRatio->setValue(100*m_ratio);  //状态栏上显示缩放百分比

    int w=m_ratio*m_pixmap.width();   //显示宽度
    int h=m_ratio*m_pixmap.height();  //显示高度

    QPixmap pix=m_pixmap.scaled(w,h);  //图片缩放到指定高度和宽度，保持长宽比例
    ui->labPic->setPixmap(pix);
}

//适应宽度显示
void MainWindow::on_actZoomFitW_triggered()
{
    int w=ui->scrollArea->width()-20;   //得到scrollArea的高度
    int realw=m_pixmap.width();        //原始图片的实际宽度
    m_ratio=float(w)/realw;            //当前显示比例，必须转换为浮点数

    spinRatio->setValue(m_ratio*100);

    QPixmap pix=m_pixmap.scaledToWidth(w-30);
    ui->labPic->setPixmap(pix);
}

//适应高度显示图片
void MainWindow::on_actZoomFitH_triggered()
{
    int H=ui->scrollArea->height(); //得到scrollArea的高度
    int realH=m_pixmap.height();   //原始图片的实际高度
    m_ratio=float(H)/realH;        //当前显示比例，必须转换为浮点数

    spinRatio->setValue(100*m_ratio);  //状态栏上显示缩放百分比

    QPixmap pix=m_pixmap.scaledToHeight(H-30);     //图片缩放到指定高度
    ui->labPic->setPixmap(pix);     //设置Label的PixMap
}

//实际大小显示
void MainWindow::on_actZoomRealSize_triggered()
{
    m_ratio=1;
    spinRatio->setValue(100);
    ui->labPic->setPixmap(m_pixmap);
}

//删除节点当前节点及其子节点
void MainWindow::on_actDeleteItem_triggered()
{
    QTreeWidgetItem *item= ui->treeFiles->currentItem(); //当前节点
    if(item == nullptr)
        return;

    QTreeWidgetItem *parItem= item->parent();   //当前节点的父节点
    deleteItem(parItem, item);

//下面这2行代码是有问题的，如果item有子节点，其子节点将不能被彻底删除
//    parItem->removeChild(item);
//    delete item;
}

//遍历节点
void MainWindow::on_actScanItems_triggered()
{
    for (int i=0; i<ui->treeFiles->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item=ui->treeFiles->topLevelItem(i);   //顶层节点
        changeItemCaption(item);        //更改节点标题
    }
}

void MainWindow::on_dockWidget_visibilityChanged(bool visible)
{//停靠区可见性变化
    ui->actDockVisible->setChecked(visible);
}

void MainWindow::on_dockWidget_topLevelChanged(bool topLevel)
{//停靠区浮动性变化
    ui->actDockFloat->setChecked(topLevel);
}

void MainWindow::on_actDockFloat_triggered(bool checked)
{//停靠区浮动性
    ui->dockWidget->setFloating(checked);
}

void MainWindow::on_actDockVisible_triggered(bool checked)
{//停靠区的可见性
    ui->dockWidget->setVisible(checked);
}

void MainWindow::on_treeFiles_itemChanged(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    qDebug("itemChanged() is emitted");
}


void MainWindow::on_treeFiles_itemCollapsed(QTreeWidgetItem *item)
{
    Q_UNUSED(item);
    qDebug("itemCollapsed() is emitted");
}


void MainWindow::on_treeFiles_itemExpanded(QTreeWidgetItem *item)
{
    Q_UNUSED(item);
    qDebug("itemExpanded() is emitted");
}

void MainWindow::on_treeFiles_itemSelectionChanged()
{
    qDebug("itemSelectionChanged() is emitted");
}

void MainWindow::on_treeFiles_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    qDebug("itemClicked() is emitted");
}


void MainWindow::on_treeFiles_itemPressed(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    qDebug("itemPressed() is emitted");
}



