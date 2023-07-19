#ifndef TCHARTVIEW_H
#define TCHARTVIEW_H

#include    <QChartView>


class TChartView : public QChartView
{
    Q_OBJECT

private:
    QPoint  beginPoint; //选择矩形区的起点
    QPoint  endPoint;   //选择矩形区的终点
    bool    m_customZoom=false;   //是否使用自定义矩形放大

protected:
    void mousePressEvent(QMouseEvent *event);   //鼠标左键按下
    void mouseReleaseEvent(QMouseEvent *event); //鼠标释放左键

    void mouseMoveEvent(QMouseEvent *event);    //鼠标移动
    void keyPressEvent(QKeyEvent *event);       //按键事件

    void wheelEvent(QWheelEvent *event);        //鼠标滚轮事件,缩放

public:
    TChartView(QWidget *parent = nullptr);
    ~TChartView();

    void  setCustomZoomRect(bool custom);   //设置是否使用自定义矩形放大模式

signals:
    void mouseMovePoint(QPoint point);      //鼠标移动信号
};

#endif // TCHARTVIEW_H
