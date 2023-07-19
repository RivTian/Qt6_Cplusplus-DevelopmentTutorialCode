#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

private:
    void    myDrawFilledRect();     //填充矩形框

    void    myDrawTextureRect();    //材质填充绘图

    void    myDrawGradient();       //3种渐变

    void    myDrawGradientSpread();       //延展渐变


    void    total();

    void    myDrawShape(); //绘制基本图形

protected:
    void    paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
