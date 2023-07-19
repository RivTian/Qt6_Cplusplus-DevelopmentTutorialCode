#ifndef TGRAPHICSVIEW_H
#define TGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>

class TGraphicsView : public QGraphicsView
{
    Q_OBJECT

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

public:
    TGraphicsView(QWidget *parent = nullptr);

signals:
    void mouseMovePoint(QPoint point);
    void mouseClicked(QPoint point);
};

#endif // TGRAPHICSVIEW_H
