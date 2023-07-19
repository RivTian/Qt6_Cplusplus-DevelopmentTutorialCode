#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include    <QGraphicsScene>
#include    <QLabel>
#include    <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    const   quint32 boundValue=100;     //随机数上限值
    //    QRandomGenerator* random;           //随机数发生器

    const int ItemId = 1;            //绘图项自定义数据的key
    const int ItemDesciption = 2;    //绘图项自定义数据的key

    int seqNum=0;   //用于图形项的编号，每个图形项有一个编号
    int backZ=0;    //用于bring to front
    int frontZ=0;   //用于bring to back

    QGraphicsScene  *scene;     //场景

    QLabel  *labViewCord;       //用于状态栏
    QLabel  *labSceneCord;
    QLabel  *labItemCord;
    QLabel  *labItemInfo;

    void    setItemProperty(QGraphicsItem* item, QString desciption);   //设置图形项的属性

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //自定义槽函数
    void    do_mouseMovePoint(QPoint point);    //鼠标移动

    void    do_mouseClicked(QPoint point);      //鼠标单击

    void    do_mouseDoubleClick(QPoint point);  //鼠标双击

    void    do_keyPress(QKeyEvent *event);      //按键


    void on_actItem_Rect_triggered();

    void on_actItem_Ellipse_triggered();

    void on_actItem_Polygon_triggered();

    void on_actEdit_Delete_triggered();

    void on_actZoomIn_triggered();

    void on_actZoomOut_triggered();

    void on_actRestore_triggered();

    void on_actRotateLeft_triggered();

    void on_actRotateRight_triggered();

    void on_actEdit_Front_triggered();

    void on_actEdit_Back_triggered();

    void on_actItem_Line_triggered();

    void on_actItem_Text_triggered();

    void on_actGroup_triggered();

    void on_actGroupBreak_triggered();

    void on_actItem_Circle_triggered();

    void on_actItem_Triangle_triggered();

    //    void on_actBackBrush_triggered();

    void on_actHelp_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
