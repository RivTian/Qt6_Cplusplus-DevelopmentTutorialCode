#ifndef WIDGET_H
#define WIDGET_H

#include    <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event); //在窗口上双击时的响应

private slots:
    void do_doubleClick();  //与lab的doubleClicked()信号关联

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
