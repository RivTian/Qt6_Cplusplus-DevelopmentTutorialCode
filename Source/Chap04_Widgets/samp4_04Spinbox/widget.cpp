#include    "widget.h"
#include    "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    UIPropertySetup();  //界面组件属性设置

//“数量”和“单价”两个SpinBox的valueChanged()信号与槽函数on_btnCal_clicked()关联
//    connect(ui->spinNum,&QSpinBox::valueChanged,
//                     this,&Widget::on_btnCal_clicked);

//    connect(ui->spinPrice,&QDoubleSpinBox::valueChanged,
//                     this,&Widget::on_btnCal_clicked);

    connect(ui->spinNum,SIGNAL(valueChanged(int)),this,SLOT(on_btnCal_clicked()));
    connect(ui->spinPrice,SIGNAL(valueChanged(double)),this,SLOT(on_btnCal_clicked()));
}

Widget::~Widget()
{
    delete ui;
}

//界面组件属性设置
void Widget::UIPropertySetup()
{
    ui->spinPrice->setDecimals(2);  //显示2位小数
    ui->spinTotal->setDecimals(2);  //显示2位小数

    ui->spinDec->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);    //自适应步长
    ui->spinBin->setRange(0,65535); //设置范围
    ui->spinBin->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);  //水平扩展

    ui->spinBin->setRange(0,65535); //设置范围
    ui->spinBin->setDisplayIntegerBase(2);  //二进制

    ui->spinHex->setRange(0,65535);
    ui->spinHex->setDisplayIntegerBase(16); //十六进制
}



void Widget::on_btnCal_clicked()
{//"计算"按钮
    int num=ui->spinNum->value();       //读取数量，直接是整数
    float price=ui->spinPrice->value(); //读取单价，直接是浮点数
    float total=num*price;
    ui->spinTotal->setValue(total);     //直接显示浮点数
}


void Widget::on_spinDec_valueChanged(int arg1)
{//“十进制”
    qDebug(ui->spinDec->cleanText().toLocal8Bit().data());
    ui->spinBin->setValue(arg1);    //设置整数，自动以二进制显示
    ui->spinHex->setValue(arg1);    //设置整数，自动以十六进制显示
}


void Widget::on_spinBin_valueChanged(int arg1)
{//“二进制”
    qDebug(ui->spinBin->cleanText().toLocal8Bit().data());//数值的二进制字符串
    ui->spinDec->setValue(arg1);
    ui->spinHex->setValue(arg1);
}


void Widget::on_spinHex_valueChanged(int arg1)
{//“十六进制”
    qDebug(ui->spinHex->cleanText().toLocal8Bit().data());//数值的16进制字符串
    ui->spinDec->setValue(arg1);
    ui->spinBin->setValue(arg1);
}

