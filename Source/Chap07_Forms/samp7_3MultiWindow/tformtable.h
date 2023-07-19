#ifndef TFORMTABLE_H
#define TFORMTABLE_H

#include <QMainWindow>

#include    <QStandardItemModel>

#include    <QItemSelectionModel>

#include    "tdialogheaders.h"
#include    "tdialogsize.h"

QT_BEGIN_NAMESPACE
namespace Ui {class TFormTable;}
QT_END_NAMESPACE

class TFormTable : public QMainWindow
{
    Q_OBJECT

private:
    TDialogHeaders *dlgSetHeaders=nullptr; //设置表头的对话框

    QStandardItemModel  *m_model;      //数据模型
    QItemSelectionModel *m_selection;  //选择模型

public:
    TFormTable(QWidget *parent = 0);
    ~TFormTable();

private slots:
    void on_actSize_triggered();

    void on_actSetHeader_triggered();

private:
    Ui::TFormTable *ui;
};

#endif // TFORMTABLE_H
