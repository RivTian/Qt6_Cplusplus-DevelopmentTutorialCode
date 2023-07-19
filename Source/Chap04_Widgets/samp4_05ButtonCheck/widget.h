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

    void on_btnAlign_Left_clicked();

    void on_btnAlign_Center_clicked();

    void on_btnAlign_Right_clicked();

    void on_btnFont_Bold_clicked(bool checked);

    void on_btnFont_Italic_clicked(bool checked);

    void on_btnFont_UnderLine_clicked(bool checked);

    void on_chkBox_Readonly_clicked(bool checked);

    void on_chkbox_Enable_clicked(bool checked);

    void on_chkBox_ClearButton_clicked(bool checked);

    void on_radioBlack_clicked();

    void on_radioRed_clicked();

    void on_radioBlue_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
