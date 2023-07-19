#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include    <QMainWindow>

#include    <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QChart      *chart;        //当前的图表
    QLineSeries *curSeries;    //当前序列
    QValueAxis  *curAxis;      //当前坐标轴

    void    createChart();      //创建图表
    void    prepareData();      //更新数据
    void    updateFromChart();  //从图表更新到界面

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actDraw_triggered();

    void on_btnTitleSetText_clicked();

    void on_btnTitleFont_clicked();

    void on_btnSetMargin_clicked();

//    void on_chkBoxPointVisible_clicked(bool checked);

    void on_btnSeriesName_clicked();

    void on_btnSeriesColor_clicked();

    void on_btnSetAxisRange_clicked();

//    void on_spinTickCount_valueChanged(int arg1);

    void on_spinMinorTickCount_valueChanged(int arg1);

    void on_radioAxisX_clicked();

    void on_radioAxisY_clicked();

    void on_checkBox_clicked(bool checked);

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_btnLegendFont_clicked();

    void on_btnLegendlabelColor_clicked();

    //    void on_chkBoxVisible_clicked(bool checked);

    //    void on_btnAxisSetTitle_clicked();

    void on_btnAxisSetTitle_clicked();

    void on_btnAxisTitleFont_clicked();

    //    void on_chkBoxAxisTitle_clicked(bool checked);

    void on_btnAxisLabelFormat_clicked();

    void on_btnAxisLabelColor_clicked();

    void on_btnAxisLabelFont_clicked();

    //    void on_chkBoxLabelsVisible_clicked(bool checked);

    void on_btnGridLineColor_clicked();

    void on_btnGridLinePen_clicked();

    //    void on_chkAxisLineVisible_clicked(bool checked);

    //    void on_pushButton_7_clicked();

    void on_btnAxisLinePen_clicked();

    void on_btnAxisLinePenColor_clicked();

    void on_btnMinorColor_clicked();

    void on_btnMinorPen_clicked();

    void on_radioSeries0_clicked();

    void on_radioSeries1_clicked();

    //    void on_chkSeriesVisible_clicked(bool checked);

    void on_btnSeriesPen_clicked();

    //    void on_horizontalSlider_valueChanged(int value);

    //    void on_sliderOpacity_valueChanged(int value);

    void on_btnSeriesLabColor_clicked();

    void on_btnSeriesLabFont_clicked();

    void on_radioSeriesLabFormat0_clicked();

    void on_radioSeriesLabFormat1_clicked();

    void on_comboAnimation_currentIndexChanged(int index);

    void on_comboTheme_currentIndexChanged(int index);

    void on_actZoomIn_triggered();

    void on_actZoomOut_triggered();

    void on_actZoomReset_triggered();

    void on_groupBox_Legend_clicked(bool checked);

    void on_groupBox_PointLabel_clicked(bool checked);

    //    void on_groupBox_13_clicked(bool checked);

    void on_groupBox_Series_clicked(bool checked);

    void on_sliderOpacity_sliderMoved(int position);

    void on_groupBox_GridLine_clicked(bool checked);

    void on_groupBox_MinorGrid_clicked(bool checked);

    void on_groupBox_Axis_clicked(bool checked);

    void on_groupBox_AxisTitle_clicked(bool checked);

    void on_groupBox_AxisLabel_clicked(bool checked);

    void on_groupBox_Ticks_clicked(bool checked);

    void on_btnTitleColor_clicked();

    void on_btnAxisTitleColor_clicked();

    void on_chkBoxAxisReverse_clicked(bool checked);

    void on_chkkBoxUseOpenGL_clicked(bool checked);

//    void on_groupBox_Legend_toggled(bool arg1);

    void on_groupBox_Points_clicked(bool checked);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_btnPointColor_clicked();

//    void on_btnBGColor_Chart_clicked();

//    void on_radioTick_Fixed_clicked();

    void on_spinTick_Count_valueChanged(int arg1);

//    void on_radioTick_Fixed_clicked(bool checked);

    void on_radioTick_Fixed_toggled(bool checked);

    void on_radioTick_Dynamic_clicked();

    void on_spinTick_Anchor_valueChanged(double arg1);

    void on_spinTick_Intv_valueChanged(double arg1);

//    void on_radioTick_Fixed_clicked();

    void on_radioTick_Fixed_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
