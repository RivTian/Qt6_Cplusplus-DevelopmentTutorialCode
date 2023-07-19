#ifndef MYTHREAD_H
#define MYTHREAD_H

#include    <QThread>

//TDaqThread 数据采集线程
class TDaqThread : public QThread
{
    Q_OBJECT
protected:
    bool    m_stop=false;
    void    run();          //线程的任务函数
public:
    explicit TDaqThread(QObject *parent = nullptr);
    void    stopThread();   //结束线程run()函数的运行
};

//TProcessThread 数据处理线程
class TProcessThread : public QThread
{
    Q_OBJECT
protected:
    bool    m_stop=false;
    void    run();          //线程的任务函数
public:
    explicit TProcessThread(QObject *parent = nullptr);
    void    stopThread();   //结束线程run()函数的运行
signals:
    void    dataAvailable(int bufferSeq, int* bufferData, int pointCount);
};


#endif // MYTHREAD_H
