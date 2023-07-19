#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include    "tdicethread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    TDiceThread   *threadA;     //工作线程

protected:
    void    closeEvent(QCloseEvent *event);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void    do_threadA_started();
    void    do_threadA_finished();
    void    do_threadA_newValue(int seq, int diceValue);


//    void on_actStart_triggered();

    void on_actClear_triggered();

    void on_actThread_Run_triggered();

    void on_actThread_Quit_triggered();

    void on_actDice_Run_triggered();

    void on_actDice_Pause_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
