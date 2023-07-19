#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include    <QMainWindow>
#include    <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    void    setActionsForButton();  //为工具按钮设置关联的Action
    void    createSelectionMenu();   //创建下拉菜单

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actListClear_triggered();  //清除项

    void on_actListIni_triggered(); //项初始化

    void on_chkBoxListEditable_clicked(bool checked);   //chkBoxListEditable单击事件

//当前选择项发生变化
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_listWidget_customContextMenuRequested(const QPoint &pos);   //弹出菜单

    void on_actListInsert_triggered();  //插入项

    void on_actListAppend_triggered();  //添加项

    void on_actListDelete_triggered();  //删除当前项

    void on_actSelALL_triggered(); //全选

    void on_actSelNone_triggered();//全不选

    void on_actSelInvs_triggered();//反选

    void on_tBtnSortAsc_clicked();

    void on_tBtnSortDes_clicked();

    void on_chkBoxSorting_clicked(bool checked);

    void on_listWidget_currentRowChanged(int currentRow);

    void on_listWidget_currentTextChanged(const QString &currentText);

    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_listWidget_itemChanged(QListWidgetItem *item);

    void on_listWidget_itemEntered(QListWidgetItem *item);

    void on_listWidget_itemPressed(QListWidgetItem *item);

    void on_listWidget_itemSelectionChanged();

    void on_tBtnClearText_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_tBtnAddLine_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
