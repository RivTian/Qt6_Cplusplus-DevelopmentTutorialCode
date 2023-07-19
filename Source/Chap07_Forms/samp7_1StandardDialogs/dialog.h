#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {class Dialog;}


QT_BEGIN_NAMESPACE
namespace Ui {class Dialog;}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void    do_progress_canceled();

    void on_btnOpen_clicked();

//    void on_btnClearText_clicked();

    void on_btnOpenMulti_clicked();

    void on_btnSelDir_clicked();

    void on_btnSave_clicked();

    void on_btnColor_clicked();

    void on_btnFont_clicked();

    void on_btnInputString_clicked();

    void on_btnInputInt_clicked();

    void on_btnInputFloat_clicked();

    void on_btnInputItem_clicked();

    void on_btnMsgQuestion_clicked();

    void on_btnMsgInformation_clicked();

    void on_btnMsgWarning_clicked();

    void on_btnMsgCritical_clicked();

    void on_btnMsgAbout_clicked();

    void on_btnMsgAboutQt_clicked();

//    void on_btnProgress_clicked();

    void on_btnProgress_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
