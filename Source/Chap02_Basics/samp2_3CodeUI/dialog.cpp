#include    "dialog.h"
#include    <QHBoxLayout>
#include    <QVBoxLayout>

void Dialog::iniUI()
{
//创建 Underline, Italic, Bold三个CheckBox，并水平布局
    chkBoxUnder=new QCheckBox("Underline");
    chkBoxItalic=new QCheckBox("Italic");
    chkBoxBold=new QCheckBox("Bold");
    QHBoxLayout *HLay1=new QHBoxLayout();
    HLay1->addWidget(chkBoxUnder);
    HLay1->addWidget(chkBoxItalic);
    HLay1->addWidget(chkBoxBold);

//创建 Black, Red, Blue三个RadioButton，并水平布局
    radioBlack=new QRadioButton("Black");
    radioBlack->setChecked(true);           //缺省被选中
    radioRed=new QRadioButton("Red");
    radioBlue=new QRadioButton("Blue");
    QHBoxLayout *HLay2=new QHBoxLayout;
    HLay2->addWidget(radioBlack);
    HLay2->addWidget(radioRed);
    HLay2->addWidget(radioBlue);

//创建 确定, 取消, 退出 三个 PushButton, 并水平布局
    btnOK=new QPushButton("确定");
    btnCancel=new QPushButton("取消");
    btnClose=new QPushButton("退出");
    QHBoxLayout *HLay3=new QHBoxLayout;
    HLay3->addStretch();
    HLay3->addWidget(btnOK);
    HLay3->addWidget(btnCancel);
    HLay3->addStretch();
    HLay3->addWidget(btnClose);

//创建 文本框,并设置初始字体
    txtEdit=new QPlainTextEdit;
    txtEdit->setPlainText("Hello world\n手工创建");
    QFont   font=txtEdit->font();   //获取字体
    font.setPointSize(20);          //修改字体大小为20
    txtEdit->setFont(font);         //设置字体

//创建 垂直布局，并设置为主布局
    //    VLay=new QVBoxLayout(this);
    QVBoxLayout *VLay=new QVBoxLayout(this);
    VLay->addLayout(HLay1);         //添加字体类型组
    VLay->addLayout(HLay2);         //添加字体颜色组
    VLay->addWidget(txtEdit);       //添加PlainTextEdit
    VLay->addLayout(HLay3);         //添加按键组
    setLayout(VLay);    //设置为窗口的主布局
}

void Dialog::iniSignalSlots()
{
//三个设置颜色的 QRadioButton
    connect(radioBlue,SIGNAL(clicked()),this,SLOT(do_setFontColor()));
    connect(radioRed,SIGNAL(clicked()),this,SLOT(do_setFontColor()));
    connect(radioBlack,SIGNAL(clicked()),this,SLOT(do_setFontColor()));

//三个设置字体的 QCheckBox
    connect(chkBoxUnder,SIGNAL(clicked(bool)),this,SLOT(do_chkBoxUnder(bool)));
    connect(chkBoxItalic,SIGNAL(clicked(bool)),this,SLOT(do_chkBoxItalic(bool)));
    connect(chkBoxBold,SIGNAL(clicked(bool)),this,SLOT(do_chkBoxBold(bool)));

//三个按钮与窗口的槽函数关联
    connect(btnOK,SIGNAL(clicked()),this,SLOT(accept()));
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
    connect(btnClose,SIGNAL(clicked()),this,SLOT(close()));
}

void Dialog::do_chkBoxUnder(bool checked)
{
    QFont   font=txtEdit->font();
    font.setUnderline(checked);
    txtEdit->setFont(font);
}

void Dialog::do_chkBoxItalic(bool checked)
{
    QFont   font=txtEdit->font();
    font.setItalic(checked);
    txtEdit->setFont(font);
}

void Dialog::do_chkBoxBold(bool checked)
{
    QFont   font=txtEdit->font();
    font.setBold(checked);
    txtEdit->setFont(font);
}

void Dialog::do_setFontColor()
{
    QPalette   plet=txtEdit->palette();
    if (radioBlue->isChecked())
        plet.setColor(QPalette::Text,Qt::blue);
    else if (radioRed->isChecked())
        plet.setColor(QPalette::Text,Qt::red);
    else if (radioBlack->isChecked())
        plet.setColor(QPalette::Text,Qt::black);
    else
        plet.setColor(QPalette::Text,Qt::black);

    txtEdit->setPalette(plet);
}

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    iniUI();            //界面创建与布局
    iniSignalSlots();   //信号与槽的关联
    setWindowTitle("手工创建UI");   //设置窗口标题
}

Dialog::~Dialog()
{
}
