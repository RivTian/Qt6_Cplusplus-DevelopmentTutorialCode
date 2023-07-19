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

private slots:
    void on_btnCharJudge_clicked();

    void on_btnGetChars_clicked();

//    void on_btnReplaceChar_clicked();

    void on_btnClear_clicked();

    void on_btnConvLatin1_clicked();

    void on_btnConvUTF16_clicked();

    void on_btnCompare_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
