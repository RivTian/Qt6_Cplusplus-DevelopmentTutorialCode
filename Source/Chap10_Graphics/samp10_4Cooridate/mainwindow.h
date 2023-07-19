#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include    <QMainWindow>
#include    <QGraphicsScene>
#include    <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	QGraphicsScene  *scene; //场景对象

	QLabel  *labViewCord;   //用于在状态栏上显示坐标信息
	QLabel  *labSceneCord;
	QLabel  *labItemCord;

	void iniGraphicsSystem(); //Graphics View结构初始化

protected:
	void resizeEvent(QResizeEvent *event);  //窗口改变大小的事件处理函数

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void    do_mouseMovePoint(QPoint point);    //鼠标移动时处理

	void    do_mouseClicked(QPoint point);      //鼠标左键单击时处理

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
