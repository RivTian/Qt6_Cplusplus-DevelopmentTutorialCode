#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>

#include <QKeyEvent>


//根据Qt::DropAction 的枚举值，获取在下拉列表框中的索引
int Widget::getDropActionIndex(Qt::DropAction actionType)
{
    switch (actionType)
    {
    case Qt::CopyAction:
        return 0;

    case Qt::MoveAction:
        return 1;

    case Qt::LinkAction:
        return 2;

    case Qt::IgnoreAction:
        return 3;

    default:
        return  0;
    }
}

//从下拉列表框的索引号，返回Qt::DropAction类型的枚举值
Qt::DropAction Widget::getDropActionType(int index)
{
    switch (index)
    {
    case 0:
        return Qt::CopyAction;

    case 1:
        return Qt::MoveAction;

    case 2:
        return Qt::LinkAction;

    case 3:
        return Qt::IgnoreAction;

    default:
        return  Qt::CopyAction;
    }
}

//组件的属性显示到界面组件上
void Widget::refreshToUI(QGroupBox * curGroupBox)
{
    ui->chkBox_AcceptDrops->setChecked(m_itemView->acceptDrops());  //acceptDrops复选框
    ui->chkBox_DragEnabled->setChecked(m_itemView->dragEnabled());	//dragEnabled复选框

    ui->combo_Mode->setCurrentIndex((int)m_itemView->dragDropMode());   //dragDropMode
    int index=getDropActionIndex(m_itemView->defaultDropAction());
    ui->combo_DefaultAction->setCurrentIndex(index);        //defaultDropAction

    QFont font=ui->groupBox_1->font();
    font.setBold(false);
    ui->groupBox_1->setFont(font);
    ui->groupBox_2->setFont(font);
    ui->groupBox_3->setFont(font);
    ui->groupBox_4->setFont(font);

    font.setBold(true);
    curGroupBox->setFont(font);//当前设置属性的组件所在分组框字体用粗体
}

//事件过滤器处理事件，处理各item组件的删除操作
bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() != QEvent::KeyPress)      //不是KeyPress事件，退出
        return QWidget::eventFilter(watched,event);

    QKeyEvent *keyEvent=static_cast<QKeyEvent *>(event);
    if (keyEvent->key() != Qt::Key_Delete)      //按下的不是Delete键，退出
        return QWidget::eventFilter(watched,event);

    if (watched==ui->listSource)
    {
        QListWidgetItem *item= ui->listSource->takeItem(ui->listSource->currentRow());
        delete  item;
    }
    else if (watched==ui->listWidget)
    {
        QListWidgetItem *item= ui->listWidget->takeItem(ui->listWidget->currentRow());
        delete  item;
    }
    else if (watched==ui->treeWidget)
    {
        QTreeWidgetItem *curItem= ui->treeWidget->currentItem();
        if (curItem->parent() != nullptr)
        {
            QTreeWidgetItem *parItem=curItem->parent();
            parItem->removeChild(curItem);
        }
        else
        {
            int index=ui->treeWidget->indexOfTopLevelItem(curItem);
            ui->treeWidget->takeTopLevelItem(index);
        }
        delete  curItem;    //彻底删除节点
    }
    else if (watched==ui->tableWidget)
    {
        QTableWidgetItem *item= ui->tableWidget->takeItem(ui->tableWidget->currentRow(),
                                                          ui->tableWidget->currentColumn());
        delete  item;

    }
    return true;    //表示事件已经被处理
}

Widget::Widget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //安装事件过滤器，由窗口处理4个item组件的事件
    ui->listSource->installEventFilter(this);
    ui->listWidget->installEventFilter(this);
    ui->treeWidget->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);

    //设置4个item widget组件的拖放操作相关属性
    ui->listSource->setAcceptDrops(true);
    ui->listSource->setDragDropMode(QAbstractItemView::DragDrop);
    ui->listSource->setDragEnabled(true);
    ui->listSource->setDefaultDropAction(Qt::CopyAction);//CopyAction, MoveAction

    ui->listWidget->setAcceptDrops(true);
    ui->listWidget->setDragDropMode(QAbstractItemView::DragDrop);
    ui->listWidget->setDragEnabled(true);
    ui->listWidget->setDefaultDropAction(Qt::CopyAction);//CopyAction, MoveAction

    ui->treeWidget->setAcceptDrops(true);
    ui->treeWidget->setDragDropMode(QAbstractItemView::DragDrop);
    ui->treeWidget->setDragEnabled(true);
    ui->treeWidget->setDefaultDropAction(Qt::CopyAction);//CopyAction, MoveAction

    ui->tableWidget->setAcceptDrops(true);
    ui->tableWidget->setDragDropMode(QAbstractItemView::DragDrop);
    ui->tableWidget->setDragEnabled(true);
    ui->tableWidget->setDefaultDropAction(Qt::MoveAction);//CopyAction, MoveAction
}

Widget::~Widget()
{
    delete ui;
}

//listSource单选按钮
void Widget::on_radio_Source_clicked()
{
    m_itemView=ui->listSource;   //当前设置属性的组件
    refreshToUI(ui->groupBox_1); //属性刷新到界面上
}

//acceptDrops 复选框
void Widget::on_chkBox_AcceptDrops_clicked(bool checked)
{
    m_itemView->setAcceptDrops(checked);
}

//dragEnabled 复选框
void Widget::on_chkBox_DragEnabled_clicked(bool checked)
{
    m_itemView->setDragEnabled(checked);
}

//listWidget单选按钮
void Widget::on_radio_List_clicked()
{
    m_itemView=ui->listWidget;
    refreshToUI(ui->groupBox_2);
}

//treeWidget单选按钮
void Widget::on_radio_Tree_clicked()
{
    m_itemView=ui->treeWidget;
    refreshToUI(ui->groupBox_3);
}

//tableWidget单选按钮
void Widget::on_radio_Table_clicked()
{
    m_itemView=ui->tableWidget;
    refreshToUI(ui->groupBox_4);
}

//dragDropMode 下拉列表框
void Widget::on_combo_Mode_currentIndexChanged(int index)
{
    QAbstractItemView::DragDropMode  mode= (QAbstractItemView::DragDropMode)index;
    m_itemView->setDragDropMode(mode);
}

//defaultDropAction 下拉列表框
void Widget::on_combo_DefaultAction_currentIndexChanged(int index)
{
    Qt::DropAction actionType=getDropActionType(index);
    m_itemView->setDefaultDropAction(actionType);
}

//void Widget::on_chkBox_OverWrite_clicked(bool checked)
//{
//   m_itemView->setDragDropOverwriteMode(checked);
//}
