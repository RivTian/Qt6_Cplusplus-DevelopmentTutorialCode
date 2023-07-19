#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QPaintEvent>
#include    <QPainter>
#include    "tpendialog.h"

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter  painter(this);
    QRect rect(0,0,width(),height());   //viewport矩形区
    painter.setViewport(rect);          //设置Viewport
    painter.setWindow(0,0,100,50);      // 设置窗口大小，逻辑坐标
    painter.setPen(m_pen);
    painter.drawRect(10,10,80,30);

    event->accept();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pen.setColor(Qt::blue);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_Pen_triggered()
{
    bool   ok=false;
    QPen   pen=TPenDialog::getPen(m_pen,&ok);
    if (ok)
    {
        m_pen=pen;
        this->repaint();
    }
}

