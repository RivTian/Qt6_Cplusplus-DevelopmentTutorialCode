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
	bool    openByIO_Whole(const QString &aFileName);   //用QFile打开文件，整体读取
	bool    openByIO_Lines(const QString &aFileName);   //用QFile打开文件，逐行读取
	bool    saveByIO_Whole(const QString &aFileName);   //用QFile保存文件，整体保存
//    bool    saveByIO_Document(const QString& aFileName);//用QFile保存文件，用QTextDocument逐段保存，不成功
	bool    saveByIO_Safe(const QString &aFileName);    //用QSaveFile保存文件，用QTextDocument逐段保存

	bool    openByStream_Whole(const QString &aFileName);   //用QTextStream读取文件，整体读取
	bool    openByStream_Lines(const QString &aFileName);   //用QTextStream读取文件，逐行读取

	bool    saveByStream_Whole(const QString &aFileName);
	bool    saveByStream_Safe(const QString &aFileName);
public:
	MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actOpen_IODevice_triggered();

    void on_actOpen_TextStream_triggered();

    void on_actSave_IODevice_triggered();

    void on_actSave_TextStream_triggered();

    void on_actSave_TextSafe_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
