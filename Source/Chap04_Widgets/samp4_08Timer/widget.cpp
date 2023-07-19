#include "widget.h"
#include "ui_widget.h"

#include    <QTime>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->groupBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);//"定时器"分组框垂直方向固定尺寸
    ui->groupBox_3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);//"定时器精度"分组框垂直方向固定尺寸

    m_timer=new QTimer(this);   //创建定时器
    m_timer->stop();            //先停止定时器
    m_timer->setTimerType(Qt::CoarseTimer);     //定时器精度等级
    ui->radioCoarse->setChecked(true);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(do_timer_timeout()));   //关联定时器的信号与槽
}

Widget::~Widget()
{
    delete ui;
}

//与定时器的timeout()信号关联的槽函数
void Widget::do_timer_timeout()
{
    QApplication::beep();   //使系统的蜂鸣器发声
    QTime curTime=QTime::currentTime();     //获取当前时间
    ui->LCDHour->display(curTime.hour());   //LCD显示  小时
    ui->LCDMin->display(curTime.minute());  //LCD显示  分钟
    ui->LCDSec->display(curTime.second());  //LCD显示  秒

    if (m_timer->isSingleShot())    //如果是单次定时，显示流逝的时间
    {
        int tmMsec=m_counter.elapsed();  //毫秒数
        QString str=QString("流逝的时间：%1毫秒").arg(tmMsec);
        ui->labElapsedTime->setText(str); //显示流逝的时间

        ui->btnStart->setEnabled(true);
        ui->btnOneShot->setEnabled(true);
        ui->btnStop->setEnabled(false);
    }
}

//与动态创建的单次定时器的timeout()信号关联的槽函数
void Widget::do_timer_shot()
{
    QApplication::beep();
    int tmMsec=m_counter.elapsed();  //流逝的时间：毫秒
    QString str=QString("流逝的时间：%1毫秒").arg(tmMsec);
    ui->labElapsedTime->setText(str);
    ui->btnOneShot->setEnabled(true);
}


//“开始”按钮
void Widget::on_btnStart_clicked()
{
    m_timer->setInterval(ui->spinBoxIntv->value()); //设置定时器的周期

    //单次定时或连续定时
    if (ui->radioContiue->isChecked())
        m_timer->setSingleShot(false);  //连续定时
    else
        m_timer->setSingleShot(true);   //单次定时

    //定时器精度
    if (ui->radioPrecise->isChecked())
        m_timer->setTimerType(Qt::PreciseTimer);
    else if (ui->radioCoarse->isChecked())
        m_timer->setTimerType(Qt::CoarseTimer);
    else
        m_timer->setTimerType(Qt::VeryCoarseTimer);


    m_timer->start();     //启动定时器
    m_counter.start();    //启动计时器
    ui->btnStart->setEnabled(false);
    ui->btnOneShot->setEnabled(false);
    ui->btnStop->setEnabled(true);
}

//“停止”按钮
void Widget::on_btnStop_clicked()
{
    m_timer->stop();    //定时器停止

    int tmMsec=m_counter.elapsed();  //流逝的时间：毫秒
    int ms= tmMsec % 1000;  //余数毫秒
    int sec=tmMsec/1000;    //整秒

    QString str=QString("流逝的时间：%1秒，%2毫秒").arg(sec).arg(ms,3,10,QChar('0'));
    ui->labElapsedTime->setText(str); //显示流逝的时间

    ui->btnStart->setEnabled(true);
    ui->btnOneShot->setEnabled(true);
    ui->btnStop->setEnabled(false);
}

//"动态创建单次定时器"按钮
void Widget::on_btnOneShot_clicked()
{
    int intv=ui->spinBoxIntv->value();  //定时周期
    QTimer::singleShot(intv,Qt::PreciseTimer,this,&Widget::do_timer_shot);
    m_counter.start();    //启动计时器
    ui->btnOneShot->setEnabled(false);
}

