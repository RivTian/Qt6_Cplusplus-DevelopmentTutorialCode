#ifndef TDICETHREAD_H
#define TDICETHREAD_H

#include    <QThread>
#include    <QMutex>

class TDiceThread : public QThread
{
    Q_OBJECT
private:
    QMutex  mutex;  //互斥量
    int     m_seq=0;        //掷骰子次数序号
    int     m_diceValue;    //骰子点数
    bool    m_paused=true;  //暂停次骰子
    bool    m_stop=false;   //停止线程run()
protected:
    void    run();      //线程的事件循环
public:
    explicit TDiceThread(QObject *parent = nullptr);

    void    diceBegin();    //开始掷骰子
    void    dicePause();    //暂停
    void    stopThread();   //结束线程run()

    bool    readValue(int *seq, int *diceValue); //用于主线程读取数据的函数

};

#endif // TDICETHREAD_H
