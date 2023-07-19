#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

protected:
    void dragEnterEvent(QDragEnterEvent *event);

    //   void dragMoveEvent(QDragMoveEvent *event);
    void resizeEvent(QResizeEvent *event);

    void dropEvent(QDropEvent *event);

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private slots:

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
