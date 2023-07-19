#ifndef TMYLABEL_H
#define TMYLABEL_H

#include <QLabel>
#include <QObject>

class TMyLabel : public QLabel
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *event);
public:
    TMyLabel(QWidget *parent = nullptr);

signals:
    void clicked();
};

#endif // TMYLABEL_H
