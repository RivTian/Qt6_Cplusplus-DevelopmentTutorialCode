#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QRadioButton>
#include <QPlainTextEdit>
#include <QPushButton>
//#include <QVBoxLayout>

class Dialog : public QDialog
{
    Q_OBJECT

private:
    QCheckBox   *chkBoxUnder;
    QCheckBox   *chkBoxItalic;
    QCheckBox   *chkBoxBold;

    QRadioButton    *radioBlack;
    QRadioButton    *radioRed;
    QRadioButton    *radioBlue;

    QPlainTextEdit  *txtEdit;

    QPushButton     *btnOK;
    QPushButton     *btnCancel;
    QPushButton     *btnClose;

    void    iniUI();            //UI创建与初始化
    void    iniSignalSlots();   //初始化信号与槽的链接

private slots:
    void do_chkBoxUnder(bool checked);  //Underline

    void do_chkBoxItalic(bool checked); //Italic

    void do_chkBoxBold(bool checked);   //Bold

    void do_setFontColor();             //设置文字颜色

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

};

#endif // DIALOG_H
