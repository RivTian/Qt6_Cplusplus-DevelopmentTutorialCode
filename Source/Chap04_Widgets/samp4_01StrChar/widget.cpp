#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


//[2]单个字符特性判断
void Widget::on_btnCharJudge_clicked()
{
    QString str= ui->editChar->text();  //读取QLineEdit组件里输入的字符串
    if (str.isEmpty())
        return;

    QChar ch= str[0];    //只取第1个字符

    char16_t  uniCode= ch.unicode();    //获取QChar字符的UTF-16编码值
    QString chStr(ch);  //将QChar字符转换为QString字符串
    QString info = chStr+ QString::asprintf("\t,Unicode编码= 0x%X\n",uniCode);
    ui->plainTextEdit->appendPlainText(info);


    ui->chkDigit->setChecked(ch.isDigit()); //是否为数字0~9

    ui->chkLetter->setChecked(ch.isLetter());   //是否为字母

    ui->chkLetterOrNumber->setChecked(ch.isLetterOrNumber());   //是否为数字或字母

    ui->chkUpper->setChecked(ch.isUpper());     //是否为大写

    ui->chkLower->setChecked(ch.isLower());     //是否为小写

    ui->chkMark->setChecked(ch.isMark());       //是否为符号

    ui->chkSpace->setChecked(ch.isSpace());     //是否为空格

    ui->chkSymbol->setChecked(ch.isSymbol());   //是否为符号

    ui->chkPunct->setChecked(ch.isPunct());     //是否为标点符号
}

//[1]每个字符的Unicode
void Widget::on_btnGetChars_clicked()
{
    QString str= ui->editStr->text(); //读取QLineEdit组件里输入的字符串
    if (str.isEmpty())  //判断字符串是否为空
        return;

    ui->plainTextEdit->clear();
    for(qint16 i=0; i<str.size(); i++)
    {
        QChar ch= str.at(i);     //获取单个字符,是个QChar字符
        char16_t  uniCode= ch.unicode();    //获取QChar字符的UTF-16编码值

        QString chStr(ch);  //使用构造函数 QString(QChar ch),将QChar字符转换为QString字符串
        QString info = chStr+ QString::asprintf("\t,Unicode编码= 0x%X",uniCode);
        ui->plainTextEdit->appendPlainText(info);
    }
}


//void Widget::on_btnReplaceChar_clicked()
//{
//    QString str="dimple,酒窝";

//    ui->plainTextEdit->clear();
//    ui->plainTextEdit->appendPlainText(str);

//    QChar ch0=str[0];       //ch0='d'
//    QChar ch7=str.at(7);    //ch7='酒'

////    str[0]=QChar('p');      //使用构造函数QChar(char ch)
//    str[0]=QChar::fromLatin1('p');      //使用静态函数
//    str[7]=QChar(0x7C89);   //'粉'的UTF-16编码,
//    str[8]=QChar::fromUcs2(0x523A);   //'刺'

////    str[8]=QChar(0x523A);   //'刺'

//    ui->plainTextEdit->appendPlainText(str);
//}

//[6]清空文本框
void Widget::on_btnClear_clicked()
{
    ui->plainTextEdit->clear();
}


//[3]与Latin-1的转换
void Widget::on_btnConvLatin1_clicked()
{
    QString str="Dimple";
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(str);

    QChar chP=QChar::fromLatin1('P');   //使用静态函数
    //    QChar chP=QChar('P');         //使用类的构造函数
    str[0]=chP;
    ui->plainTextEdit->appendPlainText("\n"+str);
}

//[4]与UTF-16的转换
void Widget::on_btnConvUTF16_clicked()
{
    QString str="Hello,北京";
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(str);    //原来的字符串

    str[6]=QChar(0x9752);   //'青'
    str[7]=QChar::fromUcs2(0x5C9B);   //'岛'
    ui->plainTextEdit->appendPlainText("\n"+str);    //替换汉字后的字符串
}

//[5]QChar比较和替换
void Widget::on_btnCompare_clicked()
{
    QString str="他们来自于河南或河北";
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(str);

    QString HuStr="河to湖";   //临时用字符串
    QChar He=QChar::fromUcs2(HuStr[0].unicode());   //获取‘河’的UTF-16码，再转换为QChar字符
    QChar Hu=QChar(HuStr[3].unicode());   //获取‘湖’的UTF-16，再转换为QChar字符

    for(qint16 i=0; i<str.size(); i++)
    {
        if (str.at(i) == He)  //如果是‘河’
            str[i]=Hu;        //就替换为‘湖’
    }
    ui->plainTextEdit->appendPlainText("\n"+str);
}

