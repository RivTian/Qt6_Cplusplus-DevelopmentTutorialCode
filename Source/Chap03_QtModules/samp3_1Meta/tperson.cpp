#include "tperson.h"

TPerson::TPerson(QString aName,QObject *parent) : QObject(parent)
{ //构造函数
    m_name= aName;
}

TPerson::~TPerson()
{  //析构函数里显示信息
    qDebug("TPerson对象被删除了");
}

int TPerson::age()
{ //返回age
    return  m_age;
}

void TPerson::setAge(int value)
{//设置age
    if ( m_age != value)
    {
        m_age= value;
        emit ageChanged(m_age);  //发射信号
    }
}

void TPerson::incAge()
{
    m_age++;
    emit ageChanged(m_age);  //发射信号
}

