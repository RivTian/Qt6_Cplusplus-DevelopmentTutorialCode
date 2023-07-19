#ifndef TMYLABEL_H
#define TMYLABEL_H

#include    <QLabel>
#include    <QMouseEvent>

class TMyLabel : public QLabel
{
    Q_OBJECT
public:
    TMyLabel(QWidget *parent = nullptr);

    bool event(QEvent *e);  //重新实现event()函数

protected:
    void mouseDoubleClickEvent(QMouseEvent *event); //重新实现鼠标双击事件的默认处理函数

signals:
    void doubleClicked();   //自定义信号
};

#endif // TMYLABEL_H
