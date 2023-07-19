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

    QSpinBox        *spinFontSize;  //字体大小
    QFontComboBox   *comboFontName; //字体名称

    QActionGroup  *actionGroup;     //Action分组，用于“中文”和“英文”的互斥选择

    void    buildUI(); //程序设计的UI初始化
    void    buildSignalSlots(); //手工关联信号与槽
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //  自定义槽函数
    void do_fontSize_changed(int fontSize);//改变字体大小的SpinBox的响应

    void do_fontSelected(const QFont &font);


    void on_textEdit_copyAvailable(bool b);

    void on_textEdit_selectionChanged();

    void on_actFont_Bold_triggered(bool checked);

    void on_actFont_Italic_triggered(bool checked);

    void on_actFont_UnderLine_triggered(bool checked);

    void on_actSys_ToggleText_triggered(bool checked);

    void on_actFile_New_triggered();

    void on_actFile_Open_triggered();

    void on_actFile_Save_triggered();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
