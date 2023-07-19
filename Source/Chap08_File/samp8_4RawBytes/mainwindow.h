#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFile>
#include <QDataStream>

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QString     m_filename;     //测试用的文件

    QFile       *fileDevice;    //QFile对象
    QDataStream *fileStream;    //QDataStream对象

    bool  iniWrite();           //初始化写文件操作，创建QFile和QDataStream对象
    bool  iniRead();            //初始化读文件操作，创建QFile和QDataStream对象
    void  delFileStream();      //删除QFile和QDataStream对象

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnFile_clicked();

    void on_btnInt8_Write_clicked();

    void on_btnInt8_Read_clicked();

    void on_btnUInt8_Write_clicked();

    void on_btnUInt8_Read_clicked();

    void on_btnInt16_Write_clicked();

    void on_btnInt16_Read_clicked();

    void on_btnUInt16_Write_clicked();

    void on_btnUIn16_Read_clicked();

    void on_btnInt32_Write_clicked();

    void on_btnInt32_Read_clicked();

    void on_btnInt64_Read_clicked();

    void on_btnInt64_Write_clicked();

    void on_btnInt_Write_clicked();

    void on_btnInt_Read_clicked();

    void on_btnBool_Write_clicked();

    void on_btnBool_Read_clicked();

    void on_btnFloat_Write_clicked();

    void on_btnFloat_Read_clicked();

    void on_btnDouble_Write_clicked();

    void on_btnDouble_Read_clicked();

    void on_btnStr_Write_clicked();

    void on_btnStr_Read_clicked();

    void on_actSaveALL_triggered();

    void on_actReadALL_triggered();

    void on_actClearOutput_triggered();

    void on_btnStr_Read2_clicked();

    void on_btnStr_Write2_clicked();

    void on_btnFloat16_Write_clicked();

    void on_btnFloat16_Read_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
