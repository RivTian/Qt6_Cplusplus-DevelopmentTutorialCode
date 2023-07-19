#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include    <QMainWindow>

#include    <QLabel>
#include    <QProgressBar>
#include    <QSpinBox>
#include    <QFontComboBox>
#include    <QActionGroup>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QLabel     *labFile;    //状态栏里使用
    QLabel     *labInfo;    //状态栏里使用
    QProgressBar    *progressBar1;  //状态栏上的进度条

    QActionGroup  *actionGroup;     //Action分组，用于“中文”和“英文”的互斥选择

    void    buildUI(); //程序设计的UI初始化
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setLanguage(QString curLan);

private slots:

    void on_textEdit_copyAvailable(bool b);

    void on_textEdit_selectionChanged();


    void on_actFont_Bold_triggered(bool checked);

    void on_actFont_Italic_triggered(bool checked);

    void on_actFont_UnderLine_triggered(bool checked);

    void on_actSys_ToggleText_triggered(bool checked);

    void on_actFile_New_triggered();

    void on_actFile_Open_triggered();

    void on_actFile_Save_triggered();

//    void on_textEdit_customContextMenuRequested(const QPoint &pos);

    void on_actLang_CN_triggered();

    void on_actLang_EN_triggered();

//    void on_actSys_ToggleText_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
