#ifndef TFORMDOC_H
#define TFORMDOC_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {class TFormDoc;}
QT_END_NAMESPACE


class TFormDoc : public QWidget
{
    Q_OBJECT

public:
    TFormDoc(QWidget *parent = nullptr);
    ~TFormDoc();

    void    loadFromFile(QString& aFileName);   //自定义函数

private slots:
    void on_actOpen_triggered();

    void on_actFont_triggered();

signals:
    void titleChanged(QString title);       //自定义信号

private:
    Ui::TFormDoc *ui;
};

#endif // TFORMDOC_H
