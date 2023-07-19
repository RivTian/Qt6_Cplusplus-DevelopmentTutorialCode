#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include    <QStandardItemModel>
#include    <QItemSelectionModel>
#include    <QBarSet>


#include    <QtCharts>

#define     COL_NAME       0   //姓名的列编号
#define     COL_MATH       1   //数学的列编号
#define     COL_CHINESE    2   //语文的列编号
#define     COL_ENGLISH    3   //英语的列编号
#define     COL_AVERAGE    4   //平均分的列编号


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    int   studCount=10;   //学生人数
    QStandardItemModel  *dataModel;  //数据模型
    void    generateData();     //初始化数据
    void    countData();        //统计数据
    void    removeAllAxis(QChart *chart);   //删除某个chart的所有坐标轴

    void    iniBarChart();          //柱状图初始化
    void    drawBarChart(bool isVertical=true);     //绘制柱状图

    void    iniStackedBar();            //堆积图初始化
    void    drawStackedBar(bool isVertical=true);

    void    iniPercentBar();            //初始化
    void    drawPercentBar(bool isVertical=true);

    void    iniPieChart();      //饼图初始化
    void    drawPieChart();   //更好一些

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //三个分数列的数据发生变化，重新计算平均分
    void do_calcuAverage(QStandardItem *item);
    void do_barHovered(bool status, int index, QBarSet *barset);
    void do_barClicked(int index, QBarSet *barset);
    void do_pieHovered(QPieSlice *slice, bool state);

    void do_legendMarkerClicked();

    //    void on_actTongJi_triggered();

    //    void on_actGenData_triggered();

    void on_btnBuildBarChart_clicked();

    //    void on_cBoxCourse_currentIndexChanged(int index);

    void on_btnDrawPieChart_clicked();

    void on_comboCourse_currentIndexChanged(int index);

    void on_spinHoleSize_valueChanged(double arg1);

    void on_spinPieSize_valueChanged(double arg1);

    void on_btnBuildStackedBar_clicked();

    void on_btnPercentBar_clicked();

    //    void on_btnScatter_clicked();

    //    void on_cBoxTheme_currentIndexChanged(int index);


    void on_btnBuildBarChartH_clicked();

    //    void on_cBoxTheme_BarChart_currentIndexChanged(int index);

    //    void on_toolButton_triggered(QAction *arg1);

    void on_comboTheme_currentIndexChanged(int index);

    void on_comboAnimation_currentIndexChanged(int index);

    //    void on_actCounting_triggered();

    void on_btnBuildStackedBarH_clicked();

    void on_btnPercentBarH_clicked();

    //    void on_toolBtn_GenData_clicked();

    void on_toolBtn_GenData_clicked();

    void on_toolBtn_Counting_clicked();

    void on_chkBox_PieLegend_clicked(bool checked);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
