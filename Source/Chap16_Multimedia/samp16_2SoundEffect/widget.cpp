#include "widget.h"
#include "ui_widget.h"

#include    <QPainter>
#include    <QPaintEvent>

void Widget::defense(QString weapon)
{
    QUrl   url=QUrl::fromLocalFile(appPath+"/sound/"+weapon);
    player1->setSource(url);
    player1->play();
}

void Widget::attack(QString weapon)
{
    QUrl   url=QUrl::fromLocalFile(appPath+"/sound/"+weapon);
    player2->setSource(url);
    player2->play();
}

void Widget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawPixmap(0,0,this->width(), this->height(),pixBackground);
	event->accept();
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    appPath=QCoreApplication::applicationDirPath(); //  无“/”

    pixBackground.load(appPath+"/sound/background.jpg");

    player1=new QSoundEffect(this);
    player1->setLoopCount(3);

    player2=new QSoundEffect(this);
    player2->setLoopCount(3);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    defense("Ak7.wav");
}

void Widget::on_pushButton_2_clicked()
{
    defense("machinegun.wav");
}

void Widget::on_pushButton_5_clicked()
{
    attack("Ak7.wav");
}

void Widget::on_pushButton_6_clicked()
{
    attack("machinegun.wav");
}

void Widget::on_pushButton_8_clicked()
{
    attack("blast.wav");
}

void Widget::on_pushButton_12_clicked()
{
    attack("tank.wav");
}

void Widget::on_pushButton_11_clicked()
{
    attack("mine.wav");
}

void Widget::on_pushButton_9_clicked()
{
    defense("shell.wav");
}

void Widget::on_pushButton_4_clicked()
{
    defense("blast.wav");
}

void Widget::on_pushButton_10_clicked()
{
    defense("blast2.wav");
}

void Widget::on_pushButton_3_clicked()
{
     defense("fire.wav");
}

void Widget::on_pushButton_7_clicked()
{
    attack("fire2.wav");
}
