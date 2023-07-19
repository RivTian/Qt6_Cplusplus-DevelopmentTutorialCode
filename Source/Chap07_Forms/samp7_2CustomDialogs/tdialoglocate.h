#ifndef TDIALOGLOCATE_H
#define TDIALOGLOCATE_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {class TDialogLocate;}
QT_END_NAMESPACE


class TDialogLocate : public QDialog
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event);    //对话框关闭事件处理函数
    void showEvent(QShowEvent *event);      //对话框显示事件处理函数

public:
    TDialogLocate(QWidget *parent = 0);
    ~TDialogLocate();

    void    setSpinRange(int rowCount, int colCount); //设置最大值,用于初始化

private slots:
    void on_btnSetText_clicked();   //"设置文字"按钮的槽函数

public slots:
    void    setSpinValue(int rowNo, int colNo); //与主窗口的cellIndexChanged()信号关联

signals:
    void    changeCellText(int row, int column, QString &text); //对话框发射的信号，用于定位单元格，并设置文字
    void    changeActionEnable(bool en);    //对话框发射的信号，用于改变action的enable

private:
    Ui::TDialogLocate *ui;
};

#endif // TDIALOGLOCATE_H
