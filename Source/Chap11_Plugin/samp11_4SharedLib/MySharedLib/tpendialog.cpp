#include "tpendialog.h"
#include "ui_tpendialog.h"

#include    <QColorDialog>


TPenDialog::TPenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TPenDialog)
{
    ui->setupUi(this);

    //“线型”ComboBox的选择项设置
    // void addItem(const QString &text, const QVariant &userData = QVariant())
    ui->comboPenStyle->clear();
    ui->comboPenStyle->addItem("NoPen");        //添加字符串
    ui->comboPenStyle->addItem("SolidLine");    //index正好与Qt::PenStyle的枚举值对应
    ui->comboPenStyle->addItem("DashLine");
    ui->comboPenStyle->addItem("DotLine");
    ui->comboPenStyle->addItem("DashDotLine");
    ui->comboPenStyle->addItem("DashDotDotLine");
//    ui->comboPenStyle->addItem("CustomDashLine");

//    ui->comboPenStyle->addItem("NoPen",0);      //添加字符串和用户数据
//    ui->comboPenStyle->addItem("SolidLine",1);
//    ui->comboPenStyle->addItem("DashLine",2);
//    ui->comboPenStyle->addItem("DotLine",3);
//    ui->comboPenStyle->addItem("DashDotLine",4);
//    ui->comboPenStyle->addItem("DashDotDotLine",5);
//    ui->comboPenStyle->addItem("CustomDashLine",6);

    ui->comboPenStyle->setCurrentIndex(1);
}

TPenDialog::~TPenDialog()
{
    delete ui;
}

void TPenDialog::setPen(QPen &pen)
{//初始化Pen，并刷新显示界面
    m_pen=pen;      //保存到成员变量
    ui->spinWidth->setValue(pen.width()); //线宽
    int i=static_cast<int>(pen.style());  //枚举类型转换为整型
    ui->comboPenStyle->setCurrentIndex(i);

    QColor  color=pen.color();
    ui->btnColor->setAutoFillBackground(true); //设置颜色按钮的背景色
    QString str=QString::asprintf("background-color: rgb(%d, %d, %d);",
                                  color.red(),color.green(),color.blue());
    ui->btnColor->setStyleSheet(str);
}

QPen TPenDialog::getPen()
{//获得设置的Pen
    m_pen.setStyle(Qt::PenStyle(ui->comboPenStyle->currentIndex()));    //线型
    m_pen.setWidth(ui->spinWidth->value());     //线宽

    QColor  color;
    color=ui->btnColor->palette().color(QPalette::Button);
    m_pen.setColor(color);  //颜色
    return  m_pen;
}

QPen TPenDialog::getPen(QPen &iniPen, bool *ok)
{ //静态函数，获取QPen
    TPenDialog *dlg=new TPenDialog; //创建一个对话框
    dlg->setPen(iniPen); //设置初始化Pen

    QPen    pen;
    int ret=dlg->exec(); //弹出对话框
    if (ret==QDialog::Accepted)
    {
        pen=dlg->getPen(); //获取
        *ok=true;
    }
    else
    {
        pen=iniPen;
        *ok=false;
    }

    delete  dlg; //删除对话框对象
    return  pen; //返回设置的QPen对象
}

void TPenDialog::on_btnColor_clicked()
{//设置颜色
    QColor  color=QColorDialog::getColor();
    if (color.isValid())
    { //用样式表设置QPushButton的背景色
        QString str=QString::asprintf("background-color: rgb(%d, %d, %d);",
                                      color.red(),color.green(),color.blue());
        ui->btnColor->setStyleSheet(str);
    }
}

