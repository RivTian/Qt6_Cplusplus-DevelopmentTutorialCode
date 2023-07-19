#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->lab,SIGNAL(doubleClicked()),
            this,SLOT(do_doubleClick()));
}

Widget::~Widget()
{
    delete ui;
}

//双击窗口时的响应
void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    ui->lab->setText("窗口被双击了");
    ui->lab->adjustSize();
}

//双击标签时的响应
void Widget::do_doubleClick()
{
    ui->lab->setText("标签被双击了，信号的槽函数响应");
    ui->lab->adjustSize();
}

