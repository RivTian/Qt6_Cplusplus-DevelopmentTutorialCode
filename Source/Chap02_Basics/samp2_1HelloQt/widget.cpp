#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
//    ui->labDemo->setText("欢迎使用Qt6");
//    ui->btnClose->setText("关闭");
}

Widget::~Widget()
{
    delete ui;
}

