#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include    <QtDataVisualization>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget         *graphContainer;    //图表的容器
    Q3DSurface      *graph3D;           //三维图表
    QSurface3DSeries    *series;        //序列
    //    QSurfaceDataProxy   *dataProxy;     //数据代理

    void    iniGraph3D();       //初始化绘制三维图表

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void do_pointSelected(const QPoint &position);

    void on_chkBoxGrid_clicked(bool checked);

    void on_chkBoxSmooth_clicked(bool checked);

    void on_chkBoxReflection_clicked(bool checked);

    void on_chkBoxAxisTitle_clicked(bool checked);

    void on_chkBoxAxisBackground_clicked(bool checked);

    void on_chkBoxReverse_clicked(bool checked);

    void on_chkBoxBackground_clicked(bool checked);

    void on_spinFontSize_valueChanged(int arg1);

    void on_cBoxTheme_currentIndexChanged(int index);

    //    void on_btnBarColor_clicked();

    void on_chkBoxItemLabel_clicked(bool checked);

    void on_cBoxBarStyle_currentIndexChanged(int index);

    void on_cBoxSelectionMode_currentIndexChanged(int index);

    //    void on_spinItemSize_valueChanged(double arg1);

    void on_chkBoxShadow_clicked(bool checked);

    //    void on_comboBox_currentIndexChanged(int index);

    void on_sliderH_valueChanged(int value);

    void on_sliderV_valueChanged(int value);

    void on_comboDrawMode_currentIndexChanged(int index);

    //    void on_btnGrad1_clicked();

    //    void on_btnGrad2_clicked();

    //    void on_pushButton_clicked();

    //    void on_pushButton_clicked();

    void on_sliderZoom_valueChanged(int value);

    void on_comboCamera_currentIndexChanged(int index);

    void on_chkBoxFlatShading_clicked(bool checked);

    void on_actSurf_Color_triggered();

    void on_actSurf_GradColor1_triggered();

    void on_actSurf_GradColor2_triggered();

    //    void on_actSurf_Texture_triggered();

    void on_actPoint_Modify_triggered();

    void on_actPoint_DeleteRow_triggered();

    //    void on_cBoxSelectionMode_2_currentIndexChanged(int index);

    void on_btnResetCamera_clicked();

    void on_btnMoveUp_clicked();

    void on_btnMoveLeft_clicked();

    void on_btnMoveRight_clicked();

    void on_btnMoveDown_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
