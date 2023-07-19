#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include    <QLabel>
#include    <QStandardItemModel>
#include    <QItemSelectionModel>
#include    <QModelIndex>

#include    "tdialogheaders.h"
#include    "tdialoglocate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    //用于状态栏的信息显示
    QLabel  *labCellPos;    //当前单元格行列号
    QLabel  *labCellText;   //当前单元格内容

    QStandardItemModel  *m_model;      //数据模型
    QItemSelectionModel *m_selection;  //Item选择模型

    TDialogHeaders *dlgSetHeaders=nullptr; //设置表头文字对话框, 一次创建，exec()重复调用

protected:
    void closeEvent(QCloseEvent *event);    //关闭窗口时，询问是否退出

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void    selectCell(int row,int column);    //选择一个单元格,由dlgLocate调用
private slots:
    void do_model_currentChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_actTab_SetSize_triggered();

    void on_actTab_SetHeader_triggered();

    void on_actTab_Locate_triggered();

    void on_tableView_clicked(const QModelIndex &index);

public slots://自定义的公共槽函数
    void    do_setCellText(int row, int column, QString &text); //设置一个单元格的内容
//    void    do_setActLocateEnable(bool enable); //设置actTab_Locate的enabled属性

signals://自定义信号，在tableView上点击时发射此信号
    void    cellIndexChanged(int rowNo, int colNo); //当前单元格发生变化

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
