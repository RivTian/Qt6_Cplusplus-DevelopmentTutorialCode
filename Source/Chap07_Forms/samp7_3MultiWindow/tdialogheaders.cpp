#include "tdialogheaders.h"
#include "ui_tdialogheaders.h"
#include    <QMessageBox>

TDialogHeaders::TDialogHeaders(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TDialogHeaders)
{
    ui->setupUi(this);
    m_model= new QStringListModel(this); //创建模型
    ui->listView->setModel(m_model);  //与QListView组件构成Model/View结构
}

TDialogHeaders::~TDialogHeaders()
{
    QMessageBox::information(this,"提示","TDialogHeaders对话框被删除");
    delete ui;
}

void TDialogHeaders::setHeaderList(QStringList &headers)
{//设置模型的字符串列表
    m_model->setStringList(headers);
}

QStringList TDialogHeaders::headerList()
{//返回模型的字符串列表
    return  m_model->stringList();
}
