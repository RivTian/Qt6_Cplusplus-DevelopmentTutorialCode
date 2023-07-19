#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void UIPropertySetup();//UI界面组件属性设置,某些特别需要注意的属性设置

private slots:
    void on_btnCal_clicked(); //计算按键 clicked()响应

    void on_spinDec_valueChanged(int arg1);

    void on_spinBin_valueChanged(int arg1);

    void on_spinHex_valueChanged(int arg1);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
