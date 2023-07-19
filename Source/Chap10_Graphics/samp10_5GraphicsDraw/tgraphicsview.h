#ifndef TGRAPHICSVIEW_H
#define TGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>

class TGraphicsView : public QGraphicsView
{
    Q_OBJECT

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

public:
    TGraphicsView(QWidget *parent = nullptr);

signals:
    void mouseMovePoint(QPoint point);      //鼠标移动
    void mouseClicked(QPoint point);        //鼠标单击
    void mouseDoubleClick(QPoint point);    //双击事件
    void keyPress(QKeyEvent *event);        //按键事件
};

#endif // TGRAPHICSVIEW_H
