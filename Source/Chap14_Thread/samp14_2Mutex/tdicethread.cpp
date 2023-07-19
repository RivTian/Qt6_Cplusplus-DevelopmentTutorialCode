#include    "tdicethread.h"
//#include    <QTime>

#include    <QRandomGenerator>
#include    <QMutexLocker>

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

bool TDiceThread::readValue(int *seq, int *diceValue)
{
    if (mutex.tryLock(100))  //尝试锁定互斥量，等待100ms
    {
        *seq=m_seq;
        *diceValue=m_diceValue;
        mutex.unlock();     //解锁互斥量
        return true;
    }
    else
        return false;
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
            QMutexLocker locker(&mutex);
//            mutex.lock();       //锁定互斥量
            m_diceValue=0;
            for(int i=0; i<5; i++)
                m_diceValue += QRandomGenerator::global()->bounded(1,7);  //产生随机数[1,6]
            m_diceValue =m_diceValue/5;
            m_seq++;
//            mutex.unlock();     //解锁互斥量
        }
        msleep(500);    //线程休眠500ms
    }
    quit();     //在  m_stop==true时结束线程任务
}

