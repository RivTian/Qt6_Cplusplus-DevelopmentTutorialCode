#include "widget.h"
#include "ui_widget.h"

#include    <QMenu>

#include    <QMap>
#include    <QVariant>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

//"初始化列表"按钮
void Widget::on_btnIniItems_clicked()
{
    QIcon   icon;
    icon.addFile(":/images/icons/aim.ico"); //从资源文件中获取图标

    ui->comboBox->clear(); //清除列表
    for (int i=0;i<20;i++)
        ui->comboBox->addItem(icon,QString("Item %1").arg(i)); //带图标
    //        ui->comboBox->addItem(QString("Item %1").arg(i)); //不带图标


    //    QStringList strList;
    //    strList<<"北京"<<"上海"<<"天津"<<"河北省"<<"山东省"<<"山西省";
    //    ui->comboBox->addItems(strList);
}

//“清空列表”按钮
void Widget::on_btnClearItems_clicked()
{
    ui->comboBox->clear();
}

//“可编辑” Checkbox
void Widget::on_chkBoxEditable_clicked(bool checked)
{
    ui->comboBox->setEditable(checked);
}

//“清空文本框”按钮
void Widget::on_btnClearText_clicked()
{
    ui->plainTextEdit->clear();
}

void Widget::on_plainTextEdit_customContextMenuRequested(const QPoint &pos)
{ //创建并显示标准弹出式菜单
    QMenu* menu=ui->plainTextEdit->createStandardContextMenu(); //创建标准右键菜单
    //    menu->exec(QCursor::pos());//在鼠标光标位置显示右键快捷菜单
    menu->exec(pos);//在鼠标光标位置显示右键快捷菜单
}

//初始化具有自定义数据的comboBox
void Widget::on_btnIni2_clicked()
{
    //QMap自动根据 key排序
    QMap<QString, int> City_Zone;
    City_Zone.insert("北京",10);
    City_Zone.insert("上海",21);
    City_Zone.insert("天津",22);
    City_Zone.insert("大连",411);
    City_Zone.insert("锦州",416);
    City_Zone.insert("徐州",516);
    City_Zone.insert("福州",591);
    City_Zone.insert("青岛",532);

    ui->comboBox2->clear();
    foreach(const QString &str,City_Zone.keys())
        ui->comboBox2->addItem(str,City_Zone.value(str));
}

void Widget::on_comboBox_currentTextChanged(const QString &arg1)
{
    ui->plainTextEdit->appendPlainText(arg1);
}

//条目有用户数据
void Widget::on_comboBox2_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QString city=ui->comboBox2->currentText();      //标题
    QString zone=ui->comboBox2->currentData().toString();   //用户数据
    ui->plainTextEdit->appendPlainText(city+": 区号 = "+zone);
}

