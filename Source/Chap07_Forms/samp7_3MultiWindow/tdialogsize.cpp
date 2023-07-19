#include "tdialogsize.h"
#include "ui_tdialogsize.h"

#include    <QMessageBox>

TDialogSize::TDialogSize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TDialogSize)
{
    ui->setupUi(this);
}

TDialogSize::~TDialogSize()
{
    QMessageBox::information(this,"提示","TDialogSize对话框被删除");
    delete ui;
}

int TDialogSize::rowCount()
{ //用于主窗口调用获得行数的输入值
    return  ui->spinBoxRow->value();
}

int TDialogSize::columnCount()
{//用于主窗口调用获得列数的输入值
    return  ui->spinBoxColumn->value();
}

void TDialogSize::setRowColumn(int row, int column)
{ //初始化数据显示
    ui->spinBoxRow->setValue(row);
    ui->spinBoxColumn->setValue(column);
}

