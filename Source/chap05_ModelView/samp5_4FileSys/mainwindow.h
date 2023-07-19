#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include    <QMainWindow>
#include    <QFileSystemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QFileSystemModel   *m_model;     //数据模型变量

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_actSetRoot_triggered();

    void on_btnApplyFilters_clicked();

    void on_chkBoxEnableFilter_clicked(bool checked);

    void on_radioShowAll_clicked();

    void on_radioShowOnlyDir_clicked();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
