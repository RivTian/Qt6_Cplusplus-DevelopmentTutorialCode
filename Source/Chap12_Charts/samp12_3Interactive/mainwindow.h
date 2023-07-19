#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include    <QtCharts>  //必须这么设置
#include    <QLabel>
#include    <QComboBox>
#include    "tchartview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QChart  *chart;     //图表对象
    QLabel  *lab_chartXY;  //状态栏上的标签
    QLabel  *lab_hoverXY;
    QLabel  *lab_clickXY;

    void  createChart();  //创建图表
    void  prepareData();  //准备数据
    int   getIndexFromX(QXYSeries *series, qreal xValue, qreal tol=0.05);//根据X值返回点的索引

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

//    void do_comboZoomMode_changed(int index);

    void do_legendMarkerClicked();      //图例单击

    void do_mouseMovePoint(QPoint point); //鼠标移动

    void do_series_clicked(const QPointF &point);   //序列被单击

    void do_series_hovered(const QPointF &point, bool state);   //移入或移出序列


    void on_actZoomReset_triggered(); //工具栏按钮，原始大小

    void on_actZoomIn_triggered();   //工具栏按钮，放大

    void on_actZoomOut_triggered();  //工具栏按钮，缩小

    void on_actDeselectAll_triggered();

//    void on_comboBox_currentIndexChanged(int index);

    void on_comboRubberBand_currentIndexChanged(int index);

    void on_comboDragMode_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
