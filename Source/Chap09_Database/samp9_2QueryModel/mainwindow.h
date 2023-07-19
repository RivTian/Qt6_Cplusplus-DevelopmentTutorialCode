#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include    <QMainWindow>

#include    <QtSql>
#include    <QDataWidgetMapper>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
//    QMap<QString, int> fieldIndex;  //字段名与字段序号的数据字典

    QSqlDatabase  DB;   //数据库

    QSqlQueryModel  *qryModel;  //数据模型

    QItemSelectionModel *selModel;  //选择模型

    QDataWidgetMapper   *dataMapper;    //数据界面映射

    void    selectData();       //查询数据
    void    refreshTableView(); //移动记录时刷新tableView的当前行
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool    openDatabase(QString aFile);

private slots:

    // 与选择模型的currentRowChanged()信号关联
    void do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

    //5个Actions的槽函数
    void on_actOpenDB_triggered();

    void on_actRecFirst_triggered();

    void on_actRecPrevious_triggered();

    void on_actRecNext_triggered();

    void on_actRecLast_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
