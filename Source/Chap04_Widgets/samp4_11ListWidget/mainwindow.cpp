#include    "mainwindow.h"
#include    "ui_mainwindow.h"
#include    <QMenu>


//为QToolButton按钮设置Action
void MainWindow::setActionsForButton()
{
    ui->tBtnListIni->setDefaultAction(ui->actListIni);          //初始化列表
    ui->tBtnListClear->setDefaultAction(ui->actListClear);      //清除列表
    ui->tBtnListInsert->setDefaultAction(ui->actListInsert);    //插入项
    ui->tBtnListAppend->setDefaultAction(ui->actListAppend);    //添加项
    ui->tBtnListDelete->setDefaultAction(ui->actListDelete);    //删除当前项

    ui->tBtnSelALL->setDefaultAction(ui->actSelALL);        //全选
    ui->tBtnSelNone->setDefaultAction(ui->actSelNone);      //空选
    ui->tBtnSelInvs->setDefaultAction(ui->actSelInvs);      //反选
}

void MainWindow::createSelectionMenu()
{
//创建下拉菜单
    QMenu* menuSelection=new QMenu(this);       //创建下拉菜单
    menuSelection->addAction(ui->actSelALL);    //全选
    menuSelection->addAction(ui->actSelNone);   //空选
    menuSelection->addAction(ui->actSelInvs);   //反选

//listWidget上方的tBtnSelectItem按钮
    ui->tBtnSelectItem->setPopupMode(QToolButton::MenuButtonPopup);     //菜单弹出模式，执行按钮的Action
    ui->tBtnSelectItem->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->tBtnSelectItem->setDefaultAction(ui->actSelPopMenu);    //关联Action
    ui->tBtnSelectItem->setMenu(menuSelection);                 //设置下拉菜单

//工具栏上的下拉式菜单按钮
    QToolButton  *aBtn=new QToolButton(this);
    aBtn->setPopupMode(QToolButton::InstantPopup);      //立刻下拉菜单，按钮关联的Action不会被执行
    aBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    aBtn->setDefaultAction(ui->actSelPopMenu);          //关联Action
    aBtn->setMenu(menuSelection);       //设置下拉菜单
    ui->mainToolBar->addWidget(aBtn);   //工具栏上添加按钮

//工具栏添加分隔条和“退出”按钮
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actQuit);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->splitter); //设置中心组件
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);    //定制快捷菜单
    setActionsForButton();
    createSelectionMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//“清除列表”Action
void MainWindow::on_actListClear_triggered()
{
    ui->listWidget->clear();
}

//“初始化列表”Action
void MainWindow::on_actListIni_triggered()
{
    QListWidgetItem *aItem;     //每一行是一个QListWidgetItem

    QIcon aIcon;
    aIcon.addFile(":/images/icons/check2.ico");     //设置图标
    bool chk=ui->chkBoxListEditable->isChecked();   //是否可编辑

    ui->listWidget->clear();    //清除列表
    for (int i=0; i<10; i++)
    {
        QString str=QString("Item %1").arg(i);
        aItem=new QListWidgetItem(); //新建一个项

        aItem->setText(str);    //设置文字标签
        aItem->setIcon(aIcon);  //设置图标
        aItem->setCheckState(Qt::Checked);  //设置为选中状态
        if (chk)    //可编辑, 设置flags
            aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
        else        //不可编辑, 设置flags
            aItem->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
        ui->listWidget->addItem(aItem);     //添加一个项
    }
}

void MainWindow::on_chkBoxListEditable_clicked(bool checked)
{ //可编辑 QCheckBox的响应代码， 设置所有项是否可编辑
    int i,cnt;
    QListWidgetItem *aItem;

    cnt=ui->listWidget->count();//项的个数
    for (i=0; i<cnt; i++)
    {
        aItem=ui->listWidget->item(i);//获得一个项
        if (checked) //可编辑
            aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
        else
            aItem->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
    }

}

//listWidget的currentItemChanged()信号
void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    QString str;
    if (current != NULL)        //需要检测指针是否为空
    {
        if (previous==NULL)     //需要检测指针是否为空
            str="当前："+current->text();
        else
            str="前一项："+previous->text()+"; 当前项："+current->text();
        ui->editCutItemText->setText(str);
    }
    ui->plainTextEdit->appendPlainText("currentItemChanged()信号被发射");
}

//“插入项”Action
void MainWindow::on_actListInsert_triggered()
{
    //    QIcon aIcon;
    //    aIcon.addFile(":/images/icons/check2.ico");  //图标
    QIcon aIcon(":/images/icons/check2.ico");   //定义图标变量并直接赋值

    bool chk=ui->chkBoxListEditable->isChecked(); //是否可编辑

    QListWidgetItem* aItem=new QListWidgetItem("Inserted Item"); //创建一个项
    aItem->setIcon(aIcon);      //设置图标
    aItem->setCheckState(Qt::Checked);  //设置为checked
    if (chk)    //可编辑，设置标志
        aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
    else
        aItem->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);

    ui->listWidget->insertItem(ui->listWidget->currentRow(),aItem); //在当前行的前面插入一个项
}

//“添加项”Action
void MainWindow::on_actListAppend_triggered()
{
    QIcon aIcon(":/images/icons/check2.ico");       //定义图标
    bool chk=ui->chkBoxListEditable->isChecked();   //是否可编辑

    QListWidgetItem* aItem=new QListWidgetItem("Added Item"); //创建一个Item
    aItem->setIcon(aIcon);              //设置图标
    aItem->setCheckState(Qt::Checked);  //设置为checked
    if (chk)    //可编辑，设置标志
        aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
    else
        aItem->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);

    ui->listWidget->addItem(aItem);     //添加一个项
}

//“删除当前项”Action
void MainWindow::on_actListDelete_triggered()
{
    int row=ui->listWidget->currentRow();   //当前行号

    QListWidgetItem* aItem=ui->listWidget->takeItem(row); //移除指定行的项，但不delete
    delete aItem;   //需要手工删除对象

//    QListWidgetItem *QListWidget::takeItem(int row)
//    Removes and returns the item from the given row in
//            the list widget; otherwise returns 0.
//    Items removed from a list widget will not be managed by Qt,
//            and will need to be deleted manually.
}


void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu* menuList=new QMenu(this); //创建菜单

    //添加Actions创建菜单项
    menuList->addAction(ui->actListIni);
    menuList->addAction(ui->actListClear);
    menuList->addAction(ui->actListInsert);
    menuList->addAction(ui->actListAppend);
    menuList->addAction(ui->actListDelete);
    menuList->addSeparator();
    menuList->addAction(ui->actSelALL);
    menuList->addAction(ui->actSelNone);
    menuList->addAction(ui->actSelInvs);

    menuList->exec(QCursor::pos()); //在鼠标光标位置显示快捷菜单

    delete menuList;    //菜单显示完后，需要删除对象
}

//"全选"Action
void MainWindow::on_actSelALL_triggered()
{
    int cnt=ui->listWidget->count();    //项的总数
    for (int i=0; i<cnt; i++)
    {
        QListWidgetItem *aItem=ui->listWidget->item(i);
        aItem->setCheckState(Qt::Checked);   //设置为选中
    }
}

//"空选"Action
void MainWindow::on_actSelNone_triggered()
{
    QListWidgetItem *aItem;

    int cnt=ui->listWidget->count();
    for (int i=0; i<cnt; i++)
    {
        aItem=ui->listWidget->item(i);
        aItem->setCheckState(Qt::Unchecked);
    }
}

//"反选"Action
void MainWindow::on_actSelInvs_triggered()
{
    QListWidgetItem *aItem;
    int cnt=ui->listWidget->count();    //项的总数
    for (int i=0; i<cnt; i++)
    {
        aItem=ui->listWidget->item(i);  //获取一个项
        if (aItem->checkState()!=Qt::Checked)
            aItem->setCheckState(Qt::Checked);
        else
            aItem->setCheckState(Qt::Unchecked);
    }
}

void MainWindow::on_tBtnSortAsc_clicked()
{
    ui->listWidget->sortItems(Qt::AscendingOrder);      //升序排序
}


void MainWindow::on_tBtnSortDes_clicked()
{
    ui->listWidget->sortItems(Qt::DescendingOrder);     //降序排序
}


void MainWindow::on_chkBoxSorting_clicked(bool checked)
{
    ui->listWidget->setSortingEnabled(checked);         //是否允许排序
}


void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    ui->plainTextEdit->appendPlainText(QString("currentRowChanged()信号被发射，currentRow=%1").arg(currentRow));
}


void MainWindow::on_listWidget_currentTextChanged(const QString &currentText)
{
    ui->plainTextEdit->appendPlainText("currentTextChanged()信号被发射，currentText="+currentText);
}


void MainWindow::on_listWidget_itemActivated(QListWidgetItem *item)
{
    ui->plainTextEdit->appendPlainText("itemActivated()信号被发射，"+item->text());
}


void MainWindow::on_listWidget_itemChanged(QListWidgetItem *item)
{
    ui->plainTextEdit->appendPlainText("itemChanged()信号被发射，"+item->text());
}


void MainWindow::on_listWidget_itemEntered(QListWidgetItem *item)
{
    ui->plainTextEdit->appendPlainText("itemEntered()信号被发射，"+item->text());
}


void MainWindow::on_listWidget_itemPressed(QListWidgetItem *item)
{
    ui->plainTextEdit->appendPlainText("itemPressed()信号被发射，"+item->text());
}


void MainWindow::on_listWidget_itemSelectionChanged()
{
    ui->plainTextEdit->appendPlainText("itemSelectionChanged()信号被发射");
}


void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->plainTextEdit->appendPlainText("itemClicked()信号被发射，"+item->text());
}

void MainWindow::on_tBtnClearText_clicked()
{
    ui->plainTextEdit->clear();
}

void MainWindow::on_tBtnAddLine_clicked()
{
    ui->plainTextEdit->appendPlainText("");
}


void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    ui->plainTextEdit->appendPlainText("itemClicked()信号被发射，"+item->text());
}

