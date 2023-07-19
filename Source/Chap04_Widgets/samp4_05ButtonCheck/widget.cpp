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


void Widget::on_btnAlign_Left_clicked()
{//居左
    ui->editInput->setAlignment(Qt::AlignLeft);
}

void Widget::on_btnAlign_Center_clicked()
{//居中
    ui->editInput->setAlignment(Qt::AlignCenter);
}

void Widget::on_btnAlign_Right_clicked()
{//居右
    ui->editInput->setAlignment(Qt::AlignRight);
}

void Widget::on_btnFont_Bold_clicked(bool checked)
{//粗体
    QFont font= ui->editInput->font();
    font.setBold(checked);
    ui->editInput->setFont(font);
}

void Widget::on_btnFont_Italic_clicked(bool checked)
{//斜体
    QFont font= ui->editInput->font();
    font.setItalic(checked);
    ui->editInput->setFont(font);
}

void Widget::on_btnFont_UnderLine_clicked(bool checked)
{//下划线
    QFont font= ui->editInput->font();
    font.setUnderline(checked);
    ui->editInput->setFont(font);
}

void Widget::on_chkBox_Readonly_clicked(bool checked)
{//Readonly
    ui->editInput->setReadOnly(checked);
}

void Widget::on_chkbox_Enable_clicked(bool checked)
{//Enabled
    ui->editInput->setEnabled(checked);
}

void Widget::on_chkBox_ClearButton_clicked(bool checked)
{//ClearButtonEnabled
    ui->editInput->setClearButtonEnabled(checked);
}

void Widget::on_radioBlack_clicked()
{//Black 单选按钮
    QPalette plet= ui->editInput->palette();
    plet.setColor(QPalette::Text, Qt::black);
    ui->editInput->setPalette(plet);
}


void Widget::on_radioRed_clicked()
{//Red 单选按钮
    QPalette plet= ui->editInput->palette();
    plet.setColor(QPalette::Text, Qt::red);
    ui->editInput->setPalette(plet);
}


void Widget::on_radioBlue_clicked()
{//Blue 单选按钮
    QPalette plet= ui->editInput->palette();
    plet.setColor(QPalette::Text, Qt::blue);
    ui->editInput->setPalette(plet);
}

