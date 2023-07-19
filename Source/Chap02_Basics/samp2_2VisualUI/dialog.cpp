#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(ui->radioBlack,SIGNAL(clicked()),this,SLOT(do_setFontColor()));
    connect(ui->radioRed,  SIGNAL(clicked()),this,SLOT(do_setFontColor()));
    connect(ui->radioBlue, SIGNAL(clicked()),this,SLOT(do_setFontColor()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::do_setFontColor()
{//自定义槽函数，设置文字颜色
    QPalette   plet=ui->plainTextEdit->palette();
    if (ui->radioBlue->isChecked())
        plet.setColor(QPalette::Text,Qt::blue);
    else if (ui->radioRed->isChecked())
        plet.setColor(QPalette::Text,Qt::red);
    else if (ui->radioBlack->isChecked())
        plet.setColor(QPalette::Text,Qt::black);
    else
        plet.setColor(QPalette::Text,Qt::black);

    ui->plainTextEdit->setPalette(plet);
}

void Dialog::on_chkBoxUnder_clicked(bool checked)
{//Underline 复选框
    QFont   font=ui->plainTextEdit->font();
    font.setUnderline(checked);
    ui->plainTextEdit->setFont(font);
}

void Dialog::on_chkBoxItalic_clicked(bool checked)
{//Italic 复选框
    QFont  font=ui->plainTextEdit->font();
    font.setItalic(checked);
    ui->plainTextEdit->setFont(font);
}

void Dialog::on_chkBoxBold_clicked(bool checked)
{// Bold 复选框
    QFont   font=ui->plainTextEdit->font();
    font.setBold(checked);
    ui->plainTextEdit->setFont(font);
}


void Dialog::on_btnClear_clicked()
{//"清空"按钮
    ui->plainTextEdit->clear();
}

