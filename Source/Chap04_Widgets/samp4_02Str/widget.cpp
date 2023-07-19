#include "widget.h"
#include "ui_widget.h"

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

void Widget::showBoolInfo(QString str, bool CheckValue)
{
    if(CheckValue)
        ui->plainTextEdit->appendPlainText(str+"= true");
    else
        ui->plainTextEdit->appendPlainText(str+"= false");
}


void Widget::on_pushButton_3_clicked()
{//contains()函数
    ui->plainTextEdit->appendPlainText("\n===contains()函数测试");

    QString str1=ui->comboBox1->currentText();
    QString str2=ui->comboBox2->currentText();
    ui->plainTextEdit->appendPlainText(str1);
    ui->plainTextEdit->appendPlainText(str2);

    bool chk=str1.contains(str2);
    showBoolInfo("contains()= ", chk);
}

void Widget::on_pushButton_4_clicked()
{//count(),length(),size()函数
    ui->plainTextEdit->appendPlainText("\n===size(),count(),length()函数测试");

    QString str1=ui->comboBox1->currentText();
    ui->plainTextEdit->appendPlainText(str1);

    int N=str1.size();
    ui->plainTextEdit->appendPlainText(QString::asprintf("size()=%d",N));

    N=str1.count();
    ui->plainTextEdit->appendPlainText(QString::asprintf("count()=%d",N));

    N=str1.length();
    ui->plainTextEdit->appendPlainText(QString::asprintf("length()=%d",N));
}


void Widget::on_pushButton_6_clicked()
{//endsWith()函数
    ui->plainTextEdit->appendPlainText("\n===endsWith(),startsWith()函数测试");

    QString str1=ui->comboBox1->currentText();
    QString str2=ui->comboBox2->currentText();
    ui->plainTextEdit->appendPlainText("str1= " +str1);
    ui->plainTextEdit->appendPlainText("str2= " +str2);

    bool chk=str1.endsWith(str2);
    showBoolInfo("endsWith= ", chk);

    chk=str1.startsWith(str2);
    showBoolInfo("startsWith= ", chk);
}

void Widget::on_pushButton_7_clicked()
{//indexOf()函数

    ui->plainTextEdit->appendPlainText("\n===indexOf(),lastIndexOf()函数测试");

    QString str1=ui->comboBox1->currentText();
    QString str2=ui->comboBox2->currentText();
    ui->plainTextEdit->appendPlainText("str1= " +str1);
    ui->plainTextEdit->appendPlainText("str2= " +str2);

    int N=str1.indexOf(str2);
    QString info=QString::asprintf("indexOf()= %d",N);
    ui->plainTextEdit->appendPlainText(info);

    N=str1.lastIndexOf(str2);
    info=QString::asprintf("lastIndexOf()= %d",N);
    ui->plainTextEdit->appendPlainText(info);
}

void Widget::on_pushButton_13_clicked()
{//trimmed()函数
    ui->plainTextEdit->appendPlainText("\n===trimmed(),simplified()函数测试");
    QString str1=ui->comboBox1->currentText();

    ui->plainTextEdit->appendPlainText("str1= " +str1);

    QString str2=str1.trimmed();
    ui->plainTextEdit->appendPlainText("trimmed()= " +str2);

    str2=str1.simplified();
    ui->plainTextEdit->appendPlainText("simplified()= " +str2);
}

void Widget::on_pushButton_14_clicked()
{//section()函数
    ui->plainTextEdit->appendPlainText("\n===section()函数测试");

    QString str1=ui->comboBox1->currentText();
    int N=ui->spinBox->value();
    QString str2=ui->comboBox2->currentText();  //用于分隔的字符

    QString str3;
    if (QString::compare(str2,"\\",Qt::CaseInsensitive)==0)
        str3=str1.section('\\',N,N); //字符‘\’需要单独处理
    else
        str3=str1.section(str2,N,N); //其他分隔字符，如‘,’

    ui->plainTextEdit->appendPlainText(str3);
}

void Widget::on_pushButton_15_clicked()
{//left(), right()函数
	ui->plainTextEdit->appendPlainText("\n===left(), right()函数测试");

//	QString str1="G:\\Qt6Book\\QtSamples\\qw.cpp";
    QString str1=ui->comboBox1->currentText();
	ui->plainTextEdit->appendPlainText("str1= "+str1);

	QString str2=str1.left(2);
	ui->plainTextEdit->appendPlainText("left(2)= "+str2);

	str2=str1.right(6);
	ui->plainTextEdit->appendPlainText("right(6)= "+str2);
}

void Widget::on_pushButton_18_clicked()
{//isNull(),isEmpty函数
    ui->plainTextEdit->appendPlainText("\n===isNull()函数测试");

    QString str1, str2="";
    ui->plainTextEdit->appendPlainText("QString str1, str2=""");
    showBoolInfo("str1.isNull()", str1.isNull());
    showBoolInfo("str1.isEmpty()",str1.isEmpty());

    showBoolInfo("\nstr2.isNull()",str2.isNull());
    showBoolInfo("str2.isEmpty()", str2.isEmpty());

    str2.clear();
    ui->plainTextEdit->appendPlainText("\nstr2.clear()后");
    showBoolInfo("\nstr2.isNull()",str2.isNull());
    showBoolInfo("str2.isEmpty()", str2.isEmpty());
}

//front()函数，获取第1个zi符
void Widget::on_pushButton_20_clicked()
{
    ui->plainTextEdit->appendPlainText("\n===front(), back()函数测试");

    QString str1=ui->comboBox1->currentText();
    ui->plainTextEdit->appendPlainText("str1= "+str1);

    QChar  ch=str1.front();
    QString str2=QString(ch);
    ui->plainTextEdit->appendPlainText("front()= "+str2);

    ch=str1.back();
    str2=QString(ch);
    ui->plainTextEdit->appendPlainText("back()= "+str2);
}


void Widget::on_pushButton_22_clicked()
{// count()，统计子字符串出现的次数
    ui->plainTextEdit->appendPlainText("\n===count()统计子字符串出现的次数");

    QString str1=ui->comboBox1->currentText();
    QString str2=ui->comboBox2->currentText();
    ui->plainTextEdit->appendPlainText(str1);
    ui->plainTextEdit->appendPlainText(str2);

    int N=str1.count(str2,Qt::CaseSensitive);
    ui->plainTextEdit->appendPlainText(QString::asprintf("count()= %d",N));
}

void Widget::on_pushButton_23_clicked()
{//first()和last()函数
    ui->plainTextEdit->appendPlainText("\n===first(), last()函数测试");

    QString str1=ui->comboBox1->currentText();  //获取str1输入
    QString str2=ui->comboBox2->currentText();  //获取str2输入
    ui->plainTextEdit->appendPlainText("str1= " +str1);
    ui->plainTextEdit->appendPlainText("str2= " +str2);

    int N= str1.lastIndexOf(str2);    //str2出现的最后位置
    QString str3= str1.first(N+1);    //路径名称,带最后的"\"
    ui->plainTextEdit->appendPlainText(str3);

    str3= str1.last(str1.size()-N-1);   //去除路径的文件名
    ui->plainTextEdit->appendPlainText(str3);
}



void Widget::on_pushButton_26_clicked()
{//resize()函数

    ui->plainTextEdit->appendPlainText("\n===resize()函数测试");

    QString str1;
    str1.resize(5,'0');       //str1="00000"
    ui->plainTextEdit->appendPlainText(str1);

    str1.resize(10,QChar(0x54C8));   //str1="00000哈哈哈哈哈", 0x54C8是‘哈’的Unicode编码
    ui->plainTextEdit->appendPlainText(str1);
}


void Widget::on_pushButton_2_clicked()
{// fill()函数

	ui->plainTextEdit->appendPlainText("\n===fill()函数测试");

	QString str1="Hello";
	str1.fill('X');			//str1=”XXXXX”
	ui->plainTextEdit->appendPlainText(str1);

    str1.fill('A',2);		//str1=”AA”，改变了长度
    ui->plainTextEdit->appendPlainText(str1);

    str1.fill(QChar(0x54C8),3);		//str1=”哈哈哈”, 0x54C8是‘哈’的UTF-16编码
    ui->plainTextEdit->appendPlainText(str1);
}


void Widget::on_pushButton_clicked()
{// insert()
    ui->plainTextEdit->appendPlainText("\n===insert()函数测试");
    QString str1="It is great";
    ui->plainTextEdit->appendPlainText(str1);

    int N= str1.lastIndexOf(" ");
//    ui->plainTextEdit->appendPlainText(QString::asprintf("N= %d",N));
    str1.insert(N, "n't");
    ui->plainTextEdit->appendPlainText(str1);

//    ui->edtResult->setText(str1);
}


void Widget::on_pushButton_19_clicked()
{
//==== replace(),替换指定的字符
    ui->plainTextEdit->appendPlainText("\n===replace(),替换指定的字符");
    QString str1="Goooogle";
    ui->plainTextEdit->appendPlainText(str1);

    str1.replace('o','e');
    ui->plainTextEdit->appendPlainText(str1);

//==== replace()，替换字符串
    ui->plainTextEdit->appendPlainText("\n===replace(),替换字符串");
    str1="It is great";
    int N= str1.lastIndexOf(" ");
    ui->plainTextEdit->appendPlainText(str1);

    QString subStr="wonderful";
    str1.replace(N+1,subStr.size(), subStr);    //str1="It is wonderful"
    ui->plainTextEdit->appendPlainText(str1);

    str1.replace(N+1, subStr.size(),"OK!");     //str1="It is OK!"
    qDebug(str1.toLocal8Bit().data());
    ui->plainTextEdit->appendPlainText(str1);
}


void Widget::on_pushButton_28_clicked()
{
//==== remove()移除特定的字符
    ui->plainTextEdit->appendPlainText("\n===remove()移除特定的字符");
    QString str1="你们，我们，他们";
    ui->plainTextEdit->appendPlainText("str1= "+str1);

    QString DeStr="们";
    QChar   DeChar=QChar(DeStr[0].unicode());   //获取汉字'的'的Unicode码，再创建QChar对象
    str1.remove(DeChar);        //str1="你，我，他"
    ui->plainTextEdit->appendPlainText("str1= "+str1);


//==== remove()
    ui->plainTextEdit->appendPlainText("\n===remove()移除右侧N个字符");
    str1="G:\\Qt6Book\\QtSamples\\qw.cpp";    //用到转移字符'\\'
    ui->plainTextEdit->appendPlainText(str1);

    int N=str1.lastIndexOf("\\");
    str1.remove(N+1, 20);       //str1="G:\Qt6Book\QtSamples\"
    ui->plainTextEdit->appendPlainText(str1);

}


void Widget::on_btnClearText_clicked()
{
    ui->plainTextEdit->clear();
}

