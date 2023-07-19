#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QAbstractItemView>
#include <QGroupBox>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

private:
    int getDropActionIndex(Qt::DropAction actionType);  //将枚举值转为index
    Qt::DropAction getDropActionType(int index);        //将index转换为枚举值

    QAbstractItemView*   m_itemView=nullptr;    //当前设置属性的item widget组件
    void  refreshToUI(QGroupBox * curGroupBox);     //组件m_itemView的属性显示到界面上

protected:
    bool eventFilter(QObject *watched, QEvent *event);

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_radio_Source_clicked();

    void on_radio_List_clicked();

    void on_radio_Tree_clicked();

    void on_radio_Table_clicked();

    void on_chkBox_AcceptDrops_clicked(bool checked);

    void on_chkBox_DragEnabled_clicked(bool checked);

    void on_combo_Mode_currentIndexChanged(int index);

    void on_combo_DefaultAction_currentIndexChanged(int index);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
