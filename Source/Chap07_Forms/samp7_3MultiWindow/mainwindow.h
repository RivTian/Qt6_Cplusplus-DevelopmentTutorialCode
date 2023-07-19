#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>

#include    "tformdoc.h"
#include    "tformtable.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void    paintEvent(QPaintEvent *event); //绘制主窗口背景图片

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void do_changeTabTitle(QString title);

    void on_tabWidget_tabCloseRequested(int index);

    void on_actWidgetInsite_triggered();

    void on_actWindowInsite_triggered();

    void on_actWindow_triggered();

    void on_actWidget_triggered();

    void on_tabWidget_currentChanged(int index);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
