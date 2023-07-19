#ifndef TDIALOGSIZE_H
#define TDIALOGSIZE_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {class TDialogSize;}
QT_END_NAMESPACE

class TDialogSize : public QDialog
{
    Q_OBJECT

public:
    TDialogSize(QWidget *parent = 0);
    ~TDialogSize();

    int rowCount();     //获取对话框输入的行数
    int columnCount();  //获取对话框输入的列数
    void    setRowColumn(int row, int column); //初始对话框上两个SpinBox的值

private slots:

private:
    Ui::TDialogSize *ui;
};

#endif // TDIALOGSIZE_H
