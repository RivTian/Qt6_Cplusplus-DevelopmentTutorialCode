#ifndef WIDGET_H
#define WIDGET_H

#include    <QWidget>
#include    <QSoundEffect>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

private:
    QSoundEffect    *player1;
    QSoundEffect    *player2;
    QString     appPath;
    QPixmap     pixBackground;
    void    defense(QString weapon);
    void    attack(QString weapon);
    void    paintEvent(QPaintEvent *event);
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
