#include "tmylabel.h"


#include <QMouseEvent>

void TMyLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked();
    event->accept();
}

TMyLabel::TMyLabel(QWidget *parent):QLabel(parent)
{

}

