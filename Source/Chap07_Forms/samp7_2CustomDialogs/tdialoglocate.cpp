#include "tdialoglocate.h"
#include "ui_tdialoglocate.h"

#include    <QCloseEvent>
#include    "mainwindow.h"
#include    <QMessageBox>

void TDialogLocate::closeEvent(QCloseEvent *event)
{ //窗口关闭
    event->accept();
    emit changeActionEnable(true);  //发射信号
}

void TDialogLocate::showEvent(QShowEvent *event)
{//窗口显示
    event->accept();
    emit changeActionEnable(false); //发射信号
}

TDialogLocate::TDialogLocate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TDialogLocate)
{
    ui->setupUi(this);
}

TDialogLocate::~TDialogLocate()
{
    QMessageBox::information(this,"提示","TDialogLocate对话框被删除");
    delete ui;
}


void TDialogLocate::setSpinRange(int rowCount, int colCount)
{//设置SpinBox输入最大值
    ui->spinBoxRow->setMaximum(rowCount-1);
    ui->spinBoxColumn->setMaximum(colCount-1);
}

void TDialogLocate::setSpinValue(int rowNo, int colNo)
{//关联主窗口的cellIndexChanged()信号，更新spinBox的值
    ui->spinBoxRow->setValue(rowNo);
    ui->spinBoxColumn->setValue(colNo);
}

void TDialogLocate::on_btnSetText_clicked()
{//定位到单元格，并设置文字
    int row=ui->spinBoxRow->value();        //行号
    int col=ui->spinBoxColumn->value();     //列号
    QString text=ui->edtCaption->text();    //文字

    emit changeCellText(row,col,text);  //发射信号
    if (ui->chkBoxRow->isChecked())     //行增
        ui->spinBoxRow->setValue(1+ui->spinBoxRow->value());

    if (ui->chkBoxColumn->isChecked())  //列增
        ui->spinBoxColumn->setValue(1+ui->spinBoxColumn->value());
}
