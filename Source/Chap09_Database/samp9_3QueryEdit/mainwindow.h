#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include    <QtSql>
#include    <QDataWidgetMapper>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QSqlDatabase  DB;   //数据库
    QSqlQueryModel  *qryModel;  //数据模型
    QItemSelectionModel *selModel;  //选择模型

    void    selectData();    //查询数据
    void    updateRecord(int recNo);    //更新一条记录
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool    openDatabase(QString aFile);

private slots:

    // 与选择模型的currentRowChanged()信号关联
    //    void do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_actOpenDB_triggered();

    void on_actRecInsert_triggered();

    void on_actRecDelete_triggered();

    void on_actRecEdit_triggered();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_actScan_triggered();

    void on_actSQLtest_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
