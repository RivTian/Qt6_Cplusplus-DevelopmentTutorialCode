#include    "widget.h"
#include    "ui_widget.h"
#include    <QMetaProperty>
#include    <QCheckBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    boy=new TPerson("王小明", this);
    boy->setProperty("score",95);   //设置属性值
    boy->setProperty("age",10);
    boy->setProperty("sex","Boy");  //sex是动态属性
    connect(boy,SIGNAL(ageChanged(int)),this,SLOT(do_ageChanged(int)));
//    connect(boy,&TPerson::ageChanged,this,&Widget::do_ageChanged);

    girl=new TPerson("张小丽",this);
    girl->setProperty("score",81);  //设置属性值
    girl->setProperty("age",20);
    girl->setProperty("sex","Girl");//sex是动态属性
    connect(girl,&TPerson::ageChanged,this,&Widget::do_ageChanged);

    ui->spinBoy->setProperty("isBoy",true);         //isBoy是动态属性
    ui->spinGirl->setProperty("isBoy",false);       //isBoy是动态属性

    connect(ui->spinGirl,SIGNAL(valueChanged(int)), this,SLOT(do_spinChanged(int)));
    connect(ui->spinBoy, &QSpinBox::valueChanged,  this,&Widget::do_spinChanged);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::do_ageChanged( int value)
{
    Q_UNUSED(value);
    TPerson *person = qobject_cast<TPerson *>(sender());   //获取信号发射者

    QString hisName=person->property("name").toString();   //姓名
    QString hisSex=person->property("sex").toString();     //动态属性是QVariant
//    int hisAge=person->age();          //通过接口函数获取年龄
    int hisAge=person->property("age").toInt();        //通过属性获得年龄
    QString str=QString("%1, %2, 年龄=%3").arg(hisName).arg(hisSex).arg(hisAge);
    ui->textEdit->appendPlainText(str);
}

void Widget::on_btnClear_clicked()
{//"清空文本框"按钮
    ui->textEdit->clear();
}

void Widget::on_btnBoyInc_clicked()
{//"boy长大一岁"按钮
    boy->incAge();
}

void Widget::on_btnGirlInc_clicked()
{//"girl长大一岁"按钮
    girl->incAge();
}

//响应界面上spinBox的valueChanged(int)信号
void Widget::do_spinChanged(int arg1)
{
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(sender());   //获取信号发射者
    if (spinBox->property("isBoy").toBool())   //根据动态属性判断是哪个SpinBox
        boy->setAge(arg1);
    else
        girl->setAge(arg1);
}

void Widget::on_btnClassInfo_clicked()
{//"类的元对象信息"按钮
    QObject *obj=boy;
//    QObject *obj=girl;
//    QObject *obj=ui->spinBoy;
    const QMetaObject *meta=obj->metaObject();
    ui->textEdit->clear();
    ui->textEdit->appendPlainText(QString("类名称：%1\n").arg(meta->className()));

    ui->textEdit->appendPlainText("property");
    for (int i=meta->propertyOffset();i<meta->propertyCount();i++)
    {
        const char* propName=meta->property(i).name();
        QString propValue=obj->property(propName).toString();
        QString str=QString("属性名称=%1，属性值=%2").arg(propName).arg(propValue);
        ui->textEdit->appendPlainText(str);
    }
    //获取类信息
    ui->textEdit->appendPlainText("");
    ui->textEdit->appendPlainText("classInfo");
    for (int i=meta->classInfoOffset();i<meta->classInfoCount();++i)
    {
        QMetaClassInfo classInfo=meta->classInfo(i);
        ui->textEdit->appendPlainText(
                    QString("Name=%1; Value=%2").arg(classInfo.name()).arg(classInfo.value()));
    }
}

