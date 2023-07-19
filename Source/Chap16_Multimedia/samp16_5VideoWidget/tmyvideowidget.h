#ifndef TMYVIDEOWIDGET_H
#define TMYVIDEOWIDGET_H

#include    <QObject>
#include    <QWidget>
#include    <QMediaPlayer>
#include    <QVideoWidget>

class TMyVideoWidget : public QVideoWidget
{
    Q_OBJECT
private:
    QMediaPlayer    *m_player;

protected:
    void keyPressEvent(QKeyEvent *event);

    void mousePressEvent(QMouseEvent *event);

public:
    TMyVideoWidget(QWidget *parent =nullptr);

    void    setMediaPlayer(QMediaPlayer *player);
};

#endif // TMYVIDEOWIDGET_H
