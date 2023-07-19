#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include    <QTimer>
#include    "tdicethread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    TDiceThread   *threadA;     //工作线程
    int m_seq=0,m_diceValue=0;
    QTimer  *timer;     //定时器
protected:
    void    closeEvent(QCloseEvent *event);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void    do_threadA_started();
    void    do_threadA_finished();
    void    do_timeOut();   //定时器的槽函数


    void on_actClear_triggered();

    void on_actThread_Run_triggered();

    void on_actThread_Quit_triggered();

    void on_actDice_Run_triggered();

    void on_actDice_Pause_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
