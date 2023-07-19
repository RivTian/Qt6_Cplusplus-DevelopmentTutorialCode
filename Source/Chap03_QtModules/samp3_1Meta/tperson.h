#ifndef TPERSON_H
#define TPERSON_H

#include <QObject>

class TPerson : public QObject
{
    Q_OBJECT

    Q_CLASSINFO("author","Wang")    //定义附加的类信息
    Q_CLASSINFO("company","UPC")
    Q_CLASSINFO("version","2.0.0")

    Q_PROPERTY(int age READ age WRITE setAge NOTIFY ageChanged)     //定义属性age
    Q_PROPERTY(QString name MEMBER m_name)      //定义属性name
    Q_PROPERTY(int score MEMBER m_score)        //定义属性score
private:
    int  m_age=10;
    int  m_score=79;
    QString m_name;
public:
    explicit TPerson(QString aName, QObject *parent = nullptr);
    ~TPerson();

    int     age();
    void    setAge(int value);
    void    incAge();
signals:
    void    ageChanged( int  value);        //自定义信号

public slots:
};

#endif // TPERSON_H
