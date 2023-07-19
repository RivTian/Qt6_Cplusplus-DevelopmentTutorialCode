#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
   Q_OBJECT

private:
   QString   m_filename;        //用于测试的文件

   template<class T> void writeByStream(T Value);
   template<class T> void readByStream(T &Value);

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

   void on_btnQStr_Write_clicked();

   void on_btnQStr_Read_clicked();

   void on_btnFont_In_clicked();

   void on_btnFont_Write_clicked();

   void on_btnFont_Read_clicked();

   void on_btnColor_In_clicked();

   void on_btnColor_Write_clicked();

   void on_btnColor_Read_clicked();

   void on_actSaveALL_triggered();

   void on_actReadALL_triggered();

   void on_btnStr_Write_clicked();

   void on_btnStr_Read_clicked();

   void on_actClearOutput_triggered();

private:
   Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
