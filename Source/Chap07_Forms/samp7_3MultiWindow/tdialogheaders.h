#ifndef TDIALOGHEADERS_H
#define TDIALOGHEADERS_H

#include <QDialog>

#include    <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui {class TDialogHeaders;}
QT_END_NAMESPACE

class TDialogHeaders : public QDialog
{
    Q_OBJECT

private:
    QStringListModel  *m_model;   //存储字符串列表数据

public:
    TDialogHeaders(QWidget *parent = 0);
    ~TDialogHeaders();

    void    setHeaderList(QStringList& headers);    //设置字符串列表
    QStringList headerList();   //返回字符串列表
private:
    Ui::TDialogHeaders *ui;
};

#endif // TDIALOGHEADERS_H
