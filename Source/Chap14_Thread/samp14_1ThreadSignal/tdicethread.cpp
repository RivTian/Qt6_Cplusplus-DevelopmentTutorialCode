#include    "tdicethread.h"
//#include    <QTime>

#include    <QRandomGenerator>

TDiceThread::TDiceThread(QObject *parent)
    : QThread{parent}
{

}


void TDiceThread::diceBegin()
{ //开始掷骰子
    m_paused=false;
}

void TDiceThread::dicePause()
{//暂停掷骰子
    m_paused=true;
}

void TDiceThread::stopThread()
{//停止线程
    m_stop=true;
}

void TDiceThread::run()
{//线程的事件循环
    m_stop=false;       //启动线程时令m_stop=false
    m_paused=true;      //启动运行后暂时不掷骰子
    m_seq=0;            //掷骰子次数
    while(!m_stop)      //循环主体
    {
        if (!m_paused)
        {
            m_diceValue= QRandomGenerator::global()->bounded(1,7);  //产生随机数[1,6]
            m_seq++;
            emit newValue(m_seq, m_diceValue);  //发射信号
        }
        msleep(500);    //线程休眠500ms
    }

//  在  m_stop==true时结束线程任务
    quit();     //相当于exit(0), 退出线程的事件循环
}

