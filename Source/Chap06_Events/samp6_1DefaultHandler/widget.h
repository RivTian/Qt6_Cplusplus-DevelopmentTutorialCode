#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QResizeEvent>

#include <QCloseEvent>

#include <QKeyEvent>

#include <QPaintEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


class Widget : public QWidget
{
   Q_OBJECT

protected:
   void paintEvent(QPaintEvent *event);

   void closeEvent(QCloseEvent *event);

//   void keyReleaseEvent(QKeyEvent *event);

   void keyPressEvent(QKeyEvent *event);

   void showEvent(QShowEvent *event);

   void hideEvent(QHideEvent *event);

   void mousePressEvent(QMouseEvent *event);

public:
    Widget(QWidget *parent = 0);
   ~Widget();

private:
   Ui::Widget *ui;
};

#endif // WIDGET_H
